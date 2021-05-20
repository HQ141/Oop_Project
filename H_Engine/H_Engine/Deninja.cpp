#include "Deninja.h"

#include <algorithm>

Deninja::Deninja()
{
}

Deninja::~Deninja()
{
    SAFE_DELETE(ninja);
    SAFE_DELETE(ship);
    SAFE_DELETE(background);
}

void Deninja::initialize(HWND hwnd) 
{
	Game::initialize(hwnd);

    ship = new Ship({ 100.0f, 100.0f }, {1000.0f, 0.0f}, graphics);
    ninja = new Ninja(0.0f, 800.0f, graphics);
    // just making a vector on the go
    background = new Animation(graphics, std::vector<LPCWSTR>({L"png\\BG.png"}), 1.0f);
}

void Deninja::update()
{
    if (gameOver) {
        // display some message
        // quit game or provide a way to start over
        return;
    }

    // get the time that has passed since the previous frame
    const float deltatime = timer.Mark();

    // update ship's position
    ship->Update(deltatime);

    // see if ship can fire a new bullet and do so if it can
    if (ship->CanFire() && !ship->IsDestroyed()) {
        bullets.Add(ship->Fire());
    }

    // update the all bullets and kunai
    bullets.Update(deltatime);
    kunai.Update(deltatime);

    // calculate the player's velocity based on user input
    Vec2 vel = { 0,0 };
    // maxumum human running speed = 12.5 ms-1
    const float movementSpeed = 572.5f;
    // human juming initial velocity = 2.7 ms-1
    // this guy is superhuman, so... 10 ms-1
    const float jumpSpeed = 458.0f;
    // see projectile.h for details
    if (!ninja->IsDead()) {
        if (input->isKeyDown('D')) {
            vel.x += movementSpeed;
        }
        else if (input->isKeyDown('A')) {
            vel.x -= movementSpeed;
        }

        if (input->isKeyDown('W')) {
            if (!ninja->IsJumping()) {
                ninja->Jump(jumpSpeed);
            }
        }

        if (input->getMouseLButton()) {
            if (ninja->CanThrow()) {
                const float targetX = (float)input->getMouseX();
                const float targetY = (float)input->getMouseY();
                if (ninja->IsRunning()) {
                    // stop running to throw
                    ninja->SetVelocity({});
                }
                kunai.Add(ninja->Throw(Vec2{ targetX, targetY }));
            }
        }

        if (ninja->CanChangeState() && !ninja->IsJumping()) {
            ninja->SetVelocity(vel);
        }
    }

    ninja->Update(deltatime);
}

void Deninja::ai()
{
}

void Deninja::collisions()
{
    const _Rect walls(Vec2(0, 0), GAME_WIDTH, GAME_HEIGHT-100u);
    
    bullets.ProcessWallCollisions(walls);
    // process all bullet collisions with ninja
    bullets.ProcessEntityCollisions(*ninja, 
        // lambda function for what happens to the ninja from a collision
        [](BasicEntity& ninja, const Vec2& bulletCenter) 
        {
            ninja.TakeDamage();
            // ninja stops for a bit
            ninja.SetVelocity({});
        }
    );
    bullets.RemoveDestroyed();

    kunai.ProcessWallCollisions(walls);
    // process all wall collisions with ship
    kunai.ProcessEntityCollisions(*ship,
        // lambda function for what happens to the ship from a collision
        [](BasicEntity& ship, const Vec2& bulletCenter)
        {
            ship.TakeDamage();
            ship.SetVelocity(ship.GetVelocity() *= 0.9);
        }
    );
    kunai.RemoveDestroyed();

    ninja->ProcessWallCollision(walls);
    ship->ProcessWallCollision(walls);
}

void Deninja::releaseAll()
{
    if (background != NULL) {
        background->OnLostDevice();
    }
    if (ship != NULL) {
        ship->OnLostDevice();
    }
    if (ninja != NULL) {
        ninja->OnLostDevice();
    }
    Game::releaseAll();
    
    return;
}

void Deninja::resetAll()
{
    if (background != NULL) {
        background->OnResetDevice();
    }
    if (ship != NULL) {
        ship->OnResetDevice();
    }
    if (ninja != NULL) {
        ninja->OnResetDevice();
    }
    Game::resetAll();
    
    return;
}

void Deninja::render()
{
    graphics->spriteBegin();

    background->Draw(Vec2(0.0f, 0.0f));
    
    ship->Draw();
    ninja->Draw();

    bullets.Draw();
    kunai.Draw();

    graphics->spriteEnd();
}