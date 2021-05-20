#pragma once

#include "BasicEntity.h"
#include "Projectile.h"
#include "Animation.h"
#include "FiringManager.h"
#include "Kunai.h"

#include "Vec2.h"
#include <vector>

class Ninja : public Projectile
{
	// Okay... this needs some justification
	// 	   it feels weird to call a Ninja a projectile
	// 	   but basically, within the context of this game, a projectile 
	// 	   is just a BasicEntity that experiences projectile motion.
	// 	   Which... makes Ninja a projectile...
private:
	enum class MovementState {
		Running,
		Jumping,
		Idle,
		Dead
	};
	enum class AttackState {
		Throwing,
		JumpThrowing, // not actually necessary, just adding for the sake of variable(?) indexing
		Idle
	};

public:
	// constructors
	Ninja(float initial_x, float initial_y, Graphics* graphics);
	Ninja(const Vec2& initial_position, Graphics* graphics);
	
	// update position of ninja based on current velocity given deltatime
	void Update(float deltatime) override;
	
	// update velocity based on given acceleration and deltatime
	void UpdateVelocity(const Vec2& delta_velocity);
	
	// override to also consider jumping
	bool ProcessWallCollision(const _Rect& walls) override;
	// used for jumping instead of setting velocity directly
	void Jump(float vertical_velocity);

	// throwing Kunai

	bool CanThrow() const;
	Projectile* Throw(const Vec2& target);

	// draw sprite
	void Draw() override;
	// getters and setters
	// get position
	Vec2 GetPosition() const;
	// get velocity
	Vec2 GetVelocity() const;
	// set velocity
	void SetVelocity(const Vec2& new_velocity) override;
	// check if ninja is in mid-air
	bool IsJumping() const {
		return mState == MovementState::Jumping;
	}
	// check if ninja is running
	bool IsRunning() const {
		return mState == MovementState::Running;
	}
	// check if ninja is idle (not moving or attacking)
	bool IsIdle() const {
		return mState == MovementState::Idle;
	}
	bool IsDead() const {
		return mState == MovementState::Dead;
	}
	// check if sninja can change state
	bool CanChangeState() const;

	// override take damage
	virtual void TakeDamage() override;

	// gotta override these

	void OnResetDevice() override;
	void OnLostDevice() override;

private:
	// for updating which animation get's shown on screen
	// overall
	void UpdateStateAndDirection(const Vec2& v);
	// handle the x part
	void HandleVelocityX(float deltaV_x);
	// handle the y part
	// this one also uses the current velocity
	void HandleVelocityY(float deltaV_y);
	// changes state
	// works for both attack and movement states
	// also resets aState, because aState has overriding authority in drawing
	// returns true if state change was successful (the new state wasn't a copy of the previous state)
	template<typename s>
	bool ChangeState(s& state_ref, s new_state);
	void ChangeDirection(Animation::Direction new_direction);
private:
	// actual gameplay stuff
	// number of hits ninja can take
	int hp = 3;
	// controls Kunai throwing, part of it anyway
	FiringManager<Kunai> firingManager;

	// misc... i guess
	MovementState mState = MovementState::Idle;
	AttackState aState = AttackState::Idle;
	Animation::Direction direction = Animation::Direction::Right;


	// need to calculate how long a jump lasts and add a constexpr variable here to store that

	// all things animation-related
	float imageScale = 0.2f;
	std::vector<Animation> movementAnimations;
	std::vector<Animation> attackAnimations;
	// animation files
	static const std::vector<LPCWSTR> run_files;
	static const std::vector<LPCWSTR> jump_files;
	static const std::vector<LPCWSTR> idle_files;
	static const std::vector<LPCWSTR> throw_files;
	static const std::vector<LPCWSTR> jump_throw_files;
	static const std::vector<LPCWSTR> dead_files;

	// ended up needing this because of how many animations ninja has
	// and how frequently he alternates between them
	static constexpr float StateHoldDuration = 0.2f;
	static constexpr float AttackStateHoldDuration = 0.3f;
	float stateHeldFor = 0.0f;
};

