#pragma once
// used: winapi includes, singleton
#include "../common.h"
// used: usercmd
#include "../sdk/datatypes/usercmd.h"
// used: baseentity
#include "../sdk/entity.h"

#include <deque>

#define PI 3.14159265358979323846f
#define MAXBACKTRACKTICKS ((int)C::Get<float>(Vars.flBacktrack))

inline Vector angle_vector(QAngle meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f * static_cast<float>(PI));

	return Vector(cp * cy, cp * sy, -sp);
}
inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.DotProduct(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}


struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
};

struct backtrackData
{
	float simtime;
	Vector hitboxPos;
};

extern backtrackData headPositions[64][25];

class CBacktrack : public CSingleton<CBacktrack>
{

	struct stored_records {
		Vector head;
		float simulation_time;
		matrix3x4_t matrix[128];
	};
	int latest_tick;
public:
	// Get
	void Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);
	void update(CBaseEntity* pLocal);
	// lbyRecords records[64];
private:
	void legitBackTrack(CUserCmd* cmd, CBaseEntity* pLocal);
	int BacktrackTicks();
	float CBacktrack::get_lerp_time();
	int CBacktrack::time_to_ticks(float time);
	bool CBacktrack::valid_tick(float simtime);
	std::deque<stored_records> records[65];
};
