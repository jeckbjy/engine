#include "Quaternion.h"
#include "Matrix3.h"

CU_NS_BEGIN

struct EulerAngleOrderData
{
	int a, b, c;
};

const Quaternion Quaternion::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::IDENTITY(0.0f, 0.0f, 0.0f, 1.0f);

static const EulerAngleOrderData EA_LOOKUP[6] = {
	{ 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
	{ 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } 
};

void Quaternion::lerp(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, float t)
{
	assert(dst);
	assert(t >= 0.0f && t <= 1.0f);
	if (t == 0.0f)
	{
		*dst = q1;
	}
	else if (t == 1.0f)
	{
		*dst = q2;
	}
	else
	{
		float t1 = 1.0f - t;

		dst->x = t1 * q1.x + t * q2.x;
		dst->y = t1 * q1.y + t * q2.y;
		dst->z = t1 * q1.z + t * q2.z;
		dst->w = t1 * q1.w + t * q2.w;
	}
}

void Quaternion::slerp(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, float t)
{
	assert(dst);
	assert(t >= 0.0f && t <= 1.0f);
	if (t == 0.0f)
	{
		*dst = q1;
	}
	else if (t == 1.0f)
	{
		*dst = q2;
	}
	else if (q1 == q2)
	{
		*dst = q1;
	}
	else
	{
		float halfY, alpha, beta;
		float u, f1, f2a, f2b;
		float ratio1, ratio2;
		float halfSecHalfTheta, versHalfTheta;
		float sqNotU, sqU;

		float cosTheta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

		// As usual in all slerp implementations, we fold theta.
		alpha = cosTheta >= 0 ? 1.0f : -1.0f;
		halfY = 1.0f + alpha * cosTheta;

		// Here we bisect the interval, so we need to fold t as well.
		f2b = t - 0.5f;
		u = f2b >= 0 ? f2b : -f2b;
		f2a = u - f2b;
		f2b += u;
		u += u;
		f1 = 1.0f - u;

		// One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
		halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
		halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
		versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

		// Evaluate series expansions of the coefficients.
		sqNotU = f1 * f1;
		ratio2 = 0.0000440917108f * versHalfTheta;
		ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
		ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
		ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
		ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

		sqU = u * u;
		ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
		ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
		ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
		ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

		// Perform the bisection and resolve the folding done earlier.
		f1 *= ratio1 * halfSecHalfTheta;
		f2a *= ratio2;
		f2b *= ratio2;
		alpha *= f1 + f2a;
		beta = f1 + f2b;

		// Apply final coefficients to a and b as usual.
		float w = alpha * q1.w + beta * q2.w;
		float x = alpha * q1.x + beta * q2.x;
		float y = alpha * q1.y + beta * q2.y;
		float z = alpha * q1.z + beta * q2.z;

		// This final adjustment to the quaternion's length corrects for
		// any small constraint error in the inputs q1 and q2 But as you
		// can see, it comes at the cost of 9 additional multiplication
		// operations. If this error-correcting feature is not required,
		// the following code may be removed.
		f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
		*dst *= f1;
	}
}

static void slerp_squad(Quaternion* dst,const Quaternion& q1, const Quaternion& q2, float t)
{
	// cos(omega) = q1 * q2;
	// slerp(q1, q2, t) = (q1*sin((1-t)*omega) + q2*sin(t*omega))/sin(omega);
	// q1 = +- q2, slerp(q1,q2,t) = q1.
	// This is a straight-forward implementation of the formula of slerp. It does not do any sign switching.
	float c = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	if (fabs(c) >= 1.0f)
	{
		dst->x = q1.x;
		dst->y = q1.y;
		dst->z = q1.z;
		dst->w = q1.w;
		return;
	}

	float omega = acos(c);
	float s = sqrt(1.0f - c * c);
	if (fabs(s) <= 0.00001f)
	{
		dst->x = q1.x;
		dst->y = q1.y;
		dst->z = q1.z;
		dst->w = q1.w;
		return;
	}

	float r1 = sin((1 - t) * omega) / s;
	float r2 = sin(t * omega) / s;
	dst->x = (q1.x * r1 + q2.x * r2);
	dst->y = (q1.y * r1 + q2.y * r2);
	dst->z = (q1.z * r1 + q2.z * r2);
	dst->w = (q1.w * r1 + q2.w * r2);
}

void Quaternion::squad(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t)
{
	assert(dst);
	assert(t >= 0.0f && t <= 1.0f);
	Quaternion dstQ(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion dstS(0.0f, 0.0f, 0.0f, 1.0f);
	slerp_squad(&dstQ, q1, q2, t);
	slerp_squad(&dstS, s1, s2, t);
	slerp_squad(dst, dstQ, dstS, 2.0f * t * (1.0f - t));
}

Quaternion Quaternion::getRotationFromTo(const Vector3& from, const Vector3& dest, const Vector3& fallbackAxis /* = Vector3::ZERO */)
{
	// Based on Stan Melax's article in Game Programming Gems
	Quaternion q;

	Vector3 v0 = from;
	Vector3 v1 = dest;
	v0.normalize();
	v1.normalize();

	float d = Vector3::dot(v0, v1);

	// If dot == 1, vectors are the same
	if (d >= 1.0f)
		return Quaternion::IDENTITY;

	if (d < (1e-6f - 1.0f))
	{
		if (fallbackAxis != Vector3::ZERO)
		{
			// Rotate 180 degrees about the fallback axis
			q.fromAxisAngle(fallbackAxis, Math::PI);
		}
		else
		{
			// Generate an axis
			Vector3 axis = Vector3::cross(Vector3::UNIT_X, from);
			if (axis.isZeroLength()) // Pick another if colinear
				axis = Vector3::cross(Vector3::UNIT_Y, from);
			axis.normalize();
			q.fromAxisAngle(axis, Math::PI);
		}
	}
	else
	{
		float s = Math::sqrt((1 + d) * 2);
		float invs = 1 / s;

		Vector3 c = Vector3::cross(v0, v1);

		q.x = c.x * invs;
		q.y = c.y * invs;
		q.z = c.z * invs;
		q.w = s * 0.5f;
		q.normalize();
	}

	return q;
}

Quaternion::Quaternion()
	: w(1.0f), x(0.0f), y(0.0f), z(0.0f)
{
}

Quaternion::Quaternion(float w, float x, float y, float z)
	: w(w), x(x), y(y), z(z)
{
}

Quaternion Quaternion::inverse() const
{
	float norm = w*w + x*x + y*y + z*z;
	if (norm > 0.0f)
	{
		float inv = 1.0f / norm;
		return Quaternion(w*inv, -x*inv, -y*inv, -z*inv);
	}

	// Return an invalid result to flag the error
	return ZERO;
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(w, - x, -y, -z);
}

float Quaternion::normalize()
{
	//float factor = Math::invsqrt(len);
	float len = w*w + x*x + y*y + z*z;
	len = sqrt(len);
	if (len < 0.000001f)
		return 0;
	(*this) *= (1 / len);
	return len;
}

float Quaternion::dot(const Quaternion& other) const
{
	return w*other.w + x*other.x + y*other.y + z*other.z;
}

void Quaternion::multiply(const Quaternion& q2)
{
	const Quaternion& q1 = *this;
	float rx = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	float ry = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	float rz = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	float rw = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

	this->x = rx;
	this->y = ry;
	this->z = rz;
	this->w = rw;
}

Vector3 Quaternion::rotate(const Vector3& v) const
{
	Matrix3 rot;
	toRotationMatrix(rot);
	return rot.transform(v);
}

void Quaternion::lookRotation(const Vector3& forwardDir)
{
	if (forwardDir == Vector3::ZERO)
		return;

	Vector3 nrmForwardDir = Vector3::normalize(forwardDir);
	Vector3 currentForwardDir = -zAxis();

	Quaternion targetRotation;
	if ((nrmForwardDir + currentForwardDir).squared() < 0.00005f)
	{
		// Oops, a 180 degree turn (infinite possible rotation axes)
		// Default to yaw i.e. use current UP
		*this = Quaternion(-y, -z, w, x);
	}
	else
	{
		// Derive shortest arc to new direction
		Quaternion rotQuat = getRotationFromTo(currentForwardDir, nrmForwardDir);
		*this = rotQuat * *this;
	}
}

void Quaternion::lookRotation(const Vector3& forwardDir, const Vector3& upDir)
{
	Vector3 forward = Vector3::normalize(forwardDir);
	Vector3 up = Vector3::normalize(upDir);

	if (Math::equals(Vector3::dot(forward, up), 1.0f))
	{
		lookRotation(forward);
		return;
	}

	Vector3 x = Vector3::cross(forward, up);
	Vector3 y = Vector3::cross(x, forward);

	x.normalize();
	y.normalize();

	*this = Quaternion(x, y, -forward);
}

Vector3 Quaternion::xAxis() const
{
	float fTy = 2.0f*y;
	float fTz = 2.0f*z;
	float fTwy = fTy*w;
	float fTwz = fTz*w;
	float fTxy = fTy*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTzz = fTz*z;

	return Vector3(1.0f - (fTyy + fTzz), fTxy + fTwz, fTxz - fTwy);
}

Vector3 Quaternion::yAxis() const
{
	float fTx = 2.0f*x;
	float fTy = 2.0f*y;
	float fTz = 2.0f*z;
	float fTwx = fTx*w;
	float fTwz = fTz*w;
	float fTxx = fTx*x;
	float fTxy = fTy*x;
	float fTyz = fTz*y;
	float fTzz = fTz*z;

	return Vector3(fTxy - fTwz, 1.0f - (fTxx + fTzz), fTyz + fTwx);
}

Vector3 Quaternion::zAxis() const
{
	float fTx = 2.0f*x;
	float fTy = 2.0f*y;
	float fTz = 2.0f*z;
	float fTwx = fTx*w;
	float fTwy = fTy*w;
	float fTxx = fTx*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTyz = fTz*y;

	return Vector3(fTxz + fTwy, fTyz - fTwx, 1.0f - (fTxx + fTyy));
}

void Quaternion::fromRotationMatrix(const Matrix3& mat)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	float trace = mat[0][0] + mat[1][1] + mat[2][2];
	float root;

	if (trace > 0.0f)
	{
		// |w| > 1/2, may as well choose w > 1/2
		root = Math::sqrt(trace + 1.0f);  // 2w
		w = 0.5f*root;
		root = 0.5f / root;  // 1/(4w)
		x = (mat[2][1] - mat[1][2])*root;
		y = (mat[0][2] - mat[2][0])*root;
		z = (mat[1][0] - mat[0][1])*root;
	}
	else
	{
		// |w| <= 1/2
		static uint32 nextLookup[3] = { 1, 2, 0 };
		uint32 i = 0;

		if (mat[1][1] > mat[0][0])
			i = 1;

		if (mat[2][2] > mat[i][i])
			i = 2;

		uint32 j = nextLookup[i];
		uint32 k = nextLookup[j];

		root = Math::sqrt(mat[i][i] - mat[j][j] - mat[k][k] + 1.0f);

		float* cmpntLookup[3] = { &x, &y, &z };
		*cmpntLookup[i] = 0.5f*root;
		root = 0.5f / root;

		w = (mat[k][j] - mat[j][k])*root;
		*cmpntLookup[j] = (mat[j][i] + mat[i][j])*root;
		*cmpntLookup[k] = (mat[k][i] + mat[i][k])*root;
	}

	normalize();
}

void Quaternion::fromAxisAngle(const Vector3& axis, float angle)
{
	float halfAngle(0.5f*angle);
	float sin = Math::sin(halfAngle);

	w = Math::cos(halfAngle);
	x = sin*axis.x;
	y = sin*axis.y;
	z = sin*axis.z;
}

void Quaternion::fromAxes(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
{
	Matrix3 kRot;

	kRot[0][0] = xaxis.x;
	kRot[1][0] = xaxis.y;
	kRot[2][0] = xaxis.z;

	kRot[0][1] = yaxis.x;
	kRot[1][1] = yaxis.y;
	kRot[2][1] = yaxis.z;

	kRot[0][2] = zaxis.x;
	kRot[1][2] = zaxis.y;
	kRot[2][2] = zaxis.z;

	fromRotationMatrix(kRot);
}

void Quaternion::fromEulerAngles(float xAngle, float yAngle, float zAngle)
{
	float halfXAngle = xAngle * 0.5f;
	float halfYAngle = yAngle * 0.5f;
	float halfZAngle = zAngle * 0.5f;

	float cx = Math::cos(halfXAngle);
	float sx = Math::sin(halfXAngle);

	float cy = Math::cos(halfYAngle);
	float sy = Math::sin(halfYAngle);

	float cz = Math::cos(halfZAngle);
	float sz = Math::sin(halfZAngle);

	Quaternion quatX(cx, sx, 0.0f, 0.0f);
	Quaternion quatY(cy, 0.0f, sy, 0.0f);
	Quaternion quatZ(cz, 0.0f, 0.0f, sz);

	*this = (quatY * quatX) * quatZ;
}

void Quaternion::fromEulerAngles(float xAngle, float yAngle, float zAngle, EulerAngleOrder order)
{
	const EulerAngleOrderData& l = EA_LOOKUP[(int)order];

	float halfXAngle = xAngle * 0.5f;
	float halfYAngle = yAngle * 0.5f;
	float halfZAngle = zAngle * 0.5f;

	float cx = Math::cos(halfXAngle);
	float sx = Math::sin(halfXAngle);

	float cy = Math::cos(halfYAngle);
	float sy = Math::sin(halfYAngle);

	float cz = Math::cos(halfZAngle);
	float sz = Math::sin(halfZAngle);

	Quaternion quats[3];
	quats[0] = Quaternion(cx, sx, 0.0f, 0.0f);
	quats[1] = Quaternion(cy, 0.0f, sy, 0.0f);
	quats[2] = Quaternion(cz, 0.0f, 0.0f, sz);

	*this = (quats[l.a] * quats[l.b]) * quats[l.c];
}

void Quaternion::toRotationMatrix(Matrix3& mat) const
{
	float tx = x + x;
	float ty = y + y;
	float fTz = z + z;
	float twx = tx*w;
	float twy = ty*w;
	float twz = fTz*w;
	float txx = tx*x;
	float txy = ty*x;
	float txz = fTz*x;
	float tyy = ty*y;
	float tyz = fTz*y;
	float tzz = fTz*z;

	mat[0][0] = 1.0f - (tyy + tzz);
	mat[0][1] = txy - twz;
	mat[0][2] = txz + twy;
	mat[1][0] = txy + twz;
	mat[1][1] = 1.0f - (txx + tzz);
	mat[1][2] = tyz - twx;
	mat[2][0] = txz - twy;
	mat[2][1] = tyz + twx;
	mat[2][2] = 1.0f - (txx + tyy);
}

void Quaternion::toAxisAngle(Vector3& axis, float& angle) const
{
	float sqrLength = x*x + y*y + z*z;
	if (sqrLength > 0.0)
	{
		angle = 2.0*Math::acos(w);
		float invLength = Math::invsqrt(sqrLength);
		axis.x = x*invLength;
		axis.y = y*invLength;
		axis.z = z*invLength;
	}
	else
	{
		// Angle is 0 (mod 2*pi), so any axis will do
		angle = 0.0;
		axis.x = 1.0;
		axis.y = 0.0;
		axis.z = 0.0;
	}
}

void Quaternion::toAxes(Vector3& xaxis, Vector3& yaxis, Vector3& zaxis) const
{
	Matrix3 matRot;
	toRotationMatrix(matRot);

	xaxis.x = matRot[0][0];
	xaxis.y = matRot[1][0];
	xaxis.z = matRot[2][0];

	yaxis.x = matRot[0][1];
	yaxis.y = matRot[1][1];
	yaxis.z = matRot[2][1];

	zaxis.x = matRot[0][2];
	zaxis.y = matRot[1][2];
	zaxis.z = matRot[2][2];
}

bool Quaternion::toEulerAngles(float& xAngle, float& yAngle, float& zAngle) const
{
	Matrix3 matRot;
	toRotationMatrix(matRot);
	return matRot.toEulerAngles(xAngle, yAngle, zAngle);
}

String Quaternion::toString() const
{
	char buf[128];
	snprintf(buf, sizeof(buf), "%g %g %g %g", w, x, y, z);
	return String(buf);
}

Quaternion& Quaternion::operator +=(const Quaternion& rhs)
{
	w += rhs.w;
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Quaternion& Quaternion::operator -=(const Quaternion& rhs)
{
	w -= rhs.w;
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& rhs)
{
	multiply(rhs);
	return *this;
}

Quaternion& Quaternion::operator *=(const float& rhs)
{
	w *= rhs;
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

CU_NS_END