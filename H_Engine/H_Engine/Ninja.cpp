#include "Ninja.h"
#include <math.h>
#include <assert.h>

const std::vector<LPCWSTR> Ninja::run_files = {
	L"png\\ninja\\Run__000.png",
	L"png\\ninja\\Run__001.png",
	L"png\\ninja\\Run__002.png",
	L"png\\ninja\\Run__003.png",
	L"png\\ninja\\Run__004.png",
	L"png\\ninja\\Run__005.png",
	L"png\\ninja\\Run__006.png",
	L"png\\ninja\\Run__007.png",
	L"png\\ninja\\Run__008.png",
	L"png\\ninja\\Run__009.png"
};
const std::vector<LPCWSTR> Ninja::idle_files = { 
	L"png\\ninja\\Idle__000.png",
	L"png\\ninja\\Idle__001.png",
	L"png\\ninja\\Idle__002.png",
	L"png\\ninja\\Idle__003.png",
	L"png\\ninja\\Idle__004.png",
	L"png\\ninja\\Idle__005.png",
	L"png\\ninja\\Idle__006.png",
	L"png\\ninja\\Idle__007.png",
	L"png\\ninja\\Idle__008.png",
	L"png\\ninja\\Idle__009.png",
};
const std::vector<LPCWSTR> Ninja::jump_files = {
	L"png\\Ninja\\Jump__000.png",
	L"png\\Ninja\\Jump__001.png",
	L"png\\Ninja\\Jump__002.png",
	L"png\\Ninja\\Jump__003.png",
	L"png\\Ninja\\Jump__004.png",
	L"png\\Ninja\\Jump__005.png",
	L"png\\Ninja\\Jump__006.png",
	L"png\\Ninja\\Jump__007.png",
	L"png\\Ninja\\Jump__008.png",
	L"png\\Ninja\\Jump__009.png",
};
const std::vector<LPCWSTR> Ninja::throw_files = {
	L"png\\Ninja\\Throw__000.png",
	L"png\\Ninja\\Throw__001.png",
	L"png\\Ninja\\Throw__002.png",
	L"png\\Ninja\\Throw__003.png",
	L"png\\Ninja\\Throw__004.png",
	L"png\\Ninja\\Throw__005.png",
	L"png\\Ninja\\Throw__006.png",
	L"png\\Ninja\\Throw__007.png",
	L"png\\Ninja\\Throw__008.png",
	L"png\\Ninja\\Throw__009.png",
};
const std::vector<LPCWSTR> Ninja::jump_throw_files = {
	L"png\\Ninja\\Jump_Throw__000.png",
	L"png\\Ninja\\Jump_Throw__001.png",
	L"png\\Ninja\\Jump_Throw__002.png",
	L"png\\Ninja\\Jump_Throw__003.png",
	L"png\\Ninja\\Jump_Throw__004.png",
	L"png\\Ninja\\Jump_Throw__005.png",
	L"png\\Ninja\\Jump_Throw__006.png",
	L"png\\Ninja\\Jump_Throw__007.png",
	L"png\\Ninja\\Jump_Throw__008.png",
	L"png\\Ninja\\Jump_Throw__009.png",
};
const std::vector<LPCWSTR> Ninja::dead_files = {
	L"png\\ninja\\Dead__000.png",
	L"png\\ninja\\Dead__001.png",
	L"png\\ninja\\Dead__002.png",
	L"png\\ninja\\Dead__003.png",
	L"png\\ninja\\Dead__004.png",
	L"png\\ninja\\Dead__005.png",
	L"png\\ninja\\Dead__006.png",
	L"png\\ninja\\Dead__007.png",
	L"png\\ninja\\Dead__008.png",
	L"png\\ninja\\Dead__009.png",
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
	// initialize attack movement animations
	// running
	movementAnimations.push_back( Animation( graphics, run_files, imageScale ) );
	// jumping
	movementAnimations.push_back( Animation( graphics, jump_files, imageScale, 0.0f, 0.2f, false ) );
	// idle
	movementAnimations.push_back( Animation( graphics, idle_files, imageScale ) );
	// dead
	movementAnimations.push_back( Animation( graphics, dead_files, imageScale, 0.0f, 0.2f, false ) );

	// initialize attack animation
	// throwing
	attackAnimations.push_back( Animation( graphics, throw_files, imageScale, 0.0f, AttackStateHoldDuration / 10.0f, false ) );
	// throwing while jumping
	attackAnimations.push_back( Animation( graphics, jump_throw_files, imageScale, 0.0f, AttackStateHoldDuration / 10.0f, false ) );

	// get width and height of sprite (assume all sprites have the same dimensions... which they should)
	SetWidth(movementAnimations[0].GetWidth());
	SetHeight(movementAnimations[0].GetHeight());
}

void Ninja::Update(float deltatime)
{
	// update position based on the projectile motion formula
	Projectile::Update(deltatime);
	firingManager.Update(deltatime);
	// update appropriate animation
	if (aState != AttackState::Idle) {
		attackAnimations[int(aState)].Advance(deltatime);
	}
	else {
		movementAnimations[int(mState)].Advance(deltatime);
	}
	stateHeldFor += deltatime;
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
	Vec2 v = { 0.0f, 0.0f };
	if (mState == MovementState::Running) {
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

bool Ninja::CanThrow() const
{
	return firingManager.CanFire();
}

Projectile* Ninja::Throw(const Vec2& target)
{
	const Vec2 dir = (target - GetPosition()).UnitVector();
	const float magnitude = 1000.0f;

	// has overriding authority
	// can change state even if previous state's time is not yet over
	if (IsJumping()) {
		if (ChangeState(aState, AttackState::JumpThrowing)) {
			attackAnimations[int(aState)].Reset();
		}
	}
	else {
		if (ChangeState(aState, AttackState::Throwing)) {
			attackAnimations[int(aState)].Reset();
		}
	}
	// ninja turns around to face the direction he's throwing in
	if (dir.x > 0) {
		ChangeDirection(Animation::Direction::Right);
	}
	else {
		ChangeDirection(Animation::Direction::Left);
	}
	
	return firingManager.Fire(GetCenter(), dir * magnitude);
}

void Ninja::Draw()
{
	if (aState != AttackState::Idle) {
		// attack state has priority
		attackAnimations[int(aState)].Draw(GetPosition(), direction);
	}
	else {
		movementAnimations[int(mState)].Draw(GetPosition(), direction);
	}
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

void Ninja::OnResetDevice()
{
	for (Animation& a : movementAnimations) {
		a.OnResetDevice();
	}
}

void Ninja::OnLostDevice()
{
	for (Animation& a : movementAnimations) {
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
		if (ChangeState(mState, MovementState::Idle)) {
			movementAnimations[int(mState)].Reset();
		}
	}
	else {
		if (ChangeState(mState, MovementState::Running)) {
			movementAnimations[int(mState)].Reset();
		}
	}

	// decide direction
	// relies only on delta v
	// remains same if no change
	if (deltaV_x > 0.0f) {
		ChangeDirection(Animation::Direction::Right);
	}
	else if (deltaV_x < 0.0f) {
		ChangeDirection(Animation::Direction::Left);
	}
}

void Ninja::HandleVelocityY(float deltaV_y)
{
	// if there is any vertical velocity, ninja is in air, which'll count as jumping
	if (deltaV_y != 0.0f || GetVelocity().y != 0.0f) {
		if (ChangeState(mState, MovementState::Jumping)) {
			movementAnimations[int(mState)].Reset();
		}
	}
}

void Ninja::ChangeDirection(Animation::Direction new_direction)
{
	direction = new_direction;
}

bool Ninja::CanChangeState() const
{
	if (IsDead()) {
		return false;
	}
	else if (aState == AttackState::Idle) {
		return stateHeldFor >= StateHoldDuration;
	}
	else {
		return stateHeldFor >= AttackStateHoldDuration;
	}
}

void Ninja::TakeDamage()
{
	hp--;
	if (hp <= 0) {
		mState = MovementState::Dead;
	}
}

template<typename s>
bool Ninja::ChangeState(s& state_ref, s new_state)
{
	if (!IsDead()) {
		// can't think of another way / place to reset it
		if (CanChangeState()) {
			aState = AttackState::Idle;
		}
		// don't reassign the same state
		if (state_ref != new_state) {
			stateHeldFor = 0.0f;
			state_ref = new_state;
			return true;
		}
	}
	return false;
}