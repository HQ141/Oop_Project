#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    for (Projectile* p : projectiles) {
        delete p;
        p = NULL;
    }
}

void ProjectileManager::Add(Projectile* p)
{
	projectiles.push_back(p);
}

void ProjectileManager::Update(float deltatime)
{
	for (Projectile* p : projectiles) {
		p->Update(deltatime);
	}
}

int ProjectileManager::ProcessWallCollisions(const _Rect& walls)
{
    int count = 0;
	for (Projectile* p : projectiles) {
        if (p->ProcessWallCollision(walls)) {
            count++;
        }
	}
    return count;
}

void ProjectileManager::RemoveDestroyed()
{
    // remove any projectiles that were destroyed from collisions or any other means
    const std::vector<Projectile*>::const_iterator newEnd = std::remove_if(
        // begin and end points
        projectiles.begin(), projectiles.end(),
        // lambda function to remove destroyed projectiles
        [](Projectile* p) {
            if (p->IsDestroyed()) {
                delete p;
                p = NULL;
                return true;
            }
            return false;
        }
    );
    // clear out the freed up space in the vector
    projectiles.erase(newEnd, projectiles.end());
}

void ProjectileManager::Draw()
{
    for (Projectile* p : projectiles) {
        p->Draw();
    }
}
