#include "BasicEntity.h"

#include <assert.h>

bool BasicEntity::IsDestroyed()
{
	return isDestoryed;
}

void BasicEntity::SetDestroyed()
{
	// should only be called when an entity hasn'r already been destroyed
	assert(!IsDestroyed());
	isDestoryed = true;
}

BasicEntity::BasicEntity()
{
}

BasicEntity::BasicEntity(const Vec2& position)
	:
	position(position)
{
}

BasicEntity::BasicEntity(const Vec2& position, const Vec2& velocity)
	:
	position(position),
	velocity(velocity)
{
}

_Rect BasicEntity::GetRect() const
{
	assert(width != -1);
	assert(height != -1);

	return _Rect(position, width, height);
}

void BasicEntity::Update(float deltatime)
{
	position += velocity * deltatime;
}

Vec2 BasicEntity::GetPosition() const
{
	return position;
}

Vec2 BasicEntity::GetVelocity() const
{
	return velocity;
}

void BasicEntity::SetPosition(const Vec2& new_position)
{
	position = new_position;
}

void BasicEntity::SetVelocity(const Vec2& new_velocity)
{
	velocity = new_velocity;
}

void BasicEntity::SetWidth(float width_in)
{
	width = width_in;
}

void BasicEntity::SetHeight(float height_in)
{
	height = height_in;
}

float BasicEntity::GetWidth()
{
	return width;
}

float BasicEntity::GetHeight()
{
	return height;
}

bool BasicEntity::DetectEntityCollision(const BasicEntity& other) const
{
	assert(width >= 0);
	assert(height >= 0);
	
	return GetRect().DetectOverlap(other.GetRect());
}

bool BasicEntity::ProcessWallCollision(const _Rect& walls)
{
	_Rect rect = GetRect();
	if (!rect.IsContainedBy(walls)) {

		if (rect.left < walls.left) {
			position.x += (walls.left - rect.left);
		}
		else if (rect.right > walls.right) {
			position.x += (walls.right - rect.right);
		}
		if (rect.top < walls.top) {
			position.y += (walls.top - rect.top);
		}
		else if (rect.bottom > walls.bottom) {
			position.y += (walls.bottom - rect.bottom);
		}

		return true;
	}
	return false;
}

Vec2 BasicEntity::GetCenter() const
{
	assert(width > 0 && height > 0);
	return position + Vec2(width / 2, height / 2);
}
