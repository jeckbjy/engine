#pragma once
#include "CMath.h"
#include "Vector3.h"

CU_NS_BEGIN

/** A 3x3 matrix. Can be used for non-homogenous transformations of three dimensional vectors and points. */
class Quaternion;
class CU_API Matrix3
{
public:
	static const Matrix3 ZERO;
	static const Matrix3 IDENTITY;

	Matrix3();
	Matrix3(float v);
	Matrix3(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);
	Matrix3(const Quaternion& rotation) { fromQuaternion(rotation); }
	Matrix3(const Quaternion& rotation, const Vector3& scale);
	Matrix3(const Vector3& axis, float angle){ fromAxisAngle(axis, angle); }
	Matrix3(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) { fromAxes(xAxis, yAxis, zAxis); }
	Matrix3(float xAngle, float yAngle, float zAngle) { fromEulerAngles(xAngle, yAngle, zAngle); }
	Matrix3(float xAngle, float yAngle, float zAngle, EulerAngleOrder order) { fromEulerAngles(xAngle, yAngle, zAngle, order); }

	void swap(Matrix3& other);

	Vector3 getColumn(size_t col) const;
	void setColumn(size_t col, const Vector3& vec);

	float determinant() const;
	// Gram - Schmidt orthonormalization(applied to columns of rotation matrix)
	void orthonormalize();

	Matrix3 scaled(const Vector3& scale) const;
	Matrix3 transpose() const;
	Vector3 transform(const Vector3& vec) const;
	Matrix3 inverse(float tolerance = 1e-06f) const;
	bool inverse(Matrix3& mat, float tolerance = 1e-06f) const;

	void decomposition(Quaternion& rotation, Vector3& scale) const;
	void QDUDecomposition(Matrix3& matQ, Vector3& vecD, Vector3& vecU) const;
	void singularValueDecomposition(Matrix3& matL, Vector3& matS, Matrix3& matR) const;
	/**
	* Eigensolver, matrix must be symmetric.
	*
	* @note
	* Eigenvectors are vectors which when transformed by the matrix, only change in magnitude, but not in direction.
	* Eigenvalue is that magnitude. In other words you will get the same result whether you multiply the vector by the
	* matrix or by its eigenvalue.
	*/
	void eigenSolveSymmetric(float eigenValues[3], Vector3 eigenVectors[3]) const;

	void fromAxisAngle(const Vector3& axis, float angle);
	void fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
	void fromQuaternion(const Quaternion& quat);
	void fromEulerAngles(float xAngle, float yAngle, float zAngle);
	void fromEulerAngles(float xAngle, float yAngle, float zAngle, EulerAngleOrder order);

	void toQuaternion(Quaternion& quat) const;
	bool toEulerAngles(float& xAngle, float& yAngle, float& zAngle) const;

	float* operator[](size_t row) const { return (float*)m[row]; }

	Matrix3 operator-() const;

	Matrix3& operator+=(const Matrix3& rhs);
	Matrix3& operator-=(const Matrix3& rhs);
	Matrix3& operator*=(const Matrix3& rhs);

	Matrix3& operator+=(float rhs);
	Matrix3& operator-=(float rhs);
	Matrix3& operator*=(float rhs);
	Matrix3& operator/=(float rhs);

	friend Matrix3 operator+(Matrix3 lhs, const Matrix3& rhs) { return lhs += rhs; }
	friend Matrix3 operator-(Matrix3 lhs, const Matrix3& rhs) { return lhs -= rhs; }
	friend Matrix3 operator*(Matrix3 lhs, const Matrix3& rhs) { return lhs -= rhs; }
	
	friend Matrix3 operator+(Matrix3 lhs, const float& rhs) { return lhs += rhs; }
	friend Matrix3 operator-(Matrix3 lhs, const float& rhs) { return lhs -= rhs; }
	friend Matrix3 operator*(Matrix3 lhs, const float& rhs) { return lhs *= rhs; }
	friend Matrix3 operator/(Matrix3 lhs, const float& rhs) { return lhs /= rhs; }

	friend Matrix3 operator*(float lhs, const Matrix3& rhs) { return rhs * lhs; }

	friend bool operator==(const Matrix3& lhs, const Matrix3& rhs) { return memcmp(&lhs, &rhs, sizeof(lhs)) == 0; }
	friend bool operator!=(const Matrix3& lhs, const Matrix3& rhs) { return memcmp(&lhs, &rhs, sizeof(lhs)) != 0; }

protected:
	// Support for eigensolver
	void tridiagonal(float diag[3], float subDiag[3]);
	bool QLAlgorithm(float diag[3], float subDiag[3]);

	// Support for singular value decomposition
	static void bidiagonalize(Matrix3& matA, Matrix3& matL, Matrix3& matR);
	static void golubKahanStep(Matrix3& matA, Matrix3& matL, Matrix3& matR);

public:
	union
	{
		float m[3][3];
		float _m[9];
		struct  
		{
#ifdef COL_MAJOR
			float m00, m10, m20;
			float m01, m11, m21;
			float m02, m12, m22;
#else
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
#endif
		};
	};
};

CU_NS_END
