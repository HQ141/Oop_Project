#pragma once
#include "BasicEntity.h"

// anything that acts like a projectile, basically
class Projectile : public BasicEntity
{
public:
	// overriding process wall collisions
	// assuming perfectly elastic collisions
	bool ProcessWallCollision(const _Rect& walls) override {
		
		const _Rect rect = GetRect();
		// left or right collision
		if (rect.left < walls.left || rect.right > walls.right) {
			ReboundX();
		}
		// top collision
		if (rect.top < walls.top) {
			ReboundY();
		}
		// bottom collision
		if (rect.bottom > walls.bottom) {
			// for this game, bottom collision just means zero vel, 
			// because otherwise I'd ave to account for the gradual 
			// loss of energy over time so projectile don't keep 
			// bouncing for eternity... which is, frankly, yuck
			// or.... I might decide to do it later, just cause
			SetVelocity({ GetVelocity().x, 0.0f });
		}
		return BasicEntity::ProcessWallCollision(walls);
	}

	void ReboundX() {
		const Vec2 vel = GetVelocity();
		SetVelocity({ -vel.x, vel.y });
	}
	void ReboundY() {
		const Vec2 vel = GetVelocity();
		SetVelocity({ vel.x, -vel.y });
	}

	// JUST AN ESTIMATION of projectile motion
	void Update(float deltatime) override {
		// initial position
		const Vec2 pos = GetPosition();
		// initial velocity
		const Vec2 u = GetVelocity();
		// final velocity
		const Vec2 v = u + (DownwardAccelerationVec * deltatime);

		// x increases normally
		const float dx = u.x * deltatime;
		// s = ut + 0.5 at^2
		const float dy = u.y * deltatime + 0.5f * (DownwardAcceleration * deltatime * deltatime);

		// new velocity is just v (the final velocity calculated)
		BasicEntity::SetVelocity(v);
		// new position is the original position + delta_position
		BasicEntity::SetPosition(pos + Vec2(dx, dy));
	}

protected:
	// protected constructor, although though it's not necessary
	Projectile(const Vec2& position = { 0.0f, 0.0f }, const Vec2& velocity = { 0.0f, 0.0f })
		:
		BasicEntity(position, velocity)
	{}
	
private:
	// pixels / sec^2
	static constexpr float DownwardAcceleration = 450.0f;
	const Vec2 DownwardAccelerationVec = {0.0f, DownwardAcceleration};
};

// some info for later
// assumptions made for the physics:
//	The ninja is 2 meters tall
//	that makes, 1 meter = 45.8 pixels
//	downward acceleration is then 9.81m / s^2 ~ 450 pixels / s^2
//