#pragma once

#include "common.h"
#include "constants.h"
#include "ecs.h"
#include "textureManager.h"

struct CEnemy : public Component<CEnemy> {
  bool enabled = false;
  Vec2 enemyPosition;

  Vec2 node;
  // Every point in a path is stored in the nodes vector as a Vec2(x,y)
  Array<Vec2> nodesVector = {};

  void addNode(Vec2 node) { nodesVector.push_back(node); }
  Array<Vec2> getNodes() { return nodesVector; }
  int currentNode = 0;
  int pathDir = -1;
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

    enemy->enemyPosition = Vec2(enemySprite->getX(), enemySprite->getY());

    if (enemy->enabled == true) {
      updateEnemyMotion(delta, enemyMotion, enemy);
      // if (enemyCollider->colliding) {
      //  enemy->enabled = false;
      //}
      if (enemyBulletEmitter->collidingWithBullet == true) {
        enemy->enabled = false;
      }
    } else {
      enemySprite->visible = false;
      enemyMotion->velocity = Vec2(0, 0),
      enemyMotion->acceleration = Vec2(0, 0);
    }
  }

  void updateEnemyMotion(float delta, CMotion* enemyMotion, CEnemy* enemy)
  {
    Vec2 pathVector = pathFollowing(enemy);
    enemyMotion->velocity = (pathVector - enemy->enemyPosition) * delta * 250;
  }

  virtual Vec2 pathFollowing(CEnemy* newEnemy)
  {
    Vec2 target = Vec2();

    // check if there is an path already
    bool isEmpty = newEnemy->nodesVector.empty();

    if (!isEmpty) {
      Array<Vec2> nodesVector =
          newEnemy->getNodes(); // if there is a path, store these nodes in a
                                // vector: nodes
      target =
          nodesVector[newEnemy->currentNode]; // get the current target nodes[0]

      // check distance between enemy position vector and target position vector
      if (distance(newEnemy->enemyPosition, target) <= 100) {
        /*newEnemy->currentNode += 1;*/
        newEnemy->currentNode += newEnemy->pathDir;

        if (newEnemy->currentNode >= nodesVector.size() ||
            newEnemy->currentNode < 0) {
          newEnemy->pathDir *= -1;
          newEnemy->currentNode += newEnemy->pathDir;
        }
      }
    }

    return !isEmpty ? target : Vec2(0, 0);
  }

  // check distance between two vectors
  inline int distance(Vec2 a, Vec2 b)
  {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }
};
