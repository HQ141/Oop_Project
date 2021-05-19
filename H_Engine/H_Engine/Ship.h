#pragma once

#include "Vec2.h"
#include "Animation.h"
#include "BasicEntity.h"
#include "Bullet.h"
#include "FiringManager.h"

#include <vector>

class Ship : public BasicEntity
{
public:
	Ship(float initial_x, float initial_y, Graphics* graphics);
	Ship(const Vec2& initial_position, Graphics* graphics);

	// update position and animation
	void Update(float deltatime);

	// returns true if a given amount of time has passed since the last fire
	bool CanFire();
	// returns a pointer to dynamically allocated bullet, as it's faster this way
	Projectile* Fire();

	// draw the ship
	void Draw() override;

	// override to rebound on impact
	bool ProcessWallCollision(const _Rect& walls) override;

	// updates the velocity of the ship
	void SetVelocity(const Vec2& new_velocity);

	// get the ship's position
	Vec2 GetPosition() const;

	// gotta override these

	void OnResetDevice() override;
	void OnLostDevice() override;

private:
	// ship gets stuck at the wall without this
	bool isCollidingWithWall = false;

	// firing
	FiringManager<Bullet> firingManager;

	// might make a projectile manager class that deals with an entitie's projectiles

	// animation area
	// list of files to load image from
	static const std::vector<LPCWSTR> files;
	Animation animation;
};

