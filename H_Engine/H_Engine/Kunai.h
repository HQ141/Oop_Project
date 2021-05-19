#pragma once
#include "Projectile.h"
#include "Animation.h"

class Kunai : public Projectile
{
public:
	//constructor
	Kunai(const Vec2& position, const Vec2& velocity, Graphics* graphics);

	// update position and animation
	void Update(float deltatime);
	// override process wall collision
	bool ProcessWallCollision(const _Rect& walls) override;

	// draws bullet depending on its position
	void Draw() override;

	// gotta override these

	void OnResetDevice() override;
	void OnLostDevice() override;

private:

	// drawing related
	// animation files
	static const std::vector<LPCWSTR> file;
	float imageScale = 1.0f;
	float degrees = 270.0f;
	Animation animation;
};

