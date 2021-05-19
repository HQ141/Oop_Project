#pragma once

#include "Vec2.h"

// a class used primarily to detect collision bw objects or with the screen 'walls'
class _Rect
{
public:
	_Rect()
	{
	}
	_Rect(float left, float right, float top, float bottom)
		:
		left(left),
		right(right),
		top(top),
		bottom(bottom)
	{
	}
	_Rect(const Vec2& topLeft, float width, float height) 
		:
		_Rect(topLeft.x, topLeft.x + width, topLeft.y, topLeft.y + height)
	{
	}
	_Rect(const Vec2& topLeft, const Vec2& bottomRight)
		:
		_Rect(topLeft.x, bottomRight.x, topLeft.y, bottomRight.y)
	{
	}

	static _Rect GetRectFromCenter( const Vec2& center, float width, float height ) {
		const float halfWidth = width / 2;
		const float halfHeight = height / 2;
		return _Rect(center - Vec2(halfWidth, halfHeight), width, height);
	}

	// check if two _Rects overlap, when true, it's a collision
	bool DetectOverlap(const _Rect& other) {
		return right >= other.left && left <= other.right
			&& top <= other.bottom && bottom >= other.top;
	}

	// check if a given point is inside the rect
	bool ContainsPoint(const Vec2& p) {
		return p.x >= left && p.x <= right
			&& p.y >= top && p.y <= bottom;
	}

	// check if one reck is fully inside another rect, when false, rect has collided with the 'walls'
	bool IsContainedBy(const _Rect& container) {
		return left >= container.left && right <= container.right
			&& top >= container.top && bottom <= container.bottom;
	}
public:
	// all data is public for ease's sake
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
};

