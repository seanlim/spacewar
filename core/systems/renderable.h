#pragma once

#include "common.h"
#include "ecs.h"
#include "graphics.h"
#include "textureManager.h"

struct CSprite : public Component<CSprite> {
  SpriteData spriteData;
  TextureManager* textureManager;

  int startFrame, endFrame, currentFrame;
  int cols;
  float frameDelay;
  COLOR_ARGB colorFilter;

  bool animates;
  bool loops;
  bool visible;
  bool animComplete;

  void initialise(int width, int height, int ncols, TextureManager* textureM)
  {
    this->spriteData.width = 2, this->spriteData.height = 2;
    spriteData.x, this->spriteData.y = 0.0;
    spriteData.scale = 1.0;
    spriteData.angle = 0.0;
    spriteData.rect.top = 0;
    spriteData.rect.right = this->spriteData.width;
    spriteData.rect.bottom = this->spriteData.height;
    spriteData.texture = NULL;
    spriteData.flipHorizontal = false, this->spriteData.flipVertical = false;

    // startFrame = 0, endFrame = 0, currentFrame = 0;
    cols = ncols;
    animTimer = 0.0;
    frameDelay = 0.05;
    visible = true;
    loops = true;
    colorFilter = graphicsNS::WHITE;
    textureManager = NULL;

    textureManager = textureM;
    spriteData.texture = textureManager->getTexture();

    this->spriteData.width = width;
    if (width == 0) this->spriteData.width = this->textureManager->getWidth();
    this->spriteData.height = height;
    if (height == 0)
      this->spriteData.height = this->textureManager->getHeight();

    int cols = ncols;
    if (ncols == 0) cols = 1;

    setRect();
  }

  float getX() { return spriteData.x; }
  float getY() { return spriteData.y; }
  float getAngle() { return spriteData.angle; }
  float getScale() { return spriteData.scale; }
  const Vec2* getCenter() { return &Vec2(getCenterX(), getCenterY()); }
  void setScale(float n) { spriteData.scale = n; }
  float getHeight() { return spriteData.height * spriteData.scale; }
  float getWidth() { return spriteData.height * spriteData.scale; }

  void setPosition(float _x, float _y) { spriteData.x = _x, spriteData.y = _y; }
  void updateCurrentFrame(float delta)
  {
    if (this->endFrame - this->startFrame > 0) {
      this->animTimer += delta;
      if (this->animTimer > this->frameDelay) {
        this->animTimer -= this->frameDelay;
        this->currentFrame++;
        if (this->currentFrame < this->startFrame ||
            this->currentFrame > this->endFrame) {
          if (loops) {
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
  void setRect()
  {
    spriteData.rect.left = (currentFrame % cols) * spriteData.width;
    // right edge + 1
    spriteData.rect.right = spriteData.rect.left + spriteData.width;
    spriteData.rect.top = (currentFrame / cols) * spriteData.height;
    // bottom edge + 1
    spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
  }

private:
  float animTimer;
  inline float getCenterX()
  {
    return spriteData.x + (spriteData.width / 2) * getScale();
  }
  inline float getCenterY()
  {
    return spriteData.y + (spriteData.height / 2) * getScale();
  }
};

class SRenderable : public System
{
protected:
  Graphics* graphics;

public:
  SRenderable(HWND _hwnd, uint32 width, uint32 height, bool fullscreen,
              Graphics* _graphics)
      : System()
  {
    System::addComponentType(CSprite::id);

    this->graphics = _graphics;
  }
  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CSprite* sprite = (CSprite*)components[0];

    if (sprite->visible) {
      graphics->spriteBegin();
      sprite->spriteData.texture = sprite->textureManager->getTexture();
      graphics->drawSprite(sprite->spriteData);
      graphics->spriteEnd();
    }

    if (sprite->animates) sprite->updateCurrentFrame(delta);
  }
};
