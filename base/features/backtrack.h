#pragma once
// used: winapi includes, singleton
#include "../common.h"
// used: usercmd
#include "../sdk/datatypes/usercmd.h"
// used: baseentity
#include "../sdk/entity.h"

#include <deque>

class CBacktrack : public CSingleton<CBacktrack>
{
	struct matrix_t {
		matrix_t() = default;

		matrix_t(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23) {
			mat_val[0][0] = m00;
			mat_val[0][1] = m01;
			mat_val[0][2] = m02;
			mat_val[0][3] = m03;
			mat_val[1][0] = m10;
			mat_val[1][1] = m11;
			mat_val[1][2] = m12;
			mat_val[1][3] = m13;
			mat_val[2][0] = m20;
			mat_val[2][1] = m21;
			mat_val[2][2] = m22;
			mat_val[2][3] = m23;
		}


		//-----------------------------------------------------------------------------
		// Creates a matrix where the X axis = forward
		// the Y axis = left, and the Z axis = up
		//-----------------------------------------------------------------------------
		void init(const Vector& x_axis, const Vector& y_axis, const Vector& z_axis, const Vector& vec_origin) {
			mat_val[0][0] = x_axis.x;
			mat_val[0][1] = y_axis.x;
			mat_val[0][2] = z_axis.x;
			mat_val[0][3] = vec_origin.x;
			mat_val[1][0] = x_axis.y;
			mat_val[1][1] = y_axis.y;
			mat_val[1][2] = z_axis.y;
			mat_val[1][3] = vec_origin.y;
			mat_val[2][0] = x_axis.z;
			mat_val[2][1] = y_axis.z;
			mat_val[2][2] = z_axis.z;
			mat_val[2][3] = vec_origin.z;
		}

		//-----------------------------------------------------------------------------
		// Creates a matrix where the X axis = forward
		// the Y axis = left, and the Z axis = up
		//-----------------------------------------------------------------------------
		matrix_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin) {
			init(xAxis, yAxis, zAxis, vecOrigin);
		}

		inline void set_origin(Vector const& p) {
			mat_val[0][3] = p.x;
			mat_val[1][3] = p.y;
			mat_val[2][3] = p.z;
		}

		inline void invalidate(void) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 4; j++) {
					mat_val[i][j] = VEC_T_NAN;
				}
			}
		}

		float* operator[](int i) {
			return mat_val[i];
		}

		const float* operator[](int i) const {
			return mat_val[i];
		}

		float* base() {
			return &mat_val[0][0];
		}

		const float* base() const {
			return &mat_val[0][0];
		}

		float mat_val[3][4];
	};

	struct stored_records {
		Vector head;
		float simulation_time;
		matrix3x4_t matrix[128];
	};

public:
	// Get
	void Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);
	void CBacktrack::update(CBaseEntity* pLocal);
private:
	float CBacktrack::get_lerp_time();
	int CBacktrack::time_to_ticks(float time);
	bool CBacktrack::valid_tick(float simtime);
	std::deque<stored_records> records[65];
};
