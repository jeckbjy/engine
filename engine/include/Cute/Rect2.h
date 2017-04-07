//! Math
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector2.h"

CUTE_NS_BEGIN

template<typename T>
class TRect2
{
public:
	typedef std::vector<TRect2> RectVector;

	TRect2();
	TRect2(T x, T y, T width, T height);

	bool contains(const TVector2<T>& point) const;
	bool overlaps(const TRect2& other) const;
	void encapsulate(const TRect2& other);
	void clip(const TRect2& other);
	
	void cut(const TRect2& cutRect, std::vector<TRect2>& pieces);
	void cut(const std::vector<TRect2>& cutRects, std::vector<TRect2>& pieces);

	void transform(const Matrix4& matrix);

	bool operator== (const TRect2& rhs) const;
	bool operator!= (const TRect2& rhs) const;

public:
	T x, y, width, height;
};

typedef TRect2<int>		Rect2I;
typedef TRect2<float>	Rect2;

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
template<typename T>
TRect2::TRect2()
	: x(0), y(0), width(0), height(0)
{
}

template<typename T>
TRect2::TRect2(T x, T y, T width, T height)
	: x(x), y(y), width(width), height(height)
{

}

template<typename T>
bool TRect2::contains(const TVector2<T>& point) const
{
	if (point.x >= x && point.x <= (x + width))
	{
		if (point.y >= y && point.y <= (y + height))
			return true;
	}

	return false;
}

template<typename T>
bool TRect2::overlaps(const TRect2& other) const
{
	float otherRight = other.x + other.width;
	float myRight = x + width;

	float otherBottom = other.y + other.height;
	float myBottom = y + height;

	if (x < otherRight && myRight > other.x &&
		y < otherBottom && myBottom > other.y)
		return true;

	return false;
}

template<typename T>
void TRect2::encapsulate(const TRect2& other)
{
	float myRight = x + width;
	float myBottom = y + height;
	float otherRight = other.x + other.width;
	float otherBottom = other.y + other.height;

	if (other.x < x)
		x = other.x;

	if (other.y < y)
		y = other.y;

	if (otherRight > myRight)
		width = otherRight - x;
	else
		width = myRight - x;

	if (otherBottom > myBottom)
		height = otherBottom - y;
	else
		height = myBottom - y;
}

template<typename T>
void TRect2::clip(const TRect2& clipRect)
{
	float newLeft = std::max(x, clipRect.x);
	float newTop = std::max(y, clipRect.y);

	float newRight = std::min(x + width, clipRect.x + clipRect.width);
	float newBottom = std::min(y + height, clipRect.y + clipRect.height);

	x = newLeft;
	y = newTop;
	width = newRight - newLeft;
	height = newBottom - newTop;
}

template<typename T>
void TRect2::transform(const Matrix4& matrix)
{
	Vector4 verts[4];
	verts[0] = Vector4(x, y, 0.0f, 1.0f);
	verts[1] = Vector4(x + width, y, 0.0f, 1.0f);
	verts[2] = Vector4(x, y + height, 0.0f, 1.0f);
	verts[3] = Vector4(x + width, y + height, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++)
		verts[i] = matrix.multiply(verts[i]);

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();

	for (int i = 0; i < 4; i++)
	{
		if (verts[i].x < minX)
			minX = verts[i].x;

		if (verts[i].y < minY)
			minY = verts[i].y;

		if (verts[i].x > maxX)
			maxX = verts[i].x;

		if (verts[i].y > maxY)
			maxY = verts[i].y;
	}

	x = minX;
	y = minY;
	width = maxX - x;
	height = maxY - y;
}

template<typename T>
void TRect2::cut(const TRect2& cutRect, std::vector<TRect2>& pieces)
{
	size_t initialPieces = pieces.size();

	// Cut horizontal
	if (cutRect.x > x && cutRect.x < (x + width))
	{
		TRect2 leftPiece;
		leftPiece.x = x;
		leftPiece.width = cutRect.x - x;
		leftPiece.y = y;
		leftPiece.height = height;

		pieces.push_back(leftPiece);
	}

	if ((cutRect.x + cutRect.width) > x && (cutRect.x + cutRect.width) < (x + width))
	{
		TRect2 rightPiece;
		rightPiece.x = cutRect.x + cutRect.width;
		rightPiece.width = (x + width) - (cutRect.x + cutRect.width);
		rightPiece.y = y;
		rightPiece.height = height;

		pieces.push_back(rightPiece);
	}

	// Cut vertical
	T cutLeft = std::min(std::max(x, cutRect.x), x + width);
	T cutRight = std::max(std::min(x + width, cutRect.x + cutRect.width), x);

	if (cutLeft != cutRight)
	{
		if (cutRect.y > y && cutRect.y < (y + height))
		{
			TRect2 topPiece;
			topPiece.y = y;
			topPiece.height = cutRect.y - y;
			topPiece.x = cutLeft;
			topPiece.width = cutRight - cutLeft;

			pieces.push_back(topPiece);
		}

		if ((cutRect.y + cutRect.height) > y && (cutRect.y + cutRect.height) < (y + height))
		{
			TRect2 bottomPiece;
			bottomPiece.y = cutRect.y + cutRect.height;
			bottomPiece.height = (y + height) - (cutRect.y + cutRect.height);
			bottomPiece.x = cutLeft;
			bottomPiece.width = cutRight - cutLeft;

			pieces.push_back(bottomPiece);
		}
	}

	// No cut
	if (initialPieces == pieces.size())
	{
		if (cutRect.x <= x && (cutRect.x + cutRect.width) >= (x + width) &&
			cutRect.y <= y && (cutRect.y + cutRect.height) >= (y + height))
		{
			// Cut rectangle completely encompasses this one
		}
		else
			pieces.push_back(*this); // Cut rectangle doesn't even touch this one
	}
}

template<typename T>
void TRect2::cut(const std::vector<TRect2>& cutRects, std::vector<TRect2>& pieces)
{
	RectVector tempPieces[2];
	size_t bufferIdx = 0;

	tempPieces[0].push_back(*this);

	for (RectVector::const_iterator itor = cutRects.begin(); itor != cutRects.end(); ++itor)
	{
		const TRect2& cutRect = *itor;

		size_t currentBufferIdx = bufferIdx;

		bufferIdx = (bufferIdx + 1) % 2;
		RectVector& piece_temp = tempPieces[bufferIdx];
		piece_temp.clear();

		for (RectVector::iterator itor1 = piece_temp.begin(); itor1 != piece_temp.end(); ++itor1)
		{
			TRect2& rect = *itor1;
			rect.cut(cutRect, piece_temp);
		}
	}

	pieces = tempPieces[bufferIdx];
}

CUTE_NS_END
