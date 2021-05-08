#include "skinchanger.h"

// used: create entity
#include "../utilities.h"
// used: client, engine interfaces
#include "../core/interfaces.h"
#include "../utilities/logging.h"
// used: cheat variables
#include "../core/variables.h"

void CSkinChanger::Run()
{
	// StatTrak™ AWP | Dragon Lore
	g_SkinChangerCfg[WEAPON_AWP].nFallbackPaintKit = 344;
	g_SkinChangerCfg[WEAPON_AWP].nFallbackStatTrak = 1337;

	// Valve AK-47 | Serpant
	g_SkinChangerCfg[WEAPON_AK47].nFallbackPaintKit = 180;
	g_SkinChangerCfg[WEAPON_AK47].iEntityQuality = 6;

	// Souvenir M4A4 | Howl
	g_SkinChangerCfg[WEAPON_M4A1].nFallbackPaintKit = 309;
	g_SkinChangerCfg[WEAPON_M4A1].iEntityQuality = 12;

	// Souvenir SSG08 |
	g_SkinChangerCfg[WEAPON_SSG08].nFallbackPaintKit = 222;

	// Souvenir M4A1-S | 430
	g_SkinChangerCfg[WEAPON_M4A1_SILENCER].nFallbackPaintKit = 430;
	g_SkinChangerCfg[WEAPON_M4A1_SILENCER].iEntityQuality = 12;

	// Prototype Desert Eagle | Conspiracy
	g_SkinChangerCfg[WEAPON_DEAGLE].nFallbackPaintKit = 351;
	g_SkinChangerCfg[WEAPON_DEAGLE].iEntityQuality = 7;

	// Glock-18 | Fade
	g_SkinChangerCfg[WEAPON_GLOCK].nFallbackPaintKit = 38;

	// USP-S | Stainless
	g_SkinChangerCfg[WEAPON_USP_SILENCER].nFallbackPaintKit = 60;

	g_SkinChangerCfg[WEAPON_KNIFE].nFallbackPaintKit = 416;
	g_SkinChangerCfg[WEAPON_KNIFE].iEntityQuality = 3;

	g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackPaintKit = 416;
	g_SkinChangerCfg[WEAPON_KNIFE_T].iEntityQuality = 3;

	CBaseEntity* pLocal = CBaseEntity::GetLocalPlayer();

	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	PlayerInfo_t pInfo;
	if (!I::Engine->GetPlayerInfo(I::Engine->GetLocalPlayer(), &pInfo))
		return;

	// it will help you here my sweetest candy <3
	// https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Skin_Changer

	
	//ApplyCustomSkin(pLocal, pWeapon);

	//for (int i = 0; i < I::ClientEntityList->GetHighestEntityIndex(); i++) {
	//	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)I::ClientEntityList->GetClientEntityFromHandle(i);
	//	if (!weapon) // null check
	//		continue;

	//	if (pWeapons[i].GetIndex() != weapon->GetIndex())
	//		continue;

	//	ApplyCustomSkin(pLocal, weapon);
	//}

	if (const auto hWeapons = pLocal->GetWeaponsHandle(); hWeapons != nullptr)
	{
		// -1 to prevent double active weapon
		for (int nIndex = MAX_WEAPONS - 1; hWeapons[nIndex]; nIndex--)
		{
			// get current weapon
			CBaseCombatWeapon* pCurrentWeapon = I::ClientEntityList->Get<CBaseCombatWeapon>(hWeapons[nIndex]);

			if (pCurrentWeapon == nullptr)
				continue;

			ApplyCustomSkin(pLocal, pCurrentWeapon);

		}
	}

	CBaseViewModel* view_model = reinterpret_cast<CBaseViewModel*>(I::ClientEntityList->GetClientEntityFromHandle(pLocal->GetViewModelHandle()));

	if (!view_model)
		return;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	const int nDefinitionIndex = pLocal->GetWeapon()->GetItemDefinitionIndex();

	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

	int knife_model = I::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");

	pWeapon->GetItemIDHigh() = -1;

	if (pWeaponData->nWeaponType == WEAPONTYPE_KNIFE) {
		view_model->GetModelIndex() = knife_model;
	}
}

void CSkinChanger::Event(IGameEvent* pEvent, const FNV1A_t uNameHash)
{
	if (!I::Engine->IsInGame())
		return;
}

void CSkinChanger::Dump()
{
}

bool CSkinChanger::ApplyCustomSkin(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon) {

	if (!pWeapon)
		return false;

	// Get the weapons item definition index.
	int nWeaponIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nWeaponIndex);

	if (pWeaponData == nullptr)
		return false;

	// Edit "m_iItemIDHigh" so fallback values will be used.
	pWeapon->GetItemIDHigh() = -1;


	// Check if this weapon has a valid override defined.
	if (g_SkinChangerCfg.find(nWeaponIndex) == g_SkinChangerCfg.end())
		return false;
	// Apply our changes to the fallback variables.
	pWeapon->GetFallbackPaintKit() = g_SkinChangerCfg[nWeaponIndex].nFallbackPaintKit;
	pWeapon->GetEntityQuality() = g_SkinChangerCfg[nWeaponIndex].iEntityQuality;
	pWeapon->GetFallbackSeed() = g_SkinChangerCfg[nWeaponIndex].nFallbackSeed;
	pWeapon->GetFallbackStatTrak() = g_SkinChangerCfg[nWeaponIndex].nFallbackStatTrak;
	pWeapon->GetFallbackWear() = g_SkinChangerCfg[nWeaponIndex].flFallbackWear;
	
	if (pWeaponData->nWeaponType == WEAPONTYPE_KNIFE) {
		int knife_model = I::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		pWeapon->SetModelIndex(knife_model);
		pWeapon->GetViewModelIndex() = knife_model;
		pWeapon->GetWorldModelIndex() = knife_model + 1;
		pWeapon->GetItemDefinitionIndex() = WEAPON_KNIFE_KARAMBIT;
	}

	// If a name is defined, write it now.
	if (g_SkinChangerCfg[nWeaponIndex].szCustomName) {
		sprintf_s(pWeapon->GetCustomName(), 32, "%s", g_SkinChangerCfg[nWeaponIndex].szCustomName);
	}

	/*L::PushConsoleColor(FOREGROUND_RED);
	L::Print(fmt::format(XorStr("[SkinApply] skin id {} "), pWeapon->GetItemDefinitionIndex()));
	L::Print(fmt::format(XorStr("[SkinApply] Paint Kit id {} "), pWeapon->GetFallbackPaintKit()));
	L::PopConsoleColor();*/
	
	return true;
}

void CSkinChanger::SkinChangerEvent(IGameEvent* pEvent, const FNV1A_t uNameHash)
{
	if (!I::Engine->IsInGame())
		return;

	CBaseEntity* pLocal = CBaseEntity::GetLocalPlayer();

	if (pLocal == nullptr || !pLocal->IsAlive())
		return;

	const float flServerTime = TICKS_TO_TIME(pLocal->GetTickBase());

	if (uNameHash == FNV1A::HashConst("player_death"))
	{
		CBaseEntity* pAttacker = I::ClientEntityList->Get<CBaseEntity>(I::Engine->GetPlayerForUserID(pEvent->GetInt(XorStr("attacker"))));

		if (pAttacker == pLocal)
		{
			CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(I::Engine->GetPlayerForUserID(pEvent->GetInt(XorStr("userid"))));

			if (pEntity != nullptr && pEntity != pLocal)
			{
				CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

				const int nDefinitionIndex = pLocal->GetWeapon()->GetItemDefinitionIndex();

				CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

				if (C::Get<bool>(Vars.bSkinChanger) && pWeaponData->nWeaponType == WEAPONTYPE_KNIFE)
					pEvent->SetString("weapon", "knife_karambit");
			}
		}
	}
	else if (uNameHash == FNV1A::HashConst("round_start")) {
		// U::ForceFullUpdate();
	}
}