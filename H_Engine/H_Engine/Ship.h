#pragma once

#include "Vec2.h"
#include "Animation.h"
#include "Projectile.h"
#include "Bullet.h"
#include "FiringManager.h"

#include <vector>

class Ship : public Projectile
{
private:
	enum class State {
		Normal,
		Falling,
		Destroyed
	};
public:
	Ship(const Vec2& initial_position, const Vec2& initial_velocity, Graphics* graphics);

	// update position and animation
	void Update(float deltatime);

	// returns true if a given amount of time has passed since the last fire
	bool CanFire();
	// returns a pointer to dynamically allocated bullet, as it's faster this way
	Projectile* Fire();

	// process wall collisions
	virtual bool ProcessWallCollision(const _Rect& walls) override;

	// draw the ship
	void Draw() override;

	// updates the velocity of the ship
	void SetVelocity(const Vec2& new_velocity);

	// get the ship's position
	Vec2 GetPosition() const;
	// remove one hp and alter state accordingly
	void TakeDamage() override;
	// check if ship is falling
	bool IsFalling() {
		return state == State::Falling;
	}
	// check if ship has been destoryed
	// might rename later, as it's currently obverloading the IsDestroyed in BasicEntity
	bool IsDestroyed() {
		return state == State::Destroyed;
	}

	// gotta override these

	void OnResetDevice() override;
	void OnLostDevice() override;

private:
	// number of hits ship can take
	int hp = 3;
	// the current state the ship is in
	State state = State::Normal;

	// firing
	FiringManager<Bullet> firingManager;

	// might make a projectile manager class that deals with an entitie's projectiles

	// animation area
	// list of files to load image from
	static const std::vector<LPCWSTR> files;
	Animation animation;
};

