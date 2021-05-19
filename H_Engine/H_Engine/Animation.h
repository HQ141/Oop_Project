#pragma once

#include "textureManager.h"
#include "image.h"
#include "Vec2.h"
#include <vector>

// manages everything related to animation
class Animation
{
public:
	// for classes like the ninja which are mirrored when facing left or right
	enum class Direction {
		Right,
		Left
	};
private:
	// utility class
	// one frame of animation
	class Frame {
	public:
		Frame(Graphics* graphics, LPCWSTR file, float scaling, float angle)
		{
			if (!texManager.initialize(graphics, file)) {
				throw GameError(gameErrorNS::FATAL_ERROR, L"Unable to load texture");
			}
			if (!image.initialize(graphics, 0, 0, 0, &texManager)) {
				throw GameError(gameErrorNS::FATAL_ERROR, L"Unable to load image");
			}
			image.setScale(abs(scaling));
			image.setDegrees(angle);
		}
		Frame(const Frame& other)
			:
			texManager(other.texManager),
			image(other.image)
		{
			// the address of the tex manager will change after it is copied
			image.setTextureManager(&texManager);
		}
		void Draw(const Vec2& position, Direction direction) {
			// call the other guy, cuz I don't wanna type the same thing again
			Draw(position.x, position.y, direction);
		}
		void Draw(float x, float y, Direction direction) {
			// set the sprite's position
			image.setX(x);
			image.setY(y);
			// flip if facing left, dont oterwise
			if (direction == Direction::Right) {
				image.flipHorizontal(false);
			}
			else {
				image.flipHorizontal(true);
			}
			// draw it
			image.draw();
		}
		float GetWidth() const {
			return image.getWidth() * image.getScale();
		}
		float GetHeight() const {
			return image.getHeight() * image.getScale();
		}
		void OnResetDevice() {
			texManager.onResetDevice();
		}
		void OnLostDevice() {
			texManager.onLostDevice();
		}
	private:
		TextureManager texManager;
		Image image;
	};
public:
	// constructor loads all the images in given vector into the class
	Animation(Graphics* graphics, const std::vector<LPCWSTR>& files, float scaling, float angle = 0.0f, float frameHoldDuration = 0.1f);
	// moves to the next frome after given hold duration
	void Advance(float deltatime);
	// draw a frame on the given location
	void Draw(const Vec2& position, Direction direction = Direction::Right);
	void Draw(float x, float y, Direction direction = Direction::Right);
	// get the width of sprie post-scaling
	float GetWidth() const;
	// get the height of sprie post-scaling
	float GetHeight() const;

	void OnResetDevice();
	void OnLostDevice();

private:
	// vector of frames
	std::vector<Frame> frames;
	// total time to hold the fame for
	const float frameHoldDuration;
	// time the current frame has already been held for
	float frameHeldFor = 0.0f;
	// index of the current frame being drawn
	int currentFrame = 0;
};

