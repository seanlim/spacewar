#pragma once

#include "common.h"
#include "ecs.h"
#include "input.h"

// Create bulletEmitted component
struct CBulletEmitter : Component<CBulletEmitter> {
  bool firing = false;
  float firingRate = 1.0;
  Vec2 velocity;
  Vec2 acceleration;
};

// Bullet system that simulates bullets
class SBullet : public System
{
  struct Bullet {
    Vec2 acceleration = Vec2(0.0, -10);
    Vec2 velocity;
    Vec2 position;
  };
  Graphics* graphics;
  Game* game;

  Array<Bullet> bullets;

public:
  CSprite bulletSprite;
  SBullet(Graphics* _graphics, Game* _game) : System()
  {
    System::addComponentType(CBulletEmitter::id);
    System::addComponentType(CSprite::id);

    this->graphics = _graphics;
    this->game = _game;
  }
  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CBulletEmitter* bulletEmitter = (CBulletEmitter*)components[0];
    CSprite* sprite = (CSprite*)components[1];

    if (bulletEmitter->firing == true) {
      bulletEmitter->firing = false;
      // Store bullet
      Bullet newBullet = Bullet();
      newBullet.velocity = bulletEmitter->velocity;
      newBullet.acceleration = bulletEmitter->acceleration;
      newBullet.position =
          Vec2(sprite->getX(), sprite->getY() - sprite->getHeight() / 2);

      // Fire bullet
      bullets.push_back(newBullet);
    }

    // Update all bullets
    for (int i = 0; i < bullets.size(); i++) {
      bullets[i].velocity += bullets[i].acceleration * delta;
      bullets[i].position += bullets[i].velocity * delta;
      bulletSprite.setPosition(bullets[i].position.x, bullets[i].position.y);

      // Check to delete bullet
      if (bullets[i].position.x < 0 || bullets[i].position.x > GAME_WIDTH ||
          bullets[i].position.y < 0 || bullets[i].position.y > GAME_HEIGHT) {
        // Manage memory
        bullets[i] = bullets.back();
        bullets.resize(bullets.size() - 1);
      }

      graphics->spriteBegin();
      bulletSprite.spriteData.texture =
          bulletSprite.textureManager->getTexture();
      graphics->drawSprite(bulletSprite.spriteData);
      graphics->spriteEnd();
    }
  }
};