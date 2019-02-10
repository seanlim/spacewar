#pragma once

#include "common.h"
#include "ecs.h"
#include "graphics.h"
#include "systems/renderable.h"

struct Particle {
  Vec2 position;
  CSprite textureSprite;
  float life;
  COLOR_ARGB color;
};

struct CEmitter : public Component<CEmiiter> {
  Vec2 position;
  CSprite textureSprite;
  uint32 emissionRate;
  float lifeSpan;

  COLOR_ARGB startColor;
  COLOR_ARGB endColor;
  float startAlpha;
  float endAlpha;

private:
};

class SParticle : public System
{
  Graphics* graphics;
  Array<uint32> particlePool;

public:
  SPhysics : System(Graphics* _graphics)
  {
    System::addComponentType(CEmitter::id);
    this->graphics = _graphics;
  }
  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CEmiiter* emitter = (CEmitter*)components[0];

    // Draw
    graphics->spriteBegin();

    graphics->spriteEnd();
  }
};