#include "Quaternion.h"

CU_NS_BEGIN

const Quaternion Quaternion::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

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

Quaternion::Quaternion()
	:x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}

Quaternion::Quaternion(float* arrays)
{
	x = arrays[0];
	y = arrays[1];
	z = arrays[2];
	w = arrays[3];
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
	return Zero;
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(-x, -y, -z, w);
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