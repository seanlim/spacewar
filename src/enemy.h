#pragma once

#include "common.h"
#include "constants.h"
#include "ecs.h"
#include "textureManager.h"

struct CEnemy : public Component<CEnemy> {
  bool enabled = false;
};

class SEnemy : public System
{
public:
  SEnemy() : System()
  {
    System::addComponentType(CEnemy::id);
    System::addComponentType(CSprite::id);
    System::addComponentType(CMotion::id);
    System::addComponentType(CCollidable::id);
    System::addComponentType(CBulletEmitter::id);
  }

  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CEnemy* enemy = (CEnemy*)components[0];
    CSprite* enemySprite = (CSprite*)components[1];
    CMotion* enemyMotion = (CMotion*)components[2];
    CCollidable* enemyCollider = (CCollidable*)components[3];
    CBulletEmitter* enemyBulletEmitter = (CBulletEmitter*)components[4];

    if (enemy->enabled == true) {
      updateEnemyMotion(enemyMotion);
      // if (enemyCollider->colliding) {
      //   enemy->enabled = false;
      // }
      if (enemyBulletEmitter->collidingWithBullet == true) {
        enemy->enabled = false;
      }
    } else {
      enemySprite->visible = false;
      enemyMotion->velocity = Vec2(0, 0),
      enemyMotion->acceleration = Vec2(0, 0);
    }
  }

  void updateEnemyMotion(CMotion* enemyMotion)
  {
    enemyMotion->velocity = Vec2(0, 100);
  }
};