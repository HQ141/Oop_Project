#include "Ship.h"

const std::vector<LPCWSTR> Ship::files = {
		L"png\\FlyingSaucer\\flying-saucer-1.png",
		L"png\\FlyingSaucer\\flying-saucer-2.png",
		L"png\\FlyingSaucer\\flying-saucer-3.png",
		L"png\\FlyingSaucer\\flying-saucer-4.png",
		L"png\\FlyingSaucer\\flying-saucer-5.png"
};

Ship::Ship(const Vec2& initial_position, const Vec2& initial_velocity, Graphics* graphics)
	:
	Projectile(initial_position, initial_velocity),
	firingManager(graphics, 1.0f),
	animation(graphics, files, 2.0f)
{
	SetWidth(animation.GetWidth());
	SetHeight(animation.GetHeight());
}

void Ship::Update(float deltatime)
{
	// update position
	if (!IsFalling()) {
		// ship ignores gravity
		BasicEntity::Update(deltatime);
	}
	else {
		// until you kill it
		Projectile::Update(deltatime);
	}
	if (!IsDestroyed()) {
		// pass deltatime to animation
		animation.Advance(deltatime);
	}
	// update time since last fire
	firingManager.Update(deltatime);
}

bool Ship::CanFire()
{
	return firingManager.CanFire();
}

Projectile* Ship::Fire()
{
	// does not check for 'can fire'
	// so the 'can fire' attriibute may be ignored, up to the field / main to decide
	
	// reset time
	return firingManager.Fire(GetCenter(), Vec2());
}

bool Ship::ProcessWallCollision(const _Rect& walls)
{
	const _Rect rect = GetRect();
	if (rect.bottom >= walls.bottom && !IsDestroyed()) {
		SetVelocity({});
		state = State::Destroyed;
	}
	return Projectile::ProcessWallCollision(walls);
}

void Ship::Draw()
{
	animation.Draw(GetPosition());
}

void Ship::SetVelocity(const Vec2& new_velocity)
{
	BasicEntity::SetVelocity(new_velocity);
}

Vec2 Ship::GetPosition() const
{
	return BasicEntity::GetPosition();
}

void Ship::TakeDamage()
{
	hp--;
	if (hp <= 0 && state == State::Normal) {
		state = State::Falling;
	}
}

void Ship::OnResetDevice()
{
	animation.OnResetDevice();
}

void Ship::OnLostDevice()
{
	animation.OnLostDevice();
}

