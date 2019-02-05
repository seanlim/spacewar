#pragma once

#include "common.h"
#include "ecs.h"
#include "math/vector2.h"
#include "systems/physics.h"
#include "systems/renderable.h"

struct CCollidable : public Component<CCollidable> {
public:
  Vec2 corners[4];
  Vec2 edge01, edge03;
  float edge01Min, edge01Max, edge03Min, edge03Max;
  Vec2 distSquared;
  float sumRadiiSquared;
  CollisionType collisionType;
  CollisionResponse collisionResponse;
  Rect edge = {-1, -1, 1, 1};
  uint32 collisionId;
  float radius;
  float angle;
  bool colliding;
  Vec2 center;
  float scale;
  Vec2 velocity;
  float mass;

  bool equals(CCollidable cmp) { return (collisionId == cmp.collisionId); }

  ///////////////////////
  // Collision response//
  ///////////////////////
  Vec2 bounce(CCollidable& c2, Vec2 collisionVector)
  {
    Vec2 Vdiff = c2.velocity - velocity;
    Vec2 cUV = collisionVector;
    Vec2NS::Vector2Normalize(&cUV);
    float cUVdotVdiff = Vec2NS::Vector2Dot(&cUV, &Vdiff);
    float massRatio = 2.0f;
    // if (c1.mass != 0) massRatio *= (c2.mass / (c.mass + c2.mass));

    return ((massRatio * cUVdotVdiff) * cUV);
  }

  //////////////
  // Collision//
  //////////////
  bool collideBox(CCollidable& c2, Vec2& collisionVector)
  {
    if ((center.x + edge.right * scale <
         c2.center.x + c2.edge.left * c2.scale) ||
        (center.x + edge.left * scale >
         c2.center.x + c2.edge.right * c2.scale) ||
        (center.y + edge.bottom * scale <
         c2.center.y + c2.edge.top * c2.scale) ||
        (center.y + edge.top * scale >
         c2.center.y + c2.edge.bottom * c2.scale)) {
      return false;
    }

    collisionVector = c2.center - center;
    return true;
  }

  bool collideCircle(CCollidable& c2, Vec2& collisionVector)
  {
    distSquared = center - c2.center;
    distSquared.x = distSquared.x * distSquared.x;
    distSquared.y = distSquared.y * distSquared.y;

    sumRadiiSquared = (radius * scale) + (c2.radius * c2.scale);
    sumRadiiSquared *= sumRadiiSquared;

    if (distSquared.x + distSquared.y <= sumRadiiSquared) {
      collisionVector = c2.center - center;
      return true;
    }
    return false;
  }

  bool collideRotatedBox(CCollidable& c2, Vec2& collisionVector)
  {
    computeRotatedBox(), c2.computeRotatedBox();
    if (projectionsOverlap(c2) && c2.projectionsOverlap(*this)) {
      collisionVector = c2.center - center;
      return true;
    }
    return false;
  }

  bool collideRotatedBoxCircle(CCollidable& c2, Vec2& collisionVector)
  {
    float min01, min03, max01, max03, center01, center03;

    computeRotatedBox();

    center01 = Vec2NS::Vector2Dot(&edge01, &c2.center);
    min01 = center01 - c2.radius * c2.scale;
    max01 = center01 + c2.radius * c2.scale;
    if (min01 > edge01Max || max01 < edge01Min) return false;

    center03 = Vec2NS::Vector2Dot(&edge03, &c2.center);
    min03 = center03 - c2.radius * c2.scale;
    max03 = center03 + c2.radius * c2.scale;
    if (min03 > edge03Max || max03 < edge03Min) return false;

    if (center01 < edge01Min && center03 < edge03Min)
      return collideCornerCircle(corners[0], c2, collisionVector);
    if (center01 > edge01Max && center03 < edge03Min)
      return collideCornerCircle(corners[1], c2, collisionVector);
    if (center01 > edge01Max && center03 > edge03Max)
      return collideCornerCircle(corners[2], c2, collisionVector);
    if (center01 < edge01Min && center03 > edge03Max)
      return collideCornerCircle(corners[3], c2, collisionVector);

    collisionVector = c2.center - center;
    return true;
  }

  //////////////////////
  // Collision helpers//
  //////////////////////
  const Vec2* getCorner(uint32 c) const
  {
    if (c >= 4) c = 0;
    return &corners[c];
  }

  bool projectionsOverlap(CCollidable& c)
  {
    float projection, min01, max01, min03, max03;

    projection = Vec2NS::Vector2Dot(&edge01, c.getCorner(0));
    min01 = projection;
    max01 = projection;
    for (int i = 1; i < 4; i++) {
      projection = Vec2NS::Vector2Dot(&edge01, c.getCorner(i));
      if (projection < min01)
        min01 = projection;
      else if (projection > max01)
        max01 = projection;
    }
    if (min01 > edge01Max || max01 < edge01Min) return false;

    projection = Vec2NS::Vector2Dot(&edge03, c.getCorner(0));
    min03 = projection;
    max03 = projection;
    for (int i = 1; i < 4; i++) {
      projection = Vec2NS::Vector2Dot(&edge03, c.getCorner(i));
      if (projection < min03)
        min03 = projection;
      else if (projection > max03)
        max03 = projection;
    }
    if (min03 > edge03Max || max03 < edge03Min) return false;

    return true;
  }
  bool collideCornerCircle(Vec2 corner, CCollidable& c, Vec2& collisionVector)
  {
    distSquared = corner - c.center;
    distSquared.x = distSquared.x * distSquared.x;
    distSquared.y = distSquared.y * distSquared.y;

    sumRadiiSquared = c.radius * c.scale;
    sumRadiiSquared *= sumRadiiSquared;

    if (distSquared.x + distSquared.y <= sumRadiiSquared) {
      collisionVector = c.center - corner;
      return true;
    }
    return false;
  }
  void computeRotatedBox()
  {
    float projection;

    Vec2 rotatedX(cos(angle), sin(angle));
    Vec2 rotatedY(-sin(angle), cos(angle));

    corners[0] = center + rotatedX * ((float)edge.left * scale) +
                 rotatedY * ((float)edge.top * scale);
    corners[1] = center + rotatedX * ((float)edge.right * scale) +
                 rotatedY * ((float)edge.top * scale);
    corners[2] = center + rotatedX * ((float)edge.right * scale) +
                 rotatedY * ((float)edge.bottom * scale);
    corners[3] = center + rotatedX * ((float)edge.left * scale) +
                 rotatedY * ((float)edge.bottom * scale);

    edge01 = Vec2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
    Vec2NS::Vector2Normalize(&edge01);
    edge03 = Vec2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
    Vec2NS::Vector2Normalize(&edge03);

    projection = Vec2NS::Vector2Dot(&edge01, &corners[0]);
    edge01Min = projection;
    edge01Max = projection;

    projection = Vec2NS::Vector2Dot(&edge01, &corners[1]);
    if (projection < edge01Min)
      edge01Min = projection;
    else if (projection > edge01Max)
      edge01Max = projection;

    projection = Vec2NS::Vector2Dot(&edge03, &corners[0]);
    edge03Min = projection;
    edge03Max = projection;
    projection = Vec2NS::Vector2Dot(&edge03, &corners[3]);
    if (projection < edge03Min)
      edge03Min = projection;
    else if (projection > edge03Max)
      edge03Max = projection;
  }
};

class SCollision : public System
{

  Array<CCollidable> collisionComponents; // Keep local cache of collision
                                          // components for random access in
                                          // collision detection
  Rect collisionBounds{0, 0, GAME_WIDTH,
                       GAME_HEIGHT}; // Bounds that desribe the simulation area

public:
  SCollision() : System()
  {
    System::addComponentType(CSprite::id);
    System::addComponentType(CMotion::id);
    System::addComponentType(CCollidable::id);
  }

  void setBounds(Rect _collisionBounds) { collisionBounds = _collisionBounds; }

  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CSprite* sprite = (CSprite*)components[0];
    CMotion* motion = (CMotion*)components[1];
    CCollidable* collidable = (CCollidable*)components[2];

    // Inform CComponent about sprite
    collidable->radius = sprite->getWidth() / 2;
    collidable->angle = sprite->getAngle();
    collidable->center = *sprite->getCenter();
    collidable->scale = sprite->getScale();
    collidable->edge = {
        -(sprite->spriteData.width / 2), -(sprite->spriteData.height / 2),
        (sprite->spriteData.width / 2), (sprite->spriteData.height / 2)};
    // Inform CComponent about motion
    collidable->velocity = motion->velocity;

    ////////////////////////////
    // Update CCollision cache//
    ////////////////////////////
    int componentIndex = -1;

    if (collisionComponents.size() > 0) {
      for (int i = 0; i < collisionComponents.size(); i++) {
        if (collisionComponents.at(i).equals(*collidable)) {
          componentIndex = i;
          collisionComponents[i] = *collidable;
        }
      }
    }

    if (componentIndex == -1) {
      collidable->collisionId = collisionComponents.size();
      collisionComponents.push_back(*collidable);
    } else {
      ////////////////////////
      // Collision Detection//
      ////////////////////////
      for (int i = 0; i < collisionComponents.size(); i++) {
        if (i == componentIndex) continue;

        bool didCollide;
        Vec2 collisionVector = Vec2(0.0, 0.0);
        CCollidable collidable2 = collisionComponents.at(i);

        if (collidable->collisionType == CollisionType::CIRCLE &&
            collidable2.collisionType == CollisionType::CIRCLE) {
          didCollide = collidable->collideCircle(collidable2, collisionVector);
        }

        else if (collidable->collisionType == CollisionType::BOX &&
                 collidable2.collisionType == CollisionType::BOX) {
          didCollide = collidable->collideBox(collidable2, collisionVector);
        }

        else if (collidable->collisionType != CollisionType::CIRCLE &&
                 collidable2.collisionType != CollisionType::CIRCLE)
          didCollide =
              collidable->collideRotatedBox(collidable2, collisionVector);

        else if (collidable->collisionType == CollisionType::CIRCLE) {
          didCollide =
              collidable2.collideRotatedBoxCircle(*collidable, collisionVector);
          collisionVector *= -1;
        } else {
          didCollide =
              collidable->collideRotatedBoxCircle(collidable2, collisionVector);
        }

        ///////////////////////
        // Collision Response//
        ///////////////////////
        if (collidable->collisionResponse == BOUNCE) {
          motion->collidedDelta =
              collidable->bounce(collidable2, collisionVector);
        } else if (collidable->collisionResponse == NONE) {
          motion->collidedDelta = Vec2(0.0, 0.0);
        }

        /////////////////////////////////
        // Edge Detection (Bounds)//
        /////////////////////////////////
        if (sprite->getX() >= collisionBounds.right - sprite->getWidth()) {
          didCollide = true;
          motion->collidedDelta.x = -2 * collidable->velocity.x;
          sprite->spriteData.x = collisionBounds.right - sprite->getWidth();
        } else if (sprite->getX() <= collisionBounds.left) {
          didCollide = true;
          motion->collidedDelta.x = -2 * collidable->velocity.x;
          sprite->spriteData.x = collisionBounds.left;
        }
        if (sprite->getY() >= collisionBounds.bottom - sprite->getHeight()) {
          didCollide = true;
          motion->collidedDelta.y = -2 * collidable->velocity.y;
          sprite->spriteData.y = collisionBounds.bottom - sprite->getHeight();
        } else if (sprite->getY() <= collisionBounds.top) {
          didCollide = true;
          motion->collidedDelta.y = -2 * collidable->velocity.y;
          sprite->spriteData.y = collisionBounds.top;
        }

        motion->colliding =
            didCollide; // Signal to motion system to apply simulated force
      }
    }
  }
};