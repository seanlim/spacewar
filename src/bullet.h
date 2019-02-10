#pragma once

#include "common.h"
#include "ecs.h"
#include "input.h"

// Create bulletEmitted component
struct CBulletEmitter : Component<CBulletEmitter> {
  char* emitterID;
  bool collidingWithBullet;
  bool firing = false;
  float firingRate = 1.0;
  Vec2 velocity;
  Vec2 acceleration;
};

// Bullet system that simulates bullets
class SBullet : public System
{
  struct Bullet {
    char* emitterID;
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
    System::addComponentType(CCollidable::id);

    this->graphics = _graphics;
    this->game = _game;
  }
  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CBulletEmitter* bulletEmitter = (CBulletEmitter*)components[0];
    CSprite* sprite = (CSprite*)components[1];
    CCollidable* collider = (CCollidable*)components[2];

    if (bulletEmitter->firing == true) {
      bulletEmitter->firing = false;
      // Store bullet
      Bullet newBullet = Bullet();
      newBullet.emitterID = bulletEmitter->emitterID;
      newBullet.velocity = bulletEmitter->velocity;
      newBullet.acceleration = bulletEmitter->acceleration;
      newBullet.position = Vec2(sprite->getX() + sprite->getWidth() / 2,
                                sprite->getY() - sprite->getHeight() / 2);

      // Fire bullet
      bullets.push_back(newBullet);
    }

    // Update all bullets
    for (int i = 0; i < bullets.size(); i++) {
      // Apply motion
      bullets[i].velocity += bullets[i].acceleration * delta;
      bullets[i].position += bullets[i].velocity * delta;

      bulletSprite.setPosition(bullets[i].position.x, bullets[i].position.y);

      // Check to delete bullet
      if (bullets[i].position.x < 0 || bullets[i].position.x > GAME_WIDTH ||
          bullets[i].position.y < 0 || bullets[i].position.y > GAME_HEIGHT) {
        removeBullet(i);
      }

      else if (bullets[i].emitterID != bulletEmitter->emitterID) {
        // Check collision
        CCollidable
            bulletCollider; // We implicity know that this is a circle collider,
                            // don't need to specify since this collidable
                            // component is not actually registered
        bulletCollider.angle = bulletSprite.getAngle();
        bulletCollider.center = *bulletSprite.getCenter();
        bulletCollider.scale = bulletSprite.getScale();
        bulletCollider.radius = bulletSprite.spriteData.width / 2;
        // bulletCollider.edge = {-(bulletSprite.spriteData.width / 2),
        //                        -(bulletSprite.spriteData.height / 2),
        //                        (bulletSprite.spriteData.width / 2),
        //                        (bulletSprite.spriteData.height / 2)};

        Vec2 collisionVector = Vec2(0, 0);
        if (collider->collideCircle(bulletCollider, collisionVector) == true) {
          removeBullet(i);
          bulletEmitter->collidingWithBullet = true;
        }
      }

      graphics->spriteBegin();
      bulletSprite.spriteData.texture =
          bulletSprite.textureManager->getTexture();
      graphics->drawSprite(bulletSprite.spriteData);
      graphics->spriteEnd();
    }

	/*If bullet collides with playerShip. Example Code*/
	/*
	if (collide)
	{
		int currentHealth = healthbar + 1;
		healthbar = currentHealth;
	}
	*/
  }

  inline void removeBullet(int index)
  {
    bullets[index] = bullets.back();
    bullets.resize(bullets.size() - 1);
  }
};