#include "Kunai.h"
#include <assert.h>

// using the bullet image for now, until I can find something else
const std::vector<LPCWSTR> Kunai::file = {
	L"png\\Ninja\\Kunai.png"
};

// copied over from bullet
// which tells me there was still room for at least one other general class

Kunai::Kunai(const Vec2& position, const Vec2& velocity, Graphics* graphics)
	:
	Projectile(position, velocity),
	animation(graphics, file, imageScale)
{
	// flipped because the image is horizontal but the bullet falls vertically
	SetWidth(animation.GetHeight());
	SetHeight(animation.GetWidth());
}

void Kunai::Update(float deltatime)
{
	Projectile::Update(deltatime);

	// advancing animations is unnecessary with only one frame
	// but it can be called in case more frames are ever added
	animation.Advance(deltatime);
}

bool Kunai::ProcessWallCollision(const _Rect& walls)
{
	// kunai can rebound off of all walls except the bottom wall, should make it interesting...
	_Rect rect = GetRect();
	if (rect.bottom >= walls.bottom) {
		SetDestroyed();
	}
	return Projectile::ProcessWallCollision(walls);
}

void Kunai::OnResetDevice()
{
	animation.OnResetDevice();
}

void Kunai::OnLostDevice()
{
	animation.OnLostDevice();
}

void Kunai::Draw()
{
	assert(!IsDestroyed());
	const Vec2 v = GetVelocity();
	float radians = (float) atan(v.y / v.x);
	const float angleOffset = PI / 2;
	
	if (v.x > 0) {
		// right two quadrants
		if (radians >= 0) {
			// Q1
			animation.Draw(GetPosition(), angleOffset + radians);
		}
		else {
			// Q4
			animation.Draw(GetPosition(), angleOffset + 2 * PI - abs(radians));
		}
	}
	else if (v.x < 0) {
		// left two quadrants
		if (radians >= 0) {
			// Q3
			animation.Draw(GetPosition(), angleOffset + PI + radians);
		}
		else {
			// Q2
			animation.Draw(GetPosition(), angleOffset + PI - abs(radians));
		}
	}
	else {
		// vertically up or down
	}
}