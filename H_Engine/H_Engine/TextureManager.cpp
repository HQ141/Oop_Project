#include "textureManager.h"

#include <assert.h>

TextureManager::TextureManager()
{
    texture = NULL;
    width = 0;
    height = 0;
    file = NULL;
    graphics = NULL;
    initialized = false;
}
TextureManager::TextureManager(const TextureManager& other)
    :
    TextureManager()
{
    if (other.initialized) {
        initialize(other.graphics, other.file);
    }
}
TextureManager::~TextureManager(){
    if(texture != NULL)
        SAFE_RELEASE(texture);
}
bool TextureManager::initialize(Graphics *g, LPCWSTR f)
{
    // make sure pointers passed were valid
    assert(g != NULL);
    assert(f != NULL);

    try{
        graphics = g;
        file = f;

        hr = graphics->loadTexture(file, graphicsNS::BLACK, width, height, texture);
        if (FAILED(hr))
        {
            SAFE_RELEASE(texture);
            throw std::exception("Error in loading texture");
            return false;
        }
    }
    catch(...) {return false;}
    initialized = true;
    return true;
}
void TextureManager::onLostDevice(){
    if (!initialized)
        return;
    SAFE_RELEASE(texture);
}
void TextureManager::onResetDevice()
{
    if (!initialized)
        return;
    graphics->loadTexture(file, graphicsNS::WHITE, width, height, texture);
}


