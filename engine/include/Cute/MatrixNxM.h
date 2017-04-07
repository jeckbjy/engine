//! Math
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/**
* Class representing a NxM matrix.
*
* @note	If you need to use matrices for more than just data storage then
*			it is suggested you use specialized Matrix3 or Matrix4 classes
*			as they provide a wide range of functionality.
*/
template<int N, int M>
class MatrixNxM
{
public:
	enum { ARRAY_SIZE = N * M };

	MatrixNxM() {}
	MatrixNxM(float v);
	MatrixNxM(float data[N*M]);

	/** Returns a transpose of the matrix (switched columns and rows). */
	MatrixNxM<N, M> transpose() const;

	/** Returns a row of the matrix. */
	float* operator[] (size_t row) const;

	MatrixNxM<N, M>& operator= (const MatrixNxM<N, M>& rhs);

	bool operator== (const MatrixNxM<N, M>& rhs) const;
	bool operator!= (const MatrixNxM<N, M>& rhs) const;

	float m[N][M];
};

typedef MatrixNxM<2, 2> Matrix2;
typedef MatrixNxM<2, 3> Matrix2x3;
typedef MatrixNxM<2, 4> Matrix2x4;
typedef MatrixNxM<3, 2> Matrix3x2;
//typedef MatrixNxM<3, 4> Matrix3x4;
typedef MatrixNxM<4, 2> Matrix4x2;
typedef MatrixNxM<4, 3> Matrix4x3;

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
template<int N, int M>
MatrixNxM<N, M>::MatrixNxM(float v)
{
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		m[i] = v;
	}
}

template<int N, int M>
MatrixNxM<N, M>::MatrixNxM(float data[N*M])
{
	memcpy(m, data, N*M * sizeof(float));
}

template<int N, int M>
MatrixNxM<N, M> MatrixNxM<N,M>::transpose() const
{
	MatrixNxM<N, M> matTranspose;
	for (int row = 0; row < N; row++)
	{
		for (int col = 0; col < M; col++)
			matTranspose[row][col] = m[col][row];
	}

	return matTranspose;
}

template<int N, int M>
float* MatrixNxM<N, M>::operator[] (size_t row) const
{
	assert(row < N);

	return (float*)m[row];
}

template<int N, int M>
MatrixNxM<N, M>& MatrixNxM<N, M>::operator= (const MatrixNxM<N, M>& rhs)
{
	memcpy(m, rhs.m, (N*M)*sizeof(float));
	return *this;
}

template<int N, int M>
bool MatrixNxM<N, M>::operator== (const MatrixNxM<N, M>& rhs) const
{
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		if (m[i] != rhs.m[i])
			return false;
	}

	return true;
}

template<int N, int M>
bool MatrixNxM<N, M>::operator!= (const MatrixNxM<N, M>& rhs) const
{
	return !operator==(rhs);
}

CUTE_NS_END
