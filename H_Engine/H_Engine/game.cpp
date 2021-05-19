#include "game.h"
Game::Game()
{
    input = new Input();
    paused = false;
    graphics = NULL;
    initialized = false;
}


Game::~Game()
{
    deleteAll(); 
    ShowCursor(true);
}
LRESULT Game::messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if(initialized)
    {
        switch( msg )
        {
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            case WM_KEYDOWN: case WM_SYSKEYDOWN:
                input->keyDown(wParam);
                return 0;
            case WM_KEYUP: case WM_SYSKEYUP:
                input->keyUp(wParam);
                return 0;
            case WM_CHAR:              
                input->keyIn(wParam);
                return 0;
            case WM_MOUSEMOVE:            
                input->mouseIn(lParam);
                return 0;
            case WM_INPUT:
                input->mouseRawIn(lParam);
                return 0;
            case WM_LBUTTONDOWN: 
                input->setMouseLButton(true);
                input->mouseIn(lParam);
                return 0;
            case WM_LBUTTONUP:               
                input->setMouseLButton(false);
                input->mouseIn(lParam);     
                return 0;
            case WM_RBUTTONDOWN:            
                input->setMouseRButton(true);
                input->mouseIn(lParam);     
                return 0;
            case WM_RBUTTONUP:               
                input->setMouseRButton(false);
                input->mouseIn(lParam);   
                return 0;
        }
    }
    return DefWindowProc( hwnd, msg, wParam, lParam ); 
}
void Game::initialize(HWND hw){
    hwnd = hw; 
    graphics = new Graphics();
    graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);
    input->initialize(hwnd, false);
    if(QueryPerformanceFrequency(&timerFreq) == false)
        throw GameError(gameErrorNS::FATAL_ERROR, L"Error initializing high resolution timer");

    QueryPerformanceCounter(&timeStart);
    initialized = true;
}
void Game::renderGame(){
    if (SUCCEEDED(graphics->beginScene()))
    {
        render();
        graphics->endScene();
    }
    handleLostGraphicsDevice();
    graphics->showBackbuffer();
}

void Game::handleLostGraphicsDevice()
{
    hr = graphics->getDeviceState();
    if(FAILED(hr)) {
        if(hr == D3DERR_DEVICELOST){
            Sleep(100);  
            return;
        } 
        else if(hr == D3DERR_DEVICENOTRESET) {
            releaseAll();
            hr = graphics->reset();
            if(FAILED(hr))
                return;
            resetAll();
        }
        else
            return; 
    }
}

void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
    releaseAll();             
    graphics->changeDisplayMode(mode);
    resetAll();                  
}
void Game::run(HWND hwnd)
{
    if(graphics == NULL) 
        return;
    //CHVHVSHDJHSDHJDHKSHJHDSH
    if (!paused){
        update(); 
        ai(); 
        collisions();
    }
    renderGame(); 
    if (input->isKeyDown(ALT_KEY) && input->wasKeyPressed(ENTER_KEY))
        setDisplayMode(graphicsNS::TOGGLE);
    if (input->isKeyDown(ESC_KEY))
        setDisplayMode(graphicsNS::WINDOW);
    input->clear(inputNS::KEYS_PRESSED);
}
void Game::releaseAll()
{}
void Game::resetAll()
{}
void Game::deleteAll()
{
    releaseAll();
    SAFE_DELETE(graphics);
    SAFE_DELETE(input);
    initialized = false;
}