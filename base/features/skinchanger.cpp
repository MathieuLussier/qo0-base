#include "skinchanger.h"

// used: create entity
#include "../utilities.h"
// used: client, engine interfaces
#include "../core/interfaces.h"
#include "../utilities/logging.h"

void CSkinChanger::Run()
{
	// StatTrak™ AWP | Dragon Lore
	g_SkinChangerCfg[WEAPON_AWP].nFallbackPaintKit = 344;
	g_SkinChangerCfg[WEAPON_AWP].flFallbackWear = 0.00000001f;
	g_SkinChangerCfg[WEAPON_AWP].nFallbackStatTrak = 1337;

	// Valve AK-47 | Serpant
	g_SkinChangerCfg[WEAPON_AK47].nFallbackPaintKit = 180;
	g_SkinChangerCfg[WEAPON_AK47].iEntityQuality = 6;

	// Souvenir M4A4 | Howl
	g_SkinChangerCfg[WEAPON_M4A1].nFallbackPaintKit = 309;
	g_SkinChangerCfg[WEAPON_M4A1].iEntityQuality = 12;

	// Souvenir M4A4 | Howl
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

	CBaseEntity* pLocal = CBaseEntity::GetLocalPlayer();

	if (pLocal == nullptr || !pLocal->IsAlive())
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

			const short nDefinitionIndex = pCurrentWeapon->GetItemDefinitionIndex();

			ApplyCustomSkin(pLocal, pCurrentWeapon);
		
		}
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

	if (pWeaponData->nWeaponType == WEAPONTYPE_KNIFE) {
		/*PlayerInfo_t pInfo;
		if (!I::Engine->GetPlayerInfo(I::Engine->GetLocalPlayer(), &pInfo))
			return false;
		int knife_model = I::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		pWeapon->SetModelIndex(knife_model);
		pWeapon->GetFallbackPaintKit() = 415;
		pWeapon->GetEntityQuality() = 3;
		pWeapon->GetItemDefinitionIndex() = WEAPON_KNIFE_KARAMBIT;
		pWeapon->GetAccountID() = pInfo.nXuidLow;
		L::PushConsoleColor(FOREGROUND_RED);
		L::Print(fmt::format(XorStr("[SkinApply] knife model index {} "), knife_model));
		L::Print(fmt::format(XorStr("[SkinApply] model index {} "), pWeapon->GetViewModelIndex()));
		L::Print(fmt::format(XorStr("[SkinApply] skin id {} "), pWeapon->GetItemDefinitionIndex()));
		L::Print(fmt::format(XorStr("[SkinApply] Paint Kit id {} "), pWeapon->GetFallbackPaintKit()));
		L::PopConsoleColor();*/
	}
	else {
		// Check if this weapon has a valid override defined.
		if (g_SkinChangerCfg.find(nWeaponIndex) == g_SkinChangerCfg.end())
			return false;
		// Apply our changes to the fallback variables.
		pWeapon->GetFallbackPaintKit() = g_SkinChangerCfg[nWeaponIndex].nFallbackPaintKit;
		pWeapon->GetEntityQuality() = g_SkinChangerCfg[nWeaponIndex].iEntityQuality;
		pWeapon->GetFallbackSeed() = g_SkinChangerCfg[nWeaponIndex].nFallbackSeed;
		pWeapon->GetFallbackStatTrak() = g_SkinChangerCfg[nWeaponIndex].nFallbackStatTrak;
		pWeapon->GetFallbackWear() = g_SkinChangerCfg[nWeaponIndex].flFallbackWear;
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