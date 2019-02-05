#include "textureManager.h"

TextureManager::TextureManager()
{
  this->texture = NULL;
  this->width, this->height = 0;
  this->file = NULL;
  this->graphics = NULL;
  this->initialised = false;
}

TextureManager::~TextureManager() { safeRelease(texture); }

bool TextureManager::initialise(Graphics* g, const char* file)
{
  try {
    this->graphics = g;
    this->file = file;

    this->hr = graphics->loadTexture(this->file, TRANSCOLOR, this->width,
                                     this->height, this->texture);

    if (FAILED(hr)) {
      safeRelease(this->texture);
      return false;
    }
  } catch (...) {
    return false;
  }

  this->initialised = true;
  return true;
}

void TextureManager::onLostDevice()
{
  if (!this->initialised) return;
  safeRelease(this->texture);
}

void TextureManager::onResetDevice()
{
  if (!this->initialised) return;
  this->graphics->loadTexture(this->file, TRANSCOLOR, this->width, this->height,
                              this->texture);
}
