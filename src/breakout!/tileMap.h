#pragma once

#include "common.h"
#include "constants.h"
#include "ecs.h"
#include "textureManager.h"

struct CTileMapCollider : public Component<CTileMapCollider> {
public:
};

class SBreakOutTileMap : public System
{
private:
  Graphics* graphics;
  int tileMap[columns][rows]; // row, column
public:
  CSprite tileSprite;
  SBreakOutTileMap(HWND _hwnd, uint32 width, uint32 height, bool fullscreen,
                   Graphics* _graphics)
      : System()
  {
    System::addComponentType(CMotion::id);
    System::addComponentType(CCollidable::id);
    System::addComponentType(CTileMapCollider::id);
    this->graphics = _graphics;
    setTileMap(1);
  }

  inline void setTileMap(int value)
  {
    for (int row = 0; row < rows; row++) {
      for (int column = 0; column < columns; column++) {
        tileMap[row][column] = 1;
      }
    }
  }

  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CMotion* motion = (CMotion*)components[0];
    CCollidable* collider = (CCollidable*)components[1];
    CTileMapCollider* tileMapCollider = (CTileMapCollider*)components[2];

    // Draw tilemap
    for (int column = 0; column < columns; column++) {
      for (int row = 0; row < rows; row++) {

        tileSprite.setPosition((marginX + (column * tileSize)),
                               (row * tileSize));

        if (tileMap[row][column] == 1) {
          graphics->spriteBegin();
          tileSprite.spriteData.texture =
              tileSprite.textureManager->getTexture();
          graphics->drawSprite(tileSprite.spriteData);
          graphics->spriteEnd();

          // Check collision
          CCollidable
              mapCollider; // We implicity know that this is a box collider,
                           // don't need to specify since this collidable
                           // component is not actually registered
          mapCollider.angle = tileSprite.getAngle();
          mapCollider.center = *tileSprite.getCenter();
          mapCollider.scale = tileSprite.getScale();
          mapCollider.edge = {-(tileSprite.spriteData.width / 2),
                              -(tileSprite.spriteData.height / 2),
                              (tileSprite.spriteData.width / 2),
                              (tileSprite.spriteData.height / 2)};
          Vec2 collisionVector = Vec2(0, 0);
          if (collider->collideBox(mapCollider, collisionVector) == true) {
            motion->collidedDelta =
                collider->bounce(mapCollider, collisionVector);
            motion->colliding = true;

            // Disable tile
            tileMap[row][column] = 0;
          }
        }
      }
    }
  }
};
