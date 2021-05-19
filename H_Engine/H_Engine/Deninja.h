#ifndef NINJA_H
#define NINJA_H
#include"game.h"

#include "Vec2.h"
#include "_Rect.h"
#include "Ninja.h"
#include "Ship.h"
#include "Bullet.h"
#include "Kunai.h"
#include "Timer.h"
#include "ProjectileManager.h"

#include <vector>

class Deninja :public Game
{
private:
    // the timers for the entire game
    Timer timer;
    
    //
    Ninja* ninja = NULL;
    Ship* ship = NULL;
    ProjectileManager bullets;
    ProjectileManager kunai;

    // isn't really animated, but there's room for it to be
    // could also make a new class to handle 'scenes' with several animations
    // has to be a pointer because a graphics pointer is needed to construct it
    Animation* background = NULL;

    int score = 0;
    bool gameOver = false;
public:
    Deninja();
    virtual ~Deninja();
    void initialize(HWND hwnd);
    void update();
    void ai();
    void collisions();
    void render();
    void releaseAll();
    void resetAll();

};

#endif