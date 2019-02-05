#include "image.h"

Image::Image()
{
  this->initialised = false;

  this->spriteData.width, this->spriteData.height = 2;
  this->spriteData.x, this->spriteData.y = 0.0;
  this->spriteData.scale = 1.0;
  this->spriteData.angle = 0.0;
  this->spriteData.rect.top = 0;
  this->spriteData.rect.right = this->spriteData.width;
  this->spriteData.rect.bottom = this->spriteData.height;
  this->spriteData.texture = NULL;
  this->spriteData.flipHorizontal, this->spriteData.flipVertical = false;

  this->cols = 1;
  this->textureManager = NULL;
  this->startFrame, endFrame, currentFrame = 0;
  this->frameDelay = 1.0; // 1 sec/frame
  this->animTimer = 0.0;
  this->visible = true; // the image is visible
  this->loop = true;    // loop frames
  this->animComplete = false;
  this->graphics = NULL;                 // link to graphics system
  this->colorFilter = graphicsNS::WHITE; // WHITE for no change
}

// Image::~Image() {}

bool Image::initialise(Graphics* g, int width, int height, int ncols,
                       TextureManager* textureM)
{
  try {
    graphics = g;
    textureManager = textureM;

    spriteData.texture = textureManager->getTexture();
    if (width == 0) width = textureManager->getWidth();
    spriteData.width = width;
    if (height == 0) height = textureManager->getHeight();
    spriteData.height = height;
    cols = ncols;
    if (cols == 0) cols = 1;

    // configure spriteData.rect to draw currentFrame
    spriteData.rect.left = (currentFrame % cols) * spriteData.width;
    // right edge + 1
    spriteData.rect.right = spriteData.rect.left + spriteData.width;
    spriteData.rect.top = (currentFrame / cols) * spriteData.height;
    // bottom edge + 1
    spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
  } catch (...) {
    return false;
  }
  initialised = true;
  return true;
}

void Image::draw(COLOR_ARGB color)
{
  if (!this->visible || this->graphics == NULL) return;

  this->spriteData.texture = this->textureManager->getTexture();
  if (color == graphicsNS::FILTER)
    graphics->drawSprite(this->spriteData, this->colorFilter);
  else
    graphics->drawSprite(this->spriteData, color);
}

void Image::draw(SpriteData sd, COLOR_ARGB color)
{
  if (!visible || graphics == NULL) return;
  sd.rect = spriteData.rect;
  sd.texture = textureManager->getTexture();

  if (color == graphicsNS::FILTER)
    this->graphics->drawSprite(sd, colorFilter);
  else
    this->graphics->drawSprite(sd, color);
}

void Image::update(float frameTime)
{
  if (this->endFrame - this->startFrame > 0) {
    this->animTimer += frameTime;
    if (this->animTimer > this->frameDelay) {
      this->animTimer -= this->frameDelay;
      this->currentFrame++;
      if (this->currentFrame < this->startFrame ||
          this->currentFrame > this->endFrame) {
        if (loop) {
          this->currentFrame = this->startFrame;
        } else {
          this->currentFrame = this->endFrame;
          this->animComplete = true;
        }
      }
      setRect();
    }
  }
}

void Image::setCurrentFrame(int c)
{
  if (c >= 0) {
    currentFrame = c;
    animComplete = false;
    setRect();
  }
}

inline void Image::setRect()
{
  spriteData.rect.left = (currentFrame % cols) * spriteData.width;
  // right edge + 1
  spriteData.rect.right = spriteData.rect.left + spriteData.width;
  spriteData.rect.top = (currentFrame / cols) * spriteData.height;
  // bottom edge + 1
  spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}