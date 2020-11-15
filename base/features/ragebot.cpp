#include "ragebot.h"

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

#include "../utilities.h"

#include "../utilities/math.h"

#include "../utilities/logging.h"
#include "autowall.h"

#define TICK_INTERVAL			( I::Globals->flIntervalPerTick )

float CRageBot::g_CapsuleVertices[][3] = {
        { -0.01f, -0.01f, 1.00f },
        { 0.51f, 0.00f, 0.86f },
        { 0.44f, 0.25f, 0.86f },
        { 0.25f, 0.44f, 0.86f },
        { -0.01f, 0.51f, 0.86f },
        { -0.26f, 0.44f, 0.86f },
        { -0.45f, 0.25f, 0.86f },
        { -0.51f, 0.00f, 0.86f },
        { -0.45f, -0.26f, 0.86f },
        { -0.26f, -0.45f, 0.86f },
        { -0.01f, -0.51f, 0.86f },
        { 0.25f, -0.45f, 0.86f },
        { 0.44f, -0.26f, 0.86f },
        { 0.86f, 0.00f, 0.51f },
        { 0.75f, 0.43f, 0.51f },
        { 0.43f, 0.75f, 0.51f },
        { -0.01f, 0.86f, 0.51f },
        { -0.44f, 0.75f, 0.51f },
        { -0.76f, 0.43f, 0.51f },
        { -0.87f, 0.00f, 0.51f },
        { -0.76f, -0.44f, 0.51f },
        { -0.44f, -0.76f, 0.51f },
        { -0.01f, -0.87f, 0.51f },
        { 0.43f, -0.76f, 0.51f },
        { 0.75f, -0.44f, 0.51f },
        { 1.00f, 0.00f, 0.01f },
        { 0.86f, 0.50f, 0.01f },
        { 0.49f, 0.86f, 0.01f },
        { -0.01f, 1.00f, 0.01f },
        { -0.51f, 0.86f, 0.01f },
        { -0.87f, 0.50f, 0.01f },
        { -1.00f, 0.00f, 0.01f },
        { -0.87f, -0.50f, 0.01f },
        { -0.51f, -0.87f, 0.01f },
        { -0.01f, -1.00f, 0.01f },
        { 0.49f, -0.87f, 0.01f },
        { 0.86f, -0.51f, 0.01f },
        { 1.00f, 0.00f, -0.02f },
        { 0.86f, 0.50f, -0.02f },
        { 0.49f, 0.86f, -0.02f },
        { -0.01f, 1.00f, -0.02f },
        { -0.51f, 0.86f, -0.02f },
        { -0.87f, 0.50f, -0.02f },
        { -1.00f, 0.00f, -0.02f },
        { -0.87f, -0.50f, -0.02f },
        { -0.51f, -0.87f, -0.02f },
        { -0.01f, -1.00f, -0.02f },
        { 0.49f, -0.87f, -0.02f },
        { 0.86f, -0.51f, -0.02f },
        { 0.86f, 0.00f, -0.51f },
        { 0.75f, 0.43f, -0.51f },
        { 0.43f, 0.75f, -0.51f },
        { -0.01f, 0.86f, -0.51f },
        { -0.44f, 0.75f, -0.51f },
        { -0.76f, 0.43f, -0.51f },
        { -0.87f, 0.00f, -0.51f },
        { -0.76f, -0.44f, -0.51f },
        { -0.44f, -0.76f, -0.51f },
        { -0.01f, -0.87f, -0.51f },
        { 0.43f, -0.76f, -0.51f },
        { 0.75f, -0.44f, -0.51f },
        { 0.51f, 0.00f, -0.87f },
        { 0.44f, 0.25f, -0.87f },
        { 0.25f, 0.44f, -0.87f },
        { -0.01f, 0.51f, -0.87f },
        { -0.26f, 0.44f, -0.87f },
        { -0.45f, 0.25f, -0.87f },
        { -0.51f, 0.00f, -0.87f },
        { -0.45f, -0.26f, -0.87f },
        { -0.26f, -0.45f, -0.87f },
        { -0.01f, -0.51f, -0.87f },
        { 0.25f, -0.45f, -0.87f },
        { 0.44f, -0.26f, -0.87f },
        { 0.00f, 0.00f, -1.00f },
};

void CRageBot::UpdateSettings(CUserCmd* pCmd, CBaseEntity* pLocal) {
    bAimbotEnable = true;
    bAimbotAutoFire = C::Get<bool>(Vars.bRageAutoFire);
    ihitchance = C::Get<int>(Vars.ihitchance);
    imin_damage = C::Get<int>(Vars.imin_damage);
    cmd = pCmd;
    bAimStep = C::Get<bool>(Vars.bAimStep);
    iAimType = C::Get<int>(Vars.iRageAimType);
    iAimScanType = C::Get<int>(Vars.iHitScanMode);
    bPositionAdjustment = C::Get<bool>(Vars.bPositionAdjustment);
    bDoNoRecoil = C::Get<bool>(Vars.bNoRecoil);
    bAutoScope = C::Get<bool>(Vars.bAutoScope);
    bAccuracyAutoStop = C::Get<bool>(Vars.bAccuracyAutoStop);
    bAimbotAutoPistol = C::Get<bool>(Vars.bAimbotAutoPistol);
    iTargetSelection = C::Get<int>(Vars.iTargetSelection);
    bPreferBodyAim = C::Get<bool>(Vars.bPreferBodyAim);

    std::vector<bool> hitboxes = C::Get<std::vector<bool>>(Vars.vecRageTargetHitbox);

    bHitboxHead = hitboxes[0];
    bHitboxChest = hitboxes[1];
    bHitboxStomach = hitboxes[2];
    bHitboxArms = hitboxes[3];
    bHitboxlegs = hitboxes[4];

    flPointScale = C::Get<float>(Vars.flPointScale);
    flHeadPointScale = 0.f;
    flChestPointScale = 0.f;

    iSmart = C::Get<int>(Vars.iSmart);
    bAutoWall = C::Get<bool>(Vars.bRageAutoWall);
    bBestPoint = C::Get<bool>(Vars.bBestPoint);
}

void CRageBot::Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket)
{
	if (!pLocal->IsAlive())
		return;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if (pWeapon == nullptr)
		return;

	short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

	if (pWeaponData == nullptr)
		return;

	static CConVar* weapon_recoil_scale = I::ConVar->FindVar(XorStr("weapon_recoil_scale"));

	if (weapon_recoil_scale == nullptr)
		return;

	if (pWeapon->GetAmmo() == 0 || !pWeapon->IsWeapon() || pWeaponData->nWeaponType == WEAPONTYPE_KNIFE || pWeaponData->nWeaponType == WEAPONTYPE_GRENADE || pWeaponData->nWeaponType == WEAPONTYPE_FISTS || pWeaponData->nWeaponType == WEAPONTYPE_C4)
		return;

	CRageBot::UpdateSettings(pCmd, pLocal);
	CRageBot::Rage(pCmd, pLocal, bSendPacket);
}

void CRageBot::Rage(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket) {

    // Position Adjustment
    if (bPositionAdjustment)
        CRageBot::PositionAdjustment(pCmd, pLocal);

    // Aimbot
    if (bAimbotEnable)
        CRageBot::DoAimbot(pCmd, pLocal, bSendPacket);

    // Recoil
    if (bDoNoRecoil)
        CRageBot::DoNoRecoil(pLocal);

    LastAngle = pCmd->angViewPoint;
}

void CRageBot::AutoRevolver(CUserCmd* pCmd, CBaseEntity* pLocal)
{
    CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

    // Auto Revolver
    if (pWeapon->GetItemDefinitionIndex() == WEAPON_REVOLVER) {

        constexpr auto timeToTicks = [](float time) {  return static_cast<int>(0.5f + time / I::Globals->flIntervalPerTick); };
        constexpr float revolverPrepareTime{ 0.234375f };

        static float readyTime;


        if (!readyTime) readyTime = I::Globals->flCurrentTime + revolverPrepareTime;
        auto ticksToReady = timeToTicks(readyTime - I::Globals->flCurrentTime - I::Engine->GetNetChannelInfo()->GetLatency(0));
        if (ticksToReady > 0 && ticksToReady <= timeToTicks(revolverPrepareTime))
            pCmd->iButtons |= IN_ATTACK;
        else
            readyTime = 0.0f;
    }
}

void CRageBot::DoAimbot(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket)
{
    CBaseEntity* pTarget = nullptr;
    Vector Start = pLocal->GetEyePosition();
    bool FindNewTarget = true;
    //IsLocked = false;

    CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();
    short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
    CCSWeaponData* weapInfo = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

    // Make sure we have a good target
    if (TargetID >= 0 && HitBox >= 0)
    {
        pTarget = I::ClientEntityList->Get<CBaseEntity>(TargetID);
        if (pTarget && CRageBot::TargetMeetsRequirements(pTarget, pLocal))
        {
            HitBox = HitScan(pTarget, pLocal);
            if (HitBox >= 0)
            {
                FindNewTarget = false;
                /*Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
                QAngle View;
                I::Engine->GetViewAngles(View);
                float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
                if (FoV < 180.f)
                    FindNewTarget = false;*/
            }
        }
    }

    // Find a new target, apparently we need to
    if (FindNewTarget)
    {
        TargetID = 0;
        pTarget = nullptr;
        HitBox = -1;

        // Target selection type
        switch (iTargetSelection)
        {
        case 0:
            TargetID = GetTargetCrosshair(pLocal);
            break;
        case 1:
            TargetID = GetTargetDistance(pLocal);
            break;
        case 2:
            TargetID = GetTargetHealth(pLocal);
            break;
        }

        // Memes
        if (TargetID >= 0)
        {
            pTarget = pTarget = I::ClientEntityList->Get<CBaseEntity>(TargetID);
        }
        else
        {
            pTarget = nullptr;
            HitBox = -1;
        }
    }

    Target = pTarget;
    TargetID = TargetID;

    // If we finally have a good target
    if (TargetID >= 0 && pTarget)
    {
        // Get the hitbox to shoot at
        HitBox = HitScan(pTarget, pLocal);

        if (!CanOpenFire(pLocal))
            return;

        float pointscale = flPointScale - 5.f;

        Vector Point;
        Vector AimPoint = pTarget->GetHitboxPosition(HitBox).value() + Vector(0, 0, pointscale);
        

        if (bBestPoint)
        {
            Point = BestPoint(pTarget, AimPoint, pLocal);
        }
        else
        {
            Point = AimPoint;
        }
        
        static float MinimumVelocity = 0.0f;

        CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();
        short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
        CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

        MinimumVelocity = pWeaponData->flMaxSpeedAlt * 0.34f;

        // Stop
        if (TargetID >= 0 && pTarget)
        {
            if (bAccuracyAutoStop && pLocal->GetVelocity().Length() >= MinimumVelocity && pLocal->GetFlags() & FL_ONGROUND)
            {
                Autostop(pCmd, pLocal);
            }
        }

        // Lets open fire
        if (weapInfo->nWeaponType == WEAPONTYPE_SNIPER && !pLocal->IsScoped() && bAutoScope) // Autoscope
        {
            pCmd->iButtons |= IN_ZOOM;
        }
        else
        {
            if ((ihitchance <= CRageBot::HitChance(M::CalcAngle(Start, Point), pTarget, pWeapon, pLocal)))
            {
                if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
                {
                    if (bAimbotAutoFire && !(pCmd->iButtons & IN_ATTACK))
                    {
                        pCmd->iButtons |= IN_ATTACK;
                    }
                    else {
                        return;
                    }
                }
                else if (bAimbotAutoFire && !(pCmd->iButtons & IN_ATTACK)) {
                    pCmd->iButtons |= IN_ATTACK;
                }
                
            }
        }

        if (IsAbleToShoot(pLocal) && pCmd->iButtons & IN_ATTACK)
            Shots += 1;
    }

    // Auto Pistol
    if (weapInfo->nWeaponType == WEAPONTYPE_PISTOL && bAimbotAutoPistol && !pWeapon->GetItemDefinitionIndex() == WEAPON_REVOLVER)
    {
        if (pCmd->iButtons & IN_ATTACK)
        {
            static bool WasFiring = false;
            WasFiring = !WasFiring;

            if (WasFiring)
            {
                pCmd->iButtons &= ~IN_ATTACK;
            }
        }
    }
}

int CRageBot::HitChance(QAngle angles, CBaseEntity* entity, CBaseCombatWeapon* pWeapon, CBaseEntity* pLocal)
{
    int hitseed = 256;

    int iHit = 0;
    int iHitsNeed = (int)((float)hitseed * ((float)ihitchance / 100.f));
    int iHitchance = 0;

    Vector forward, right, up;
    M::AngleVectors(angles, &forward, &right, &up);

    pWeapon->UpdateAccuracyPenalty();

    for (auto i = 0; i < hitseed; ++i) {

        float RandomA = M::RandomFloat(0.0f, 1.0f);
        float RandomB = 1.0f - RandomA * RandomA;
        RandomA = M::RandomFloat(0.0f, M_PI * 2.0f);
        RandomB *= pWeapon->GetSpread() + pWeapon->GetInaccuracy();
        float SpreadX1 = (cos(RandomA) * RandomB);
        float SpreadY1 = (sin(RandomA) * RandomB);
        float RandomC = M::RandomFloat(0.0f, 1.0f);
        float RandomF = RandomF = 1.0f - RandomC * RandomC;
        RandomC = M::RandomFloat(0.0f, M_PI * 2.0f);
        RandomF *= pWeapon->GetSpread();
        float SpreadX2 = (cos(RandomC) * RandomF);
        float SpreadY2 = (sin(RandomC) * RandomF);
        float fSpreadX = SpreadX1 + SpreadX2;
        float fSpreadY = SpreadY1 + SpreadY2;

        Vector vSpreadForward;
        vSpreadForward.x = forward.x + (fSpreadX * right.x) + (fSpreadY * up.x);
        vSpreadForward.y = forward.y + (fSpreadX * right.y) + (fSpreadY * up.y);
        vSpreadForward.z = forward.z + (fSpreadX * right.z) + (fSpreadY * up.z);
        vSpreadForward.NormalizeInPlace();

        QAngle qaNewAngle;
        M::VectorAngles(vSpreadForward, qaNewAngle);
        qaNewAngle.Normalize();

        Vector vEnd;
        M::AngleVectors(qaNewAngle, &vEnd);
        vEnd = pLocal->GetEyePosition() + (vEnd * 8192.f);

        FireBulletData_t bulletData = {};
        float dmg = CAutoWall::Get().GetDamage(pLocal, vEnd, bulletData);

        if (entity->GetHealth() < imin_damage)
            imin_damage = entity->GetHealth();

        if (dmg >= imin_damage)
            iHit++;

        iHitchance = (int)(((float)iHit / 256.f) * 100.f);

        if ((int)(((float)iHit / 256.f) * 100.f) >= ihitchance) {
            break;
        }

        if ((256.f - 1 - i + iHit) < iHitsNeed)
            break;
    }
    return iHitchance;
}

bool CRageBot::IsAbleToShoot(CBaseEntity* pLocal)
{
    CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

    if (!pLocal)
        return false;

    if (!pWeapon)
        return false;

    float flServerTime = pLocal->GetTickBase() * I::Globals->flIntervalPerTick;

    return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

bool CRageBot::CanOpenFire(CBaseEntity* pLocal)
{
    CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

    if (!pWeapon)
        return false;
    
    float flServerTime = (float)pLocal->GetTickBase() * I::Globals->flIntervalPerTick;
    float flNextPrimaryAttack = pWeapon->GetNextPrimaryAttack();

    return !(flNextPrimaryAttack > flServerTime);
}

Vector CRageBot::BestPoint(CBaseEntity* targetPlayer, Vector & final, CBaseEntity* pLocal)
{
    Trace_t tr;

    CTraceFilter filter(targetPlayer);
    Ray_t ray(final + Vector(0, 0, 10), final);

    I::EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

    final = tr.vecEnd;
    return final;
}

bool CRageBot::TargetMeetsRequirements(CBaseEntity* pEntity, CBaseEntity* pLocal)
{
    // Is a valid player
    if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != pLocal->GetIndex())
    {
        PlayerInfo_t pinfo;
        if (I::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
        {
            
            // Team Check
            if (pEntity->GetTeam() != pLocal->GetTeam() && pEntity->IsPlayer() && pEntity->IsAlive())
            {
                // Spawn Check
                if (!pEntity->HasImmunity())
                {
                    return true;
                }
            }
        }
    }

    // They must have failed a requirement
    return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, QAngle View, CBaseEntity* pEntity, int aHitBox)
{
    // Anything past 180 degrees is just going to wrap around
    CONST FLOAT MaxDegrees = 180.0f;

    // Get local angles
    QAngle Angles = View;


    // Get local view / eye position
    Vector Origin = ViewOffSet;

    // Create and intiialize vectors for calculations below
    Vector Delta(0, 0, 0);
    //Vector Origin(0, 0, 0);
    Vector Forward(0, 0, 0);
    M::AngleVectors(Angles, &Forward, NULL, NULL);

    // Convert angles to normalized directional forward vector
    Vector AimPos = pEntity->GetHitboxPosition(aHitBox).value();
    // Get delta vector between our local eye position and passed vector
    Delta = AimPos - Origin;
    //Delta = AimPos - Origin;

    // Normalize our delta vector
    Delta.Normalize();

    // Get dot product between delta position and directional forward vectors
    FLOAT DotProduct = Forward.DotProduct(Delta);

    // Time to calculate the field of view
    return (acos(DotProduct) * (MaxDegrees / M_PI));
}

int CRageBot::GetTargetCrosshair(CBaseEntity* pLocal)
{
    // Target selection
    int target = -1;
    float minFoV = 180.f;

    Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
    QAngle View = *pLocal->GetViewAngles();
    
    for (int i = 0; i < I::ClientEntityList->GetMaxEntities(); i++) //GetHighestEntityIndex()
    {
        CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);
        if (TargetMeetsRequirements(pEntity, pLocal))
        {
            int NewHitBox = HitScan(pEntity, pLocal);
            if (NewHitBox >= 0)
            {
                float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
                if (fov < minFoV)
                {
                    minFoV = fov;
                    target = i;
                }
            }
        }
    }

    return target;
}

int CRageBot::GetTargetDistance(CBaseEntity* pLocal)
{
    // Target selection
    int target = -1;
    float minDist = 99999.f;

    Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
    QAngle View = *pLocal->GetViewAngles();

    for (int i = 0; i < I::ClientEntityList->GetMaxEntities(); i++)
    {
        CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);
        if (TargetMeetsRequirements(pEntity, pLocal))
        {
            int NewHitBox = HitScan(pEntity, pLocal);
            if (NewHitBox >= 0)
            {
                Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
                float Distance = Difference.Length();
                float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
                if (Distance < minDist && fov < 180.f)
                {
                    minDist = Distance;
                    target = i;
                }
            }
        }
    }

    return target;
}

int CRageBot::GetTargetHealth(CBaseEntity* pLocal)
{
    // Target selection
    int target = -1;
    int minHealth = 101;

    Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
    QAngle View = *pLocal->GetViewAngles();

    for (int i = 0; i < I::ClientEntityList->GetMaxEntities(); i++)
    {
        CBaseEntity* pEntity = I::ClientEntityList->Get<CBaseEntity>(i);
        if (TargetMeetsRequirements(pEntity, pLocal))
        {
            int NewHitBox = HitScan(pEntity, pLocal);
            if (NewHitBox >= 0)
            {
                int Health = pEntity->GetHealth();
                float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
                if (Health < minHealth && fov < 180.f)
                {
                    minHealth = Health;
                    target = i;
                }
            }
        }
    }

    return target;
}

int CRageBot::HitScan(CBaseEntity* pEntity, CBaseEntity* pLocal)
{
    std::vector<int> HitBoxesToScan;

    // Get the hitboxes to scan
#pragma region GetHitboxesToScan
    int HitScanMode = iAimScanType;
    bool AWall = bAutoWall;
    bool Multipoint = false;

    if (iSmart > 0 && pLocal->GetShotsFired() + 1 > iSmart)
    {
        if (bPreferBodyAim) {
            HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
            HitBoxesToScan.push_back((int)HITBOX_CHEST);
            HitBoxesToScan.push_back((int)HITBOX_STOMACH);
            HitBoxesToScan.push_back((int)HITBOX_PELVIS);
            HitBoxesToScan.push_back((int)HITBOX_THORAX);
            HitBoxesToScan.push_back((int)HITBOX_NECK);
        }
        else {
            HitBoxesToScan.push_back((int)HITBOX_NECK);
            HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
            HitBoxesToScan.push_back((int)HITBOX_CHEST);
            HitBoxesToScan.push_back((int)HITBOX_STOMACH);
            HitBoxesToScan.push_back((int)HITBOX_PELVIS);
            HitBoxesToScan.push_back((int)HITBOX_THORAX);
        }
        HitBoxesToScan.push_back((int)HITBOX_LEFT_UPPER_ARM);
        HitBoxesToScan.push_back((int)HITBOX_RIGHT_UPPER_ARM);
        HitBoxesToScan.push_back((int)HITBOX_LEFT_THIGH);
        HitBoxesToScan.push_back((int)HITBOX_RIGHT_THIGH);
        HitBoxesToScan.push_back((int)HITBOX_LEFT_HAND);
        HitBoxesToScan.push_back((int)HITBOX_RIGHT_HAND);
        HitBoxesToScan.push_back((int)HITBOX_LEFT_FOOT);
        HitBoxesToScan.push_back((int)HITBOX_RIGHT_FOOT);
        HitBoxesToScan.push_back((int)HITBOX_LEFT_CALF);
        HitBoxesToScan.push_back((int)HITBOX_RIGHT_CALF);
        HitBoxesToScan.push_back((int)HITBOX_LEFT_FOREARM);
        HitBoxesToScan.push_back((int)HITBOX_RIGHT_FOREARM);
    }
    else
    {
        if (HitScanMode == 0)
        {
            if (bPreferBodyAim) {
                if (bHitboxChest) {
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                }

                if (bHitboxStomach) {
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                }
                if (bHitboxHead) {
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                }
            }
            else {
                if (bHitboxHead) {
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                }

                if (bHitboxChest) {
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                }

                if (bHitboxStomach) {
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                }
            }

            if (bHitboxArms) {
                HitBoxesToScan.push_back((int)HITBOX_LEFT_UPPER_ARM);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_UPPER_ARM);

                HitBoxesToScan.push_back((int)HITBOX_LEFT_FOREARM);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_FOREARM);
            }

            if (bHitboxlegs) {
                HitBoxesToScan.push_back((int)HITBOX_LEFT_CALF);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_CALF);
            }
        }
        else
        {
            switch (HitScanMode)
            {
            case 1:
                // Low
                if (bPreferBodyAim) {
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                }
                else {
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                }
                break;
            case 2:
                // Normal
                if (bPreferBodyAim) {
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                }
                else {
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                }
                HitBoxesToScan.push_back((int)HITBOX_LEFT_UPPER_ARM);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_UPPER_ARM);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_CALF);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_CALF);
                break;
            case 3:
                // High
                if (bPreferBodyAim) {
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                }
                else {
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                }
                HitBoxesToScan.push_back((int)HITBOX_LEFT_UPPER_ARM);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_UPPER_ARM);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_THIGH);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_THIGH);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_CALF);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_CALF);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_FOREARM);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_FOREARM);
            case 4:
                // Extreme
                if (bPreferBodyAim) {
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                }
                else {
                    HitBoxesToScan.push_back((int)HITBOX_HEAD);
                    HitBoxesToScan.push_back((int)HITBOX_NECK);
                    HitBoxesToScan.push_back((int)HITBOX_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_STOMACH);
                    HitBoxesToScan.push_back((int)HITBOX_UPPER_CHEST);
                    HitBoxesToScan.push_back((int)HITBOX_PELVIS);
                    HitBoxesToScan.push_back((int)HITBOX_THORAX);
                }
                HitBoxesToScan.push_back((int)HITBOX_LEFT_UPPER_ARM);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_UPPER_ARM);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_THIGH);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_THIGH);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_HAND);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_HAND);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_FOOT);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_FOOT);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_CALF);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_CALF);
                HitBoxesToScan.push_back((int)HITBOX_LEFT_FOREARM);
                HitBoxesToScan.push_back((int)HITBOX_RIGHT_FOREARM);
            }
        }
    }
#pragma endregion Get the list of shit to scan

    // check hits
    // check hits
    int optimalHitbox = -1;
    int current_health = pEntity->GetHealth();
    for (auto HitBoxID : HitBoxesToScan)
    {
        if (AWall)
        {
            Vector Point = pEntity->GetHitboxPosition(HitBoxID).value();
            Color c = Color(255, 255, 255, 255);
            FireBulletData_t data = { };
            float Damage = CAutoWall::Get().GetDamage(pLocal, Point, data);
            if (Damage > 0.f)
            {
                c = Color(0, 255, 0, 255);
                if (Damage >= imin_damage)
                {
                    optimalHitbox = HitBoxID;
                    break;
                } 
                else if (Damage >= current_health) {
                    optimalHitbox = HitBoxID;
                    break;
                }
            }
        }
        else
        {
            Vector Point = pEntity->GetHitboxPosition(HitBoxID).value();
            if (pLocal->IsVisible(pEntity, Point, false)) {
                optimalHitbox = HitBoxID;
                break;
            }
        }
    }

    return optimalHitbox;
}

void CRageBot::DoNoRecoil(CBaseEntity* pLocal)
{
    if (pLocal->IsAlive()) {
        QAngle punch = pLocal->GetPunch();

        if ((punch.x > 0.f && punch.y > 0.f && punch.z > 0.f) && (punch.x < 150.f && punch.y < 150.f && punch.z < 150.f))
            return;

        cmd->angViewPoint -= (punch * 2.f).Normalize();
    }
}

void CRageBot::PositionAdjustment(CUserCmd* pCmd, CBaseEntity* pLocal)
{
    static CConVar* cvar_cl_interp = I::ConVar->FindVar("cl_interp");
    static CConVar* cvar_cl_updaterate = I::ConVar->FindVar("cl_updaterate");
    static CConVar* cvar_sv_maxupdaterate = I::ConVar->FindVar("sv_maxupdaterate");
    static CConVar* cvar_sv_minupdaterate = I::ConVar->FindVar("sv_minupdaterate");
    static CConVar* cvar_cl_interp_ratio = I::ConVar->FindVar("cl_interp_ratio");

    CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();
    if (!pWeapon)
        return;

    float cl_interp = cvar_cl_interp->GetFloat();
    int cl_updaterate = cvar_cl_updaterate->GetInt();
    int sv_maxupdaterate = cvar_sv_maxupdaterate->GetInt();
    int sv_minupdaterate = cvar_sv_minupdaterate->GetInt();
    int cl_interp_ratio = cvar_cl_interp_ratio->GetInt();

    if (sv_maxupdaterate <= cl_updaterate)
        cl_updaterate = sv_maxupdaterate;

    if (sv_minupdaterate > cl_updaterate)
        cl_updaterate = sv_minupdaterate;

    float new_interp = (float)cl_interp_ratio / (float)cl_updaterate;

    if (new_interp > cl_interp)
        cl_interp = new_interp;

    float flSimTime = pLocal->GetSimulationTime();
    float flOldAnimTime = pLocal->GetAnimationTime();

    int iTargetTickDiff = (int)(0.5f + (flSimTime - flOldAnimTime) / I::Globals->flIntervalPerTick);

    int result = (int)((int)(floorf(TIME_TO_TICKS(cl_interp))) + (int)floorf(TIME_TO_TICKS(pLocal->GetSimulationTime())));

    if ((result - pCmd->iTickCount) >= -50)
    {
        pCmd->iTickCount = result;
    }
}

bool CRageBot::AimAtPoint(CBaseEntity* pLocal, Vector point, CUserCmd* pCmd, bool& bSendPacket)
{
    bool ReturnValue = false;
    // Get the full angles
    if (point.Length() == 0) return ReturnValue;

    QAngle angles;
    Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

    angles = M::CalcAngle(src, point).Normalize();

    if (angles.x != angles.x || angles.y != angles.y)
    {
        return ReturnValue;
    }

    IsLocked = true;
    //-----------------------------------------------


    // pSilent Aim 
    QAngle Oldview = pCmd->angViewPoint;
    static int ChokedPackets = -1;
    angles.Normalize();
    switch (iAimType)
    {
    case 0:
        I::Engine->SetViewAngles(angles);
        break;
    case 1:
        pCmd->angViewPoint = angles;
        break;
    case 2:
    {
        static int ChokedPackets = -1;
        ChokedPackets++;

        if (ChokedPackets < 6)
        {
            bSendPacket = false;
            pCmd->angViewPoint = angles;
        }
        else
        {
            bSendPacket = true;
            pCmd->angViewPoint = Oldview;
            ChokedPackets = -1;
            ReturnValue = false;
        }
        break;
    }
    default:
        I::Engine->SetViewAngles(angles);
    }
    return ReturnValue;
}

void CRageBot::Autostop(CUserCmd* cmd, CBaseEntity* pLocal)
{

    if (!pLocal->IsAlive())
        return;

    Vector Velocity = pLocal->GetVelocity();

    if (Velocity.Length() == 0)
        return;

    static float Speed = 450.f;

    QAngle Direction;
    QAngle RealView;
    M::VectorAngles(Velocity, Direction);
    I::Engine->GetViewAngles(RealView);
    Direction.y = RealView.y - Direction.y;

    Vector Forward;
    M::AngleVectors(Direction, &Forward);
    Vector NegativeDirection = Forward * -Speed;

    cmd->flForwardMove = NegativeDirection.x;
    cmd->flSideMove = NegativeDirection.y;
}