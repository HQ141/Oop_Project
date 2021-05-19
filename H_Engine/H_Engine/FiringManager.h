#pragma once

#include "graphics.h"
#include "Projectile.h"

// funny class name, heh
template<typename T>
class FiringManager
{
public:
	FiringManager(Graphics* graphics, float firingPeriod = 1.0f)
		:
		firingPeriod(firingPeriod),
		graphics(graphics)
	{}
	bool CanFire() const {
		return timeSinceLastFire >= firingPeriod;
	}
	T* Fire(const Vec2& position, const Vec2& velocity) {
		timeSinceLastFire = 0.0f;
		return new T(position, velocity, graphics);
	}
	void Update(float deltatime) {
		timeSinceLastFire += deltatime;
	}
private:
	const float firingPeriod;
	float timeSinceLastFire = 0.0f;

	Graphics* graphics;
};

