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
	enum class State {
		Running,
		Jumping,
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

	bool CanFire() const;
	Projectile* Fire(const Vec2& target);

	// draw sprite
	void Draw() override;
	// getters and setters
	// get position
	Vec2 GetPosition() const;
	// get velocity
	Vec2 GetVelocity() const;
	// set velocity
	void SetVelocity(const Vec2& new_velocity) override;
	// only change the x value of velocity
	void SetVelocityX(float new_vx);
	// only change the y value of velocity
	void SetVelocityY(float new_vy);
	// check if ninja is in mid-air
	bool IsInAir() {
		return state == State::Jumping;
	}

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

private:
	State state = State::Idle;
	Animation::Direction direction = Animation::Direction::Right;

	FiringManager<Kunai> firingManager;

	// all types of animations
	float imageScale = 0.2f;
	std::vector<Animation> animations;
	// animation files
	static const std::vector<LPCWSTR> run_files;
	static const std::vector<LPCWSTR> idle_files;
};

