#pragma once

#include "BasicEntity.h"
#include "Projectile.h"
#include "Animation.h"
#include "Vec2.h"

// bullet was originally just a basic entity, but I've updated it to be a Projectile, it makes more sense
	// also the physics of the bullet should be more interesting now, as it can acceleate

class Bullet : public Projectile
{
public:
	//constructor
	Bullet(const Vec2& position, const Vec2& velocity, Graphics* graphics);

	// update position and animation
	void Update(float deltatime) override;
	// override process wall collision
	bool ProcessWallCollision(const _Rect& walls) override;

	// draws bullet depending on its position
	void Draw() override;

	// gotta override these

	void OnResetDevice() override;
	void OnLostDevice() override;

private:

	// animation info
	static const std::vector<LPCWSTR> file;
	const float degrees = 270.0f;
	const float imageScale = 1.0f;
	Animation animation; 
	// i just realized this is very inefficient since each bullet instance will 
	// have it's own version of animation, it's own frames, etc... all of which are identical
	// but with the way the animation class works, it's unavoidable
};