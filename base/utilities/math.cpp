#include "math.h"

// used: modules definitons, convar, globals interfaces
#include "../core/interfaces.h"


bool M::Setup()
{
	RandomSeed = reinterpret_cast<RandomSeedFn>(GetProcAddress(GetModuleHandle(VSTDLIB_DLL), XorStr("RandomSeed")));
	if (RandomSeed == nullptr)
		return false;

	RandomFloat = reinterpret_cast<RandomFloatFn>(GetProcAddress(GetModuleHandle(VSTDLIB_DLL), XorStr("RandomFloat")));
	if (RandomFloat == nullptr)
		return false;

	RandomFloatExp = reinterpret_cast<RandomFloatExpFn>(GetProcAddress(GetModuleHandle(VSTDLIB_DLL), XorStr("RandomFloatExp")));
	if (RandomFloatExp == nullptr)
		return false;

	RandomInt = reinterpret_cast<RandomIntFn>(GetProcAddress(GetModuleHandle(VSTDLIB_DLL), XorStr("RandomInt")));
	if (RandomInt == nullptr)
		return false;

	RandomGaussianFloat = reinterpret_cast<RandomGaussianFloatFn>(GetProcAddress(GetModuleHandle(VSTDLIB_DLL), XorStr("RandomGaussianFloat")));
	if (RandomGaussianFloat == nullptr)
		return false;

	return true;
}

void M::VectorAngles(const Vector& vecForward, QAngle& angView)
{
	float flPitch, flYaw;

	if (vecForward.x == 0.f && vecForward.y == 0.f)
	{
		flPitch = (vecForward.z > 0.f) ? 270.f : 90.f;
		flYaw = 0.f;
	}
	else
	{
		flPitch = std::atan2f(-vecForward.z, vecForward.Length2D()) * 180.f / M_PI;

		if (flPitch < 0.f)
			flPitch += 360.f;

		flYaw = std::atan2f(vecForward.y, vecForward.x) * 180.f / M_PI;

		if (flYaw < 0.f)
			flYaw += 360.f;
	}

	angView.x = flPitch;
	angView.y = flYaw;
	angView.z = 0.f;
}

void M::AngleVectors(const QAngle& angView, Vector* pForward, Vector* pRight, Vector* pUp)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(angView.x));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(angView.y));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(angView.z));

	if (pForward != nullptr)
	{
		pForward->x = cp * cy;
		pForward->y = cp * sy;
		pForward->z = -sp;
	}

	if (pRight != nullptr)
	{
		pRight->x = -1 * sr * sp * cy + -1 * cr * -sy;
		pRight->y = -1 * sr * sp * sy + -1 * cr * cy;
		pRight->z = -1 * sr * cp;
	}

	if (pUp != nullptr)
	{
		pUp->x = cr * sp * cy + -sr * -sy;
		pUp->y = cr * sp * sy + -sr * cy;
		pUp->z = cr * cp;
	}
}

void M::AngleMatrix(const QAngle& angView, matrix3x4_t& matrix)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(angView.x));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(angView.y));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(angView.z));

	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	const float crcy = cr * cy;
	const float crsy = cr * sy;
	const float srcy = sr * cy;
	const float srsy = sr * sy;

	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp * crcy + srsy);
	matrix[1][2] = (sp * crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.f;
	matrix[1][3] = 0.f;
	matrix[2][3] = 0.f;
}

Vector2D M::AnglePixels(const float flSensitivity, const float flPitch, const float flYaw, const QAngle& angBegin, const QAngle& angEnd)
{
	QAngle angDelta = angBegin - angEnd;
	angDelta.Normalize();

	const float flPixelMovePitch = (-angDelta.x) / (flYaw * flSensitivity);
	const float flPixelMoveYaw = (angDelta.y) / (flPitch * flSensitivity);

	return Vector2D(flPixelMoveYaw, flPixelMovePitch);
}

QAngle M::PixelsAngle(const float flSensitivity, const float flPitch, const float flYaw, const Vector2D& vecPixels)
{
	const float flAngleMovePitch = (-vecPixels.x) * (flYaw * flSensitivity);
	const float flAngleMoveYaw = (vecPixels.y) * (flPitch * flSensitivity);

	return QAngle(flAngleMoveYaw, flAngleMovePitch, 0.f);
}

QAngle M::CalcAngle(const Vector& vecStart, const Vector& vecEnd)
{
	QAngle angView;
	const Vector vecDelta = vecEnd - vecStart;
	VectorAngles(vecDelta, angView);
	angView.Normalize();

	return angView;
}

QAngle M::CalcAngle(const Vector& source, const Vector& destination, const QAngle& viewAngles) {
	Vector delta = source - destination;
	auto radians_to_degrees = [](float radians) { return radians * 180 / static_cast<float>(M_PI); };
	QAngle angles;
	angles.x = radians_to_degrees(atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewAngles.x;
	angles.y = radians_to_degrees(atanf(delta.y / delta.x)) - viewAngles.y;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

	angles.Normalize();
	return angles;
}

Vector M::VectorTransform(const Vector& vecTransform, const matrix3x4_t& matrix)
{
	return Vector(vecTransform.DotProduct(matrix[0]) + matrix[0][3],
		vecTransform.DotProduct(matrix[1]) + matrix[1][3],
		vecTransform.DotProduct(matrix[2]) + matrix[2][3]);
}

__forceinline float DotProduct1(const float* a, const float* b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

__forceinline double degtorad(const double degree)
{
	double pi = 3.14159265359;
	return (degree * (pi / 180));
}

Vector M::VectorRotate(const Vector& vecRotate, const matrix3x4_t& matrix)
{
	return Vector(
		matrix[0][0] * vecRotate.x + matrix[1][0] * vecRotate.y + matrix[2][0] * vecRotate.z,
		matrix[0][1] * vecRotate.x + matrix[1][1] * vecRotate.y + matrix[2][1] * vecRotate.z,
		matrix[0][2] * vecRotate.x + matrix[1][2] * vecRotate.y + matrix[2][2] * vecRotate.z
	);
}

void M::VectorRotate(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorRotate(&in1.x, in2, &out.x);
}

void M::VectorRotate(const Vector& in1, const QAngle& in2, Vector& out)
{
	matrix3x4_t matRotate;
	AngleMatrix2(in2, matRotate);
	VectorRotate(in1, matRotate, out);
}

void M::VectorRotate(const float* in1, const matrix3x4_t& in2, float* out)
{
	out[0] = DotProduct1(in1, in2[0]);
	out[1] = DotProduct1(in1, in2[1]);
	out[2] = DotProduct1(in1, in2[2]);
}

void M::AngleMatrix2(const QAngle angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	sy = sin(degtorad(angles.y));
	cy = cos(degtorad(angles.y));

	sp = sin(degtorad(angles.x));
	cp = cos(degtorad(angles.x));

	sr = sin(degtorad(angles.z));
	cr = cos(degtorad(angles.z));

	//matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;

	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp * crcy + srsy);
	matrix[1][2] = (sp * crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

void M::VectorMatrix(const Vector& vecMatrix, matrix3x4_t& matrix)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(vecMatrix.x));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(vecMatrix.y));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(vecMatrix.z));

	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	const float crcy = cr * cy;
	const float crsy = cr * sy;
	const float srcy = sr * cy;
	const float srsy = sr * sy;

	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp * crcy + srsy);
	matrix[1][2] = (sp * crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.f;
	matrix[1][3] = 0.f;
	matrix[2][3] = 0.f;
}

Vector M::ExtrapolateTick(const Vector& p0, const Vector& v0)
{
	// position formula: p0 + v0t
	return p0 + (v0 * I::Globals->flIntervalPerTick);
}

float M::Deg2rad(float angle) {
	return (float)((M_PI / 180) * angle);
}

//void M::RotateCenter(const ImVec2& vecCenter, const float flAngle, ImVec2* pOutPoint)
//{
//	const float flSin = std::sinf(M_DEG2RAD(flAngle));
//	const float flCos = std::cosf(M_DEG2RAD(flAngle));
//
//	pOutPoint->x -= vecCenter.x;
//	pOutPoint->y -= vecCenter.y;
//
//	const float x = pOutPoint->x * flCos - pOutPoint->y * flSin;
//	const float y = pOutPoint->x * flSin + pOutPoint->y * flCos;
//
//	pOutPoint->x = x + vecCenter.x;
//	pOutPoint->y = y + vecCenter.y;
//}
float M::Rad2deg(float angle) {
	return (float)((180 / M_PI) * angle);
}
