#include "Bullet.h"
#include <assert.h>

const std::vector<LPCWSTR> Bullet::file = {
	L"bullet.png"
};

Bullet::Bullet(const Vec2& position, const Vec2& velocity, Graphics* graphics)
	:
	Projectile(position, velocity),
	animation(graphics, file, imageScale, degrees)
{
	// flipped because the image is horizontal but the bullet falls vertically
	SetWidth(animation.GetHeight());
	SetHeight(animation.GetWidth());
}

void Bullet::Update(float deltatime)
{
	Projectile::Update(deltatime);

	// advancing animations is unnecessary with only one frame
	// but it can be called in case more frames are ever added
	animation.Advance(deltatime);
}

bool Bullet::ProcessWallCollision(const _Rect& walls)
{
	// for now bullets get destroyed on any kind of collision instead
	// of rebounding, might change that later
	if (Projectile::ProcessWallCollision(walls)) {
		SetDestroyed();
	}
	return true;
}

void Bullet::OnResetDevice()
{
	animation.OnResetDevice();
}

void Bullet::OnLostDevice()
{
	animation.OnLostDevice();
}

void Bullet::Draw()
{
	assert(!IsDestroyed());
	animation.Draw(GetPosition());
}