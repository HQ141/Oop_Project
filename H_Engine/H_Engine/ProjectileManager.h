#pragma once

#include "Projectile.h"
#include<vector>

// meant for managing kunais and bullets
class ProjectileManager
{
public:
	// default constructor
	ProjectileManager();
	// destructor
	~ProjectileManager();

	// the copy constructor and assignment operator are not needed
	ProjectileManager(const ProjectileManager&) = delete;
	ProjectileManager& operator=(const ProjectileManager&) = delete;

	// adds a new projectile, doesn't have to be of the same type
	void Add(Projectile* p);
	// calls the default update function for the given projectile
	void Update(float deltatime);
	// calls the default process wall collision function for the given projectile
	// returns number of collisions that occured
	int ProcessWallCollisions(const _Rect& walls);
	
	// provide a lambda function/function/functor pointer for what happens to the other entity
	// the projectile will just obey it's own process function
	// returns number of collisions that occured
	template<typename T>
	int ProcessEntityCollisions(BasicEntity& entity, T t) {
		int count = 0;
		for (Projectile* p : projectiles) {
			if (!p->IsDestroyed()) {
				if (p->DetectEntityCollision(entity)) {
					// might just make a virtual 'ProcessEntityCollisions' function
					// instead of hardcoding what happens to the projectile here
					p->SetDestroyed();
					t(entity, p->GetCenter());
					count++;
				}
			}
		}
		return count;
	}
	// clears out all destroyed projectiles
	void RemoveDestroyed();
	void Draw();

private:
	std::vector<Projectile*> projectiles;
};