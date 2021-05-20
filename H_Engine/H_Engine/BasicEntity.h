#pragma once

#include "Vec2.h"
#include "_Rect.h"

// anything that has a position, can move, and collide with other things
// a simple rectangle
class BasicEntity
{
public:
	// just returs true or false
	bool DetectEntityCollision(const BasicEntity& other) const;
	// returns true when there is a collision
	// bonds entity within the walls
	// can be overridden to also rebound the entity
	virtual bool ProcessWallCollision(const _Rect& walls);

	// getters and setters
	// get current position
	Vec2 GetPosition() const;
	// get current velocity
	Vec2 GetVelocity() const;
	// set position, can be overrided to have some additional effect
	virtual void SetPosition(const Vec2& position);
	// set velocity, can be overrided to have some additional effect
	virtual void SetVelocity(const Vec2& velocity);
	// get the center of the entity
	Vec2 GetCenter() const;

	// animation related
	// can not be implemented here as all entities have very different animations
	// but they all DO have animations
	// could also just make a local protected vector of animations, but... this is a BASIC entity
	
	// every basic entity will be draw-able
	// cannot be implemented now because no Graphics*
	virtual void Draw() = 0;
	// must be implemented by every entity
	virtual void OnResetDevice() = 0;
	// must be implemented by every entity
	virtual void OnLostDevice() = 0;

	// every entity has some kind of lifespan (infinity still counts)
	// said lifespan ends when sone condition is met, it can be hp, or a collision or anythin
	
	// for entities that can take damage in any way (times since spawn counts as well)
	virtual void TakeDamage() {
		// does nothing
		// will maybe make this pure virtual later if I feel like seting some damage-over-time
		// condition for all entities, not for now...
		// or might just make an 'actor' or 'LivingBeing' class... I don't know, I'm too tired right now
	}
	// check if an entity has been destroyed
	bool IsDestroyed();
	// mark an entity as destroyed
	void SetDestroyed();

	// to avoid memory leaks
	virtual ~BasicEntity() {
	}

protected:
	// constructors
	// were originally protected to make the class abstract, but that's not necessary anymore
	// since the ure virtual functions were added
	// but I'm still gonne keep them here anyway
	BasicEntity();
	BasicEntity(const Vec2& position);
	BasicEntity(const Vec2& position, const Vec2& velocity);

protected:
	// get rect to detect collisions
	_Rect GetRect() const;
	// update position based on current velocity
	virtual void Update(float deltatime);

	// width and height must be initialized using these,
	// as it's not possible at runtime through the parameterized constructors
	void SetWidth(float width);
	void SetHeight(float height);
	float GetWidth();
	float GetHeight();

private:
	// variable to track entity's current sttate
	bool isDestoryed = false;
	// position on screen
	Vec2 position = {0.0f,0.0f};
	// velocity
	Vec2 velocity = { 0.0f,0.0f };
	// width of sprite
	float width = -1.0f;
	// height of sprite
	float height = -1.0f;
};

