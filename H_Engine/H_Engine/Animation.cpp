#include "Animation.h"
#include <fstream>
Animation::Animation(Graphics* graphics, const std::vector<LPCWSTR>& files, float scaling, float angle, float frameHoldDuration, bool isCyclic)
	:
	frameHoldDuration(frameHoldDuration),
	isCyclic(isCyclic)
{
	for (const wchar_t* f : files) {
		frames.push_back(Frame(graphics, f, scaling, angle));
	}
}

void Animation::Advance(float deltatime)
{
	frameHeldFor += deltatime;
	while (frameHeldFor >= frameHoldDuration) {
		frameHeldFor -= frameHoldDuration;
		currentFrame++;
		if (currentFrame >= frames.size()) {
			if (isCyclic) {
				currentFrame = 0;
			}
			else {
				// reset to the revious, aka last frame
				currentFrame--;
			}
		}
	}
}

void Animation::Draw(const Vec2& position, Direction direction)
{
	frames[currentFrame].Draw(position, direction);
}

void Animation::Draw(const Vec2& position, float radians)
{
	frames[currentFrame].Draw(position, radians);
}

void Animation::Draw(float x, float y, Direction direction)
{
	frames[currentFrame].Draw(x, y, direction);
}

float Animation::GetWidth() const
{
	return frames[0].GetWidth();
}

float Animation::GetHeight() const
{
	return frames[0].GetHeight();
}

void Animation::OnResetDevice()
{
	for (Frame& f : frames) {
		f.OnResetDevice();
	}
}

void Animation::OnLostDevice()
{
	for (Frame& f : frames) {
		f.OnLostDevice();
	}
}
