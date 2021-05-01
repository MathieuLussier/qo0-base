#pragma once
// used: define to add values to variables vector
#include "config.h"

#pragma region variables_array_entries
enum EVisualsInfoFlags : int
{
	INFO_FLAG_HELMET = 0,
	INFO_FLAG_KEVLAR,
	INFO_FLAG_KIT,
	INFO_FLAG_ZOOM,
	INFO_FLAG_MAX
};

enum EVisualsRemovals : int
{
	REMOVAL_POSTPROCESSING = 0,
	REMOVAL_PUNCH,
	REMOVAL_SMOKE,
	REMOVAL_SCOPE,
	REMOVAL_MAX
};
#pragma endregion

#pragma region variables_combo_entries

enum class EHitScanMode : int
{
	LOW = 0,
	NORMAL,
	HIGH,
	EXTREME
};

enum class EAntiAimPitchType : int
{
	NONE = 0,
	UP,
	DOWN,
	ZERO
};

enum class EAimType : int
{
	NORMAL = 0,
	SILENT,
	PSILENT
};

enum class ETargetSelection : int
{
	CROSSHAIR = 0,
	DISTANCE,
	HEALTH
};

enum class EAntiAimYawType : int
{
	NONE = 0,
	DESYNC,
	Jitter,
	BackJitter
};

enum class EVisualsBoxType : int
{
	NONE = 0,
	FULL,
	CORNERS
};

enum class EVisualsGlowStyle : int
{
	OUTER = 0,
	COVER,
	INNER
};

enum class EVisualsPlayersChams : int
{
	COVERED = 0,
	FLAT,
	WIREFRAME,
	REFLECTIVE
};

enum class EVisualsViewModelChams : int
{
	NO_DRAW = 0,
	COVERED,
	FLAT,
	WIREFRAME,
	GLOW,
	SCROLL,
	CHROME
};
#pragma endregion

struct Variables_t
{
	#pragma region variables_rage
	// ragebot
	C_ADD_VARIABLE(bool, bRage, false);

	C_ADD_VARIABLE(int, iRageAimType, 0);
	
	C_ADD_VARIABLE(int, ihitchance, 0);
	C_ADD_VARIABLE(int, imin_damage, 0);

	C_ADD_VARIABLE(int, iHitScanMode, 0);
	C_ADD_VARIABLE_VECTOR(bool, 5, vecRageTargetHitbox, false);
	C_ADD_VARIABLE(bool, bAimStep, false);
	C_ADD_VARIABLE(bool, bBestPoint, false);
	C_ADD_VARIABLE(bool, bPreferBodyAim, false);
	C_ADD_VARIABLE(bool, bNoRecoil, false);
	C_ADD_VARIABLE(bool, bAutoScope, false);
	C_ADD_VARIABLE(bool, bRageAutoFire, false);
	C_ADD_VARIABLE(bool, bPositionAdjustment, false);
	C_ADD_VARIABLE(bool, bAccuracyAutoStop, false);
	C_ADD_VARIABLE(bool, bAimbotAutoPistol, false);
	C_ADD_VARIABLE(bool, bRageAutoWall, false);
	C_ADD_VARIABLE(int, iTargetSelection, 0);
	C_ADD_VARIABLE(int, iSmart, 1);
	C_ADD_VARIABLE(float, flPointScale, 5.f);
	//C_ADD_VARIABLE(float, flHeadPointScale, 0.f);
	//C_ADD_VARIABLE(float, flChestPointScale, 0.f);

	// antiaim
	C_ADD_VARIABLE(bool, bAntiAim, false);
	C_ADD_VARIABLE(int, iAntiAimPitch, 0);
	C_ADD_VARIABLE(int, iAntiAimYaw, 0);
	C_ADD_VARIABLE(int, iAntiAimDesyncKey, VK_XBUTTON1);
	#pragma endregion

	#pragma region variables_legit
	// aimbot
	C_ADD_VARIABLE(bool, bLegit, false);
	C_ADD_VARIABLE(bool, bAimbotBetweenShots, false);
	C_ADD_VARIABLE_VECTOR(bool, 5, vecAimbotHitboxes, false);
	C_ADD_VARIABLE(int, iAimbotKey, 0);
	C_ADD_VARIABLE(float, flLegitFov, 0.f);
	C_ADD_VARIABLE(float, flLegitSmooth, 0.f);
	C_ADD_VARIABLE(bool, bLegitSilent, false);
	C_ADD_VARIABLE(float, flLegitSilentFov, 0.f);
	C_ADD_VARIABLE(float, flLegitRecoilX, 0.f);
	C_ADD_VARIABLE(float, flLegitRecoilY, 0.f);

	// aimbot pistol
	C_ADD_VARIABLE(bool, bAimbotPistol, false);
	C_ADD_VARIABLE(bool, bAimbotBetweenShotsPistol, false);
	C_ADD_VARIABLE_VECTOR(bool, 5, vecPistolHitboxes, false);
	C_ADD_VARIABLE(float, flLegitFovPistol, 0.f);
	C_ADD_VARIABLE(float, flLegitSmoothPistol, 0.f);
	C_ADD_VARIABLE(bool, bLegitSilentPistol, false);
	C_ADD_VARIABLE(float, flLegitSilentFovPistol, 0.f);

	C_ADD_VARIABLE(float, flLegitRecoilXPistol, 0.f);
	C_ADD_VARIABLE(float, flLegitRecoilYPistol, 0.f);


	// aimbot Sniper
	C_ADD_VARIABLE(bool, bAimbotSniper, false);
	C_ADD_VARIABLE(bool, bAimbotBetweenShotsSniper, false);
	C_ADD_VARIABLE_VECTOR(bool, 5, vecSniperHitboxes, false);
	C_ADD_VARIABLE(float, flLegitFovSniper, 0.f);
	C_ADD_VARIABLE(float, flLegitSmoothSniper, 0.f);
	C_ADD_VARIABLE(bool, bLegitSilentSniper, false);
	C_ADD_VARIABLE(float, flLegitSilentFovSniper, 0.f);

	C_ADD_VARIABLE(float, flLegitRecoilXSniper, 0.f);
	C_ADD_VARIABLE(float, flLegitRecoilYSniper, 0.f);

	// aimbot SMG
	C_ADD_VARIABLE(bool, bAimbotSMG, false);
	C_ADD_VARIABLE(bool, bAimbotBetweenShotsSMG, false);
	C_ADD_VARIABLE_VECTOR(bool, 5, vecSMGHitboxes, false);
	C_ADD_VARIABLE(float, flLegitFovSMG, 0.f);
	C_ADD_VARIABLE(float, flLegitSmoothSMG, 0.f);
	C_ADD_VARIABLE(bool, bLegitSilentSMG, false);
	C_ADD_VARIABLE(float, flLegitSilentFovSMG, 0.f);

	C_ADD_VARIABLE(float, flLegitRecoilXSMG, 0.f);
	C_ADD_VARIABLE(float, flLegitRecoilYSMG, 0.f);

	// aimbot Rifle
	C_ADD_VARIABLE(bool, bAimbotRifle, false);
	C_ADD_VARIABLE(bool, bAimbotBetweenShotsRifle, false);
	C_ADD_VARIABLE_VECTOR(bool, 5, vecRifleHitboxes, false);
	C_ADD_VARIABLE(float, flLegitFovRifle, 0.f);
	C_ADD_VARIABLE(float, flLegitSmoothRifle, 0.f);
	C_ADD_VARIABLE(bool, bLegitSilentRifle, false);
	C_ADD_VARIABLE(float, flLegitSilentFovRifle, 0.f);

	C_ADD_VARIABLE(float, flLegitRecoilXRifle, 0.f);
	C_ADD_VARIABLE(float, flLegitRecoilYRifle, 0.f);

	// aimbot Heavy
	C_ADD_VARIABLE(bool, bAimbotHeavy, false);
	C_ADD_VARIABLE(bool, bAimbotBetweenShotsHeavy, false);
	C_ADD_VARIABLE_VECTOR(bool, 5, vecHeavyHitboxes, false);
	C_ADD_VARIABLE(float, flLegitFovHeavy, 0.f);
	C_ADD_VARIABLE(float, flLegitSmoothHeavy, 0.f);
	C_ADD_VARIABLE(bool, bLegitSilentHeavy, false);
	C_ADD_VARIABLE(float, flLegitSilentFovHeavy, 0.f);

	C_ADD_VARIABLE(float, flLegitRecoilXHeavy, 0.f);
	C_ADD_VARIABLE(float, flLegitRecoilYHeavy, 0.f);

	// trigger
	C_ADD_VARIABLE(bool, bTrigger, false);
	C_ADD_VARIABLE(bool, bTriggerAlwaysActive, false);
	C_ADD_VARIABLE(int, iTriggerKey, 0);
	C_ADD_VARIABLE(int, iTriggerDelay, 0);
	C_ADD_VARIABLE(bool, bTriggerAutoWall, false);
	C_ADD_VARIABLE(int, iTriggerMinimalDamage, 70);

	C_ADD_VARIABLE(bool, bTriggerHead, true);
	C_ADD_VARIABLE(bool, bTriggerChest, true);
	C_ADD_VARIABLE(bool, bTriggerStomach, true);
	C_ADD_VARIABLE(bool, bTriggerArms, false);
	C_ADD_VARIABLE(bool, bTriggerLegs, false);

	// triggerbot Pistol
	C_ADD_VARIABLE(bool, bTriggerPistol, false);
	C_ADD_VARIABLE(int, iTriggerDelayPistol, 0);
	C_ADD_VARIABLE(bool, bTriggerAutoWallPistol, false);
	C_ADD_VARIABLE(int, iTriggerMinimalDamagePistol, 70);

	// triggerbot Sniper
	C_ADD_VARIABLE(bool, bTriggerSniper, false);
	C_ADD_VARIABLE(int, iTriggerDelaySniper, 0);
	C_ADD_VARIABLE(bool, bTriggerAutoWallSniper, false);
	C_ADD_VARIABLE(int, iTriggerMinimalDamageSniper, 70);

	// triggerbot SMG
	C_ADD_VARIABLE(bool, bTriggerSMG, false);
	C_ADD_VARIABLE(int, iTriggerDelaySMG, 0);
	C_ADD_VARIABLE(bool, bTriggerAutoWallSMG, false);
	C_ADD_VARIABLE(int, iTriggerMinimalDamageSMG, 70);

	// triggerbot Rifle
	C_ADD_VARIABLE(bool, bTriggerRifle, false);
	C_ADD_VARIABLE(int, iTriggerDelayRifle, 0);
	C_ADD_VARIABLE(bool, bTriggerAutoWallRifle, false);
	C_ADD_VARIABLE(int, iTriggerMinimalDamageRifle, 70);

	// triggerbot Heavy
	C_ADD_VARIABLE(bool, bTriggerHeavy, false);
	C_ADD_VARIABLE(int, iTriggerDelayHeavy, 0);
	C_ADD_VARIABLE(bool, bTriggerAutoWallHeavy, false);
	C_ADD_VARIABLE(int, iTriggerMinimalDamageHeavy, 70);

	// backtrack
	C_ADD_VARIABLE(bool, bBacktrack, false);
	C_ADD_VARIABLE(float, flBacktrack, 1.f);

	#pragma endregion

	#pragma region variables_visuals
	C_ADD_VARIABLE(bool, bEsp, false);

	// main
	C_ADD_VARIABLE(bool, bEspMain, false);
	C_ADD_VARIABLE(bool, bEspMainEnemies, false);
	C_ADD_VARIABLE(bool, bEspMainAllies, false);
	C_ADD_VARIABLE(bool, bEspMainWeapons, false);
	C_ADD_VARIABLE(bool, bEspMainGrenades, false);
	C_ADD_VARIABLE(bool, bEspMainBomb, false);

	// players
	C_ADD_VARIABLE(int, iEspMainPlayerBox, static_cast<int>(EVisualsBoxType::FULL));
	C_ADD_VARIABLE(Color, colEspMainBoxEnemies, Color(160, 60, 60, 255));
	C_ADD_VARIABLE(Color, colEspMainBoxEnemiesWall, Color(200, 185, 110, 255));
	C_ADD_VARIABLE(Color, colEspMainBoxAllies, Color(0, 200, 170, 255));
	C_ADD_VARIABLE(Color, colEspMainBoxAlliesWall, Color(170, 110, 200, 255));

	C_ADD_VARIABLE(bool, bEspMainPlayerFarRadar, false);
	C_ADD_VARIABLE(bool, bEspMainPlayerInfo, false);

	/* left */
	C_ADD_VARIABLE(bool, bEspMainPlayerHealth, true);
	C_ADD_VARIABLE(bool, bEspMainPlayerMoney, false);

	/* top */
	C_ADD_VARIABLE(bool, bEspMainPlayerRank, false);
	C_ADD_VARIABLE(bool, bEspMainPlayerName, true);
	C_ADD_VARIABLE(bool, bEspMainPlayerFlash, false);

	/* right */
	C_ADD_VARIABLE_VECTOR(bool, INFO_FLAG_MAX, vecEspMainPlayerFlags, true);

	/* bottom */
	C_ADD_VARIABLE(bool, bEspMainPlayerWeapons, true);
	C_ADD_VARIABLE(bool, bEspMainPlayerAmmo, true);
	C_ADD_VARIABLE(bool, bEspMainPlayerDistance, false);

	// weapons
	C_ADD_VARIABLE(int, iEspMainWeaponBox, static_cast<int>(EVisualsBoxType::NONE));
	C_ADD_VARIABLE(Color, colEspMainBoxWeapons, Color(255, 255, 255, 220));

	C_ADD_VARIABLE(bool, bEspMainWeaponInfo, false);

	C_ADD_VARIABLE(bool, bEspMainWeaponIcon, true);
	C_ADD_VARIABLE(bool, bEspMainWeaponAmmo, true);
	C_ADD_VARIABLE(bool, bEspMainWeaponDistance, false);

	// glow
	C_ADD_VARIABLE(bool, bEspGlow, false);
	C_ADD_VARIABLE(bool, bEspGlowEnemies, false);
	C_ADD_VARIABLE(bool, bEspGlowAllies, false);
	C_ADD_VARIABLE(bool, bEspGlowWeapons, false);
	C_ADD_VARIABLE(bool, bEspGlowGrenades, false);
	C_ADD_VARIABLE(bool, bEspGlowBomb, false);

	C_ADD_VARIABLE(bool, bEspGlowBloom, false);

	C_ADD_VARIABLE(Color, colEspGlowEnemies, Color(230, 20, 60, 128));
	C_ADD_VARIABLE(Color, colEspGlowEnemiesWall, Color(255, 235, 240, 128));
	C_ADD_VARIABLE(Color, colEspGlowAllies, Color(85, 140, 255, 128));
	C_ADD_VARIABLE(Color, colEspGlowAlliesWall, Color(240, 235, 255, 128));
	C_ADD_VARIABLE(Color, colEspGlowWeapons, Color(80, 0, 225, 128));
	C_ADD_VARIABLE(Color, colEspGlowGrenades, Color(180, 130, 30, 128));
	C_ADD_VARIABLE(Color, colEspGlowBomb, Color(140, 220, 80, 128));
	C_ADD_VARIABLE(Color, colEspGlowBombPlanted, Color(200, 210, 80, 128));

	// chams
	C_ADD_VARIABLE(bool, bEspChams, false);
	C_ADD_VARIABLE(bool, bEspChamsEnemies, false);
	C_ADD_VARIABLE(bool, bEspChamsAllies, false);
	C_ADD_VARIABLE(bool, bEspChamsViewModel, false);

	C_ADD_VARIABLE(bool, bEspChamsXQZ, false);
	C_ADD_VARIABLE(bool, bEspChamsDisableOcclusion, false);
	C_ADD_VARIABLE(int, iEspChamsPlayer, static_cast<int>(EVisualsPlayersChams::COVERED));
	C_ADD_VARIABLE(int, iEspChamsViewModel, static_cast<int>(EVisualsViewModelChams::WIREFRAME));
	C_ADD_VARIABLE(Color, colEspChamsEnemies, Color(180, 65, 65, 255));
	C_ADD_VARIABLE(Color, colEspChamsEnemiesWall, Color(180, 180, 40, 255));
	C_ADD_VARIABLE(Color, colEspChamsAllies, Color(70, 40, 190, 255));
	C_ADD_VARIABLE(Color, colEspChamsAlliesWall, Color(50, 150, 150, 255));
	C_ADD_VARIABLE(Color, colEspChamsViewModel, Color(80, 255, 45, 255));
	C_ADD_VARIABLE(Color, colEspChamsViewModelAdditional, Color(0, 0, 255, 255));

	// world
	C_ADD_VARIABLE(bool, bWorld, false);
	C_ADD_VARIABLE(bool, bWorldNightMode, false);
	C_ADD_VARIABLE(int, iWorldMaxFlash, 100);
	C_ADD_VARIABLE(int, iWorldThirdPersonKey, 0);
	C_ADD_VARIABLE(float, flWorldThirdPersonOffset, 150.f);
	C_ADD_VARIABLE_VECTOR(bool, REMOVAL_MAX, vecWorldRemovals, false);

	// on-screen
	C_ADD_VARIABLE(bool, bScreen, false);
	C_ADD_VARIABLE(float, flScreenCameraFOV, 0.f);
	C_ADD_VARIABLE(float, flScreenViewModelFOV, 0.f);
	C_ADD_VARIABLE(bool, bScreenForceCrosshair, false);
	C_ADD_VARIABLE(bool, bScreenCrosshairRecoil, false);
	C_ADD_VARIABLE(bool, bScreenHitMarker, false);
	C_ADD_VARIABLE(bool, bScreenHitMarkerDamage, false);
	C_ADD_VARIABLE(bool, bScreenHitMarkerSound, false);
	C_ADD_VARIABLE(float, flScreenHitMarkerTime, 1.0f);
	C_ADD_VARIABLE(int, iScreenHitMarkerGap, 5);
	C_ADD_VARIABLE(int, iScreenHitMarkerLenght, 10);
	C_ADD_VARIABLE(Color, colScreenHitMarker, Color(255, 255, 255, 255));
	C_ADD_VARIABLE(Color, colScreenHitMarkerDamage, Color(200, 55, 20, 255));
	#pragma endregion

	#pragma region variables_misc
	// movement
	C_ADD_VARIABLE(bool, bMiscBunnyHop, false);
	C_ADD_VARIABLE(int, iMiscBunnyHopChance, 100);
	C_ADD_VARIABLE(bool, bMiscAutoStrafe, false);

	C_ADD_VARIABLE(bool, bMiscFakeLag, false);
	C_ADD_VARIABLE(bool, bMiscAutoAccept, false);
	C_ADD_VARIABLE(bool, bMiscAutoPistol, false);
	C_ADD_VARIABLE(bool, bMiscNoCrouchCooldown, false);

	// exploits
	C_ADD_VARIABLE(bool, bMiscPingSpike, false);
	C_ADD_VARIABLE(float, flMiscLatencyFactor, 0.4f);
	C_ADD_VARIABLE(bool, bMiscRevealRanks, false);
	C_ADD_VARIABLE(bool, bMiscUnlockInventory, false);
	C_ADD_VARIABLE(bool, bMiscAntiUntrusted, true);
	#pragma endregion

	C_ADD_VARIABLE(bool, bSkinChanger, false);

	#pragma region variables_menu
	C_ADD_VARIABLE(int, iMenuKey, VK_INSERT);
	C_ADD_VARIABLE(int, iPanicKey, VK_END);
	#pragma endregion
};

inline Variables_t Vars;
