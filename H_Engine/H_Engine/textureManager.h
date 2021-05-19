#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "Constant.h"

class TextureManager{
  private:
    UINT       width;
    UINT       height;
    LP_TEXTURE texture;
    LPCWSTR file;
    Graphics *graphics;
    bool    initialized;
    HRESULT hr;

  public:
    TextureManager();
    TextureManager(const TextureManager& other);
    virtual ~TextureManager();
    LP_TEXTURE getTexture() const {return texture;}
    UINT getWidth() const {return width;}
    UINT getHeight() const {return height;}
    virtual bool initialize(Graphics *g, LPCWSTR file);
    virtual void onLostDevice();
    virtual void onResetDevice();
};

#endif

