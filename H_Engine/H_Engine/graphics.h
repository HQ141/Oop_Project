
#ifndef GRAPHICS_H
#define GRAPHICS_H
#define WIN32_LEAN_AND_MEAN
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include "Constant.h"
#include "gameError.h"
#define LP_TEXTURE  LPDIRECT3DTEXTURE9
#define LP_SPRITE   LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9

struct SpriteData
{
    int         width;
    int         height;
    float       x;
    float       y;
    float       scale;
    float       angle;
    RECT        rect;
    LP_TEXTURE  texture;
    bool        flipHorizontal;
    bool        flipVertical;
};

class Graphics{
private:
    LP_3D       direct3d;
    LP_3DDEVICE device3d;
    LP_SPRITE   sprite;
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE pMode;
    HRESULT     result;
    HWND        hwnd;
    bool        fullscreen;
    int         width;
    int         height;
    COLOR_ARGB  backColor;
    void    initD3Dpp();

public:
    Graphics();
    virtual ~Graphics();
    void    releaseAll();
    void    initialize(HWND hw, int width, int height, bool fullscreen);
    HRESULT loadTexture(LPCWSTR filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
    HRESULT showBackbuffer();
    bool    isAdapterCompatible();
    void    drawSprite(const SpriteData &spriteData, COLOR_ARGB color = graphicsNS::WHITE);
    HRESULT reset();
    void    changeDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);
    LP_3D   get3D()             { return direct3d; }
    LP_3DDEVICE get3Ddevice()   { return device3d; }
    LP_SPRITE   getSprite()     { return sprite; }
    HDC     getDC()             { return GetDC(hwnd); }
    HRESULT getDeviceState();
    bool    getFullscreen()     { return fullscreen; }
    void setBackColor(COLOR_ARGB c) {backColor = c;}
    HRESULT beginScene(){
        result = E_FAIL;
        if(device3d == NULL)
            return result;
        device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0);
        result = device3d->BeginScene();
        return result;
    }
    HRESULT endScene() {
        result = E_FAIL;
        if(device3d)
            result = device3d->EndScene();
        return result;
    }
    void spriteBegin(){
        sprite->Begin(D3DXSPRITE_ALPHABLEND);
    }
    void spriteEnd() {
        sprite->End();
    }
};

#endif

