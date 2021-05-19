#include "Ninja.h"
#include <math.h>
#include <assert.h>

const std::vector<LPCWSTR> Ninja::run_files = {
	L"Run__000.png",
	L"Run__001.png",
	L"Run__002.png",
	L"Run__003.png",
	L"Run__004.png",
	L"Run__005.png",
	L"Run__006.png",
	L"Run__007.png",
	L"Run__008.png",
	L"Run__009.png"
};
const std::vector<LPCWSTR> Ninja::idle_files = { 
	L"Idle__000.png" 
};

Ninja::Ninja(float initial_x, float initial_y, Graphics* graphics)
	:
	Ninja(Vec2(initial_x, initial_y), graphics)
{
}

Ninja::Ninja(const Vec2& initial_position, Graphics* graphics)
	:
	Projectile(initial_position),
	firingManager(graphics, 2.0f)
{
	// running
	animations.push_back( Animation( graphics, run_files, imageScale ) );
	// jumping
	animations.push_back( Animation( graphics, run_files, imageScale ) );
	// idle
	animations.push_back( Animation(graphics, idle_files, imageScale ) );

	// get width and height of sprite (assume all sprites have the same dimensions... which they should)
	SetWidth(animations[0].GetWidth());
	SetHeight(animations[0].GetHeight());
}

void Ninja::Update(float deltatime)
{
	Projectile::Update(deltatime);
	animations[int(state)].Advance(deltatime);
	firingManager.Update(deltatime);
}

void Ninja::UpdateVelocity(const Vec2& delta_velocity)
{
	// update velocity
	SetVelocity(GetVelocity() + delta_velocity);
}

bool Ninja::ProcessWallCollision(const _Rect& walls)
{
	return Projectile::ProcessWallCollision(walls);
}

void Ninja::Jump(float vertical_velocity)
{
	Vec2 v;
	if (state == State::Running) {
		if (direction == Animation::Direction::Right) {
			v.x = 1.0f;
		}
		else {
			v.x = -1.0f;
		}
	}
	v.y = -3.0f;
	v = v.UnitVector() * vertical_velocity;
	SetVelocity(v);
}

bool Ninja::CanFire() const
{
	return firingManager.CanFire();
}

Projectile* Ninja::Fire(const Vec2& target)
{
	const Vec2 dir = (target - GetPosition()).UnitVector();
	const float magnitude = 1000.0f;

	return firingManager.Fire(GetCenter(), dir * magnitude);
}

void Ninja::Draw()
{
	animations[int(state)].Draw(GetPosition(), direction);
}

Vec2 Ninja::GetPosition() const
{
	return BasicEntity::GetPosition();
}

Vec2 Ninja::GetVelocity() const
{
	return BasicEntity::GetVelocity();
}

void Ninja::SetVelocity(const Vec2& new_velocity)
{
	BasicEntity::SetVelocity(new_velocity);
	UpdateStateAndDirection(new_velocity);
}

void Ninja::SetVelocityX(float new_vx)
{
	SetVelocity({ new_vx, GetVelocity().y });
}

void Ninja::SetVelocityY(float new_vy)
{
	SetVelocity({ GetVelocity().x, new_vy });
}

void Ninja::OnResetDevice()
{
	for (Animation& a : animations) {
		a.OnResetDevice();
	}
}

void Ninja::OnLostDevice()
{
	for (Animation& a : animations) {
		a.OnLostDevice();
	}
}

void Ninja::UpdateStateAndDirection(const Vec2& v)
{
	HandleVelocityX(v.x);
	// y is called later, and is thus dominant on the state
	HandleVelocityY(v.y);
}

void Ninja::HandleVelocityX(float deltaV_x)
{
	// decide state
	// relies on both current and delta v
	if (deltaV_x == 0 && GetVelocity().x == 0) {
		state = State::Idle;
	}
	else {
		state = State::Running;
	}

	// decide direction
	// relies only on delta v
	// remains same if no change
	if (deltaV_x > 0.0f) {
		direction = Animation::Direction::Right;
	}
	else if (deltaV_x < 0.0f) {
		direction = Animation::Direction::Left;
	}
}

void Ninja::HandleVelocityY(float deltaV_y)
{
	// if there is any vertical velocity, ninja is in air, which'll count as jumping
	if (deltaV_y != 0.0f || GetVelocity().y != 0.0f) {
		state = State::Jumping;
	}
}
