#include "lagcompensation.h"

// used: globals interface
#include "../core/interfaces.h"

void CLagCompensation::Run(CUserCmd* pCmd)
{
	/*
	 * we have much public info for that
	 * now it is your own way gl
	 */
}

void CLagCompensation::UpdateIncomingSequences(INetChannel* pNetChannel)
{
	if (pNetChannel == nullptr)
		return;

	// set to real sequence to update, otherwise needs time to get it work again
	if (nLastIncomingSequence == 0)
		nLastIncomingSequence = pNetChannel->iInSequenceNr;

	// check how much sequences we can spike
	if (pNetChannel->iInSequenceNr > nLastIncomingSequence)
	{
		nLastIncomingSequence = pNetChannel->iInSequenceNr;
		vecSequences.emplace_front(SequenceObject_t(pNetChannel->iInReliableState, pNetChannel->iOutReliableState, pNetChannel->iInSequenceNr, I::Globals->flRealTime));
	}

	// is cached too much sequences
	if (vecSequences.size() > 2048U)
		vecSequences.pop_back();
}

void CLagCompensation::ClearIncomingSequences()
{
	if (!vecSequences.empty())
	{
		nLastIncomingSequence = 0;
		vecSequences.clear();
	}
}

void CLagCompensation::AddLatencyToNetChannel(INetChannel* pNetChannel, float flLatency)
{
	for (const auto& sequence : vecSequences)
	{
		if (I::Globals->flRealTime - sequence.flCurrentTime >= flLatency)
		{
			pNetChannel->iInReliableState = sequence.iInReliableState;
			pNetChannel->iInSequenceNr = sequence.iSequenceNr;
			break;
		}
	}
}

//void CLagCompensation::AnimFix(CBaseEntity* entity)
//{
//	if (!should_lag_compensate(entity) || !entity->is_self_animating())
//		return;
//
//	auto player_index = entity->GetIndex() - 1;
//	auto player_record = &G::player_lag_records[player_index];
//	
//	if (player_record->records->size() < 2)
//		return;
//
//	entity->IsClientSideAnimation() = true;
//
//	auto old_curtime = G::interfaces.globaldata->curtime;
//	auto old_frametime = G::interfaces.globaldata->frametime;
//	auto old_ragpos = entity->get_ragdoll_pos();
//
//	G::interfaces.globaldata->curtime = entity->get_simulation_time();
//	G::interfaces.globaldata->frametime = G::interfaces.globaldata->interval_per_tick;
//
//	auto player_animation_state = reinterpret_cast<DWORD*>(entity + 0x3894);
//	auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
//	if (player_animation_state != nullptr && player_model_time != nullptr)
//		if (*player_model_time == G::interfaces.globaldata->framecount)
//			*player_model_time = G::interfaces.globaldata->framecount - 1;
//
//	entity->get_ragdoll_pos() = old_ragpos;
//	entity->update_clientside_animations();
//
//	G::interfaces.globaldata->curtime = old_curtime;
//	G::interfaces.globaldata->frametime = old_frametime;
//
//	entity->client_side_animation() = false;
//}