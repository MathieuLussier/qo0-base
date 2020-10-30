#include "backtrack.h"

#include "../global.h"
// used: cheat variables
#include "../core/variables.h"
// used: main window open state
#include "../core/menu.h"
// used: key state
#include "../utilities/inputsystem.h"
// used: globals, cliententitylist interfaces
#include "../core/interfaces.h"
// used: backtracking
#include "lagcompensation.h"

#include "../utilities/math.h"

#include "../utilities/draw.h"

#include "../utilities/logging.h"

void CBacktrack::Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket)
{
	if (!(pCmd->iButtons & IN_ATTACK))
		return;

	auto local_player = pLocal;

	auto best_fov{ 255.f };
	CBaseEntity* best_target{ };
	int besst_target_index{ };
	Vector best_target_head_position{ };
	int best_record{ };

	for (int i = 1; i <= I::Globals->nMaxClients; i++) {
		CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);
		if (pEntity == nullptr || !pEntity->IsPlayer() || pEntity->IsDormant() || !pEntity->IsAlive() || pEntity->HasImmunity())
			continue;
		if (pEntity->GetTeam() == pLocal->GetTeam())
			continue;

		// auto head_position = pEntity->GetHitboxPosition(HITBOX_HEAD).value();
		auto head_position = pEntity->GetBonePosition(pEntity->GetBoneByHash(FNV1A::HashConst("head_0"))).value();

		auto angle = M::CalcAngle(pLocal->GetEyePosition(), head_position).Normalize();
		auto fov = std::hypotf(angle.x, angle.y);
		if (fov < best_fov) {
			best_fov = fov;
			best_target = pEntity;
			besst_target_index = i;
			best_target_head_position = head_position;
		}
	}

	if (best_target) {
		if (CBacktrack::records[besst_target_index].size() <= 3)
			return;

		best_fov = 255.f;

		for (size_t i = 0; i < CBacktrack::records[besst_target_index].size(); i++) {
			auto record = &CBacktrack::records[besst_target_index][i];
			if (!record || !CBacktrack::valid_tick(record->simulation_time))
				continue;

			auto angle = M::CalcAngle(pLocal->GetEyePosition(), record->head, pCmd->angViewPoint);
			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < best_fov) {
				best_fov = fov;
				best_record = i;
			}
		}
	}
	
	if (best_record) {
		auto record = records[besst_target_index][best_record];
		pCmd->iTickCount = CBacktrack::time_to_ticks(record.simulation_time);
	}
}

void CBacktrack::update(CBaseEntity* pLocal) {
	if (!pLocal->IsAlive()) {
		if (!CBacktrack::records->empty())
			CBacktrack::records->clear();
		return;
	}

	for (int i = 1; i <= I::Globals->nMaxClients; i++) {
		CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);

		if (pEntity == nullptr || !pEntity->IsPlayer() || pEntity->IsDormant() || !pEntity->IsAlive() || pEntity->HasImmunity()) {
			CBacktrack::records[i].clear();
			continue;
		}
		if (pEntity->GetTeam() == pLocal->GetTeam()) {
			CBacktrack::records[i].clear();
			continue;
		}

		if (CBacktrack::records[i].size() && (records[i].front().simulation_time == pEntity->GetSimulationTime()))
			continue;

		auto var_map = reinterpret_cast<std::uint32_t>(pEntity) + 0x24;
		auto vars_count = *reinterpret_cast<int*>(static_cast<std::uint32_t>(var_map) + 0x14);
		for (int j = 0; j < vars_count; j++)
			*reinterpret_cast<std::uint32_t*>(*reinterpret_cast<std::uint32_t*>(var_map) + j * 0xC) = 0;

		stored_records record{ };
		record.head = pLocal->GetHitboxPosition(HITBOX_HEAD).value();
		record.simulation_time = pEntity->GetSimulationTime();

		pEntity->SetupBones(record.matrix, 128, 0x7FF00, I::Globals->flCurrentTime);

		CBacktrack::records[i].push_front(record);

		while (CBacktrack::records[i].size() > 3 && CBacktrack::records[i].size() > static_cast<size_t>(CBacktrack::time_to_ticks(static_cast<float>(C::Get<float>(Vars.flBacktrack)) / 1000.f)))
			CBacktrack::records[i].pop_back();

		if (auto invalid = std::find_if(std::cbegin(CBacktrack::records[i]), std::cend(records[i]), [&](const stored_records& rec) { return !CBacktrack::valid_tick(rec.simulation_time); }); invalid != std::cend(records[i]))
			records[i].erase(invalid, std::cend(records[i]));
	}
}

float CBacktrack::get_lerp_time() {
	auto ratio = std::clamp(I::ConVar->FindVar("cl_interp_ratio")->GetFloat(), I::ConVar->FindVar("sv_client_min_interp_ratio")->GetFloat(), I::ConVar->FindVar("sv_client_max_interp_ratio")->GetFloat());
	return std::max(I::ConVar->FindVar("cl_interp")->GetFloat(), (ratio / ((I::ConVar->FindVar("sv_maxupdaterate")->GetFloat()) ? I::ConVar->FindVar("sv_maxupdaterate")->GetFloat() : I::ConVar->FindVar("cl_updaterate")->GetFloat())));
}

int CBacktrack::time_to_ticks(float time) {
	return static_cast<int>((0.5f + static_cast<float>(time) / I::Globals->flIntervalPerTick));
}

bool CBacktrack::valid_tick(float simtime) {
	INetChannelInfo* pNetChannelInfo = I::Engine->GetNetChannelInfo();
	auto network = pNetChannelInfo->GetLatency(0);
	if (!network)
		return false;
	auto delta = std::clamp(network + CBacktrack::get_lerp_time(), 0.f, I::ConVar->FindVar("sv_maxunlag")->GetFloat()) - (I::Globals->flCurrentTime - simtime);
	return std::fabsf(delta) <= 0.2f;
}