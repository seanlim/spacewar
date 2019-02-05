#pragma once
#define WIN32_LEAN_AND_MEAN

#include "common.h"
#include "game.h"
#include "image.h"
#include "input.h"

namespace entityNS
{
enum COLLISION_TYPE { NONE, CIRCLE, BOX, ORIENTED_BOX };
const float GRAVITY = 6.67428e-11f;
} // namespace entityNS

class Entity : public Image
{
protected:
  entityNS::COLLISION_TYPE collisionType;
  Vec2 center;
  float radius;
  Vec2 distSquared;
  float sumRadiiSquared;
  // edge specifies the collision box relative to the center of the entity.
  // left and top are typically negative numbers
  RECT edge;
  Vec2 corners[4];
  Vec2 edge01, edge03;
  float edge01Min, edge01Max, edge03Min, edge03Max;
  Vec2 velocity;
  Vec2 deltaV;
  float mass;
  float health;
  float rr;
  float force;
  float gravity;
  Input* input;
  HRESULT hr;
  bool active;
  bool rotatedBoxReady;
  // Circular collision detection
  // Pre: &ent = Other entity
  // Post: &collisionVector contains collision vector
  virtual bool collideCircle(Entity& ent, Vec2& collisionVector);
  // Axis aligned box collision detection
  // Pre: &ent = Other entity
  // Post: &collisionVector contains collision vector
  virtual bool collideBox(Entity& ent, Vec2& collisionVector);
  // Separating axis collision detection between boxes
  // Pre: &ent = Other entity
  // Post: &collisionVector contains collision vector
  virtual bool collideRotatedBox(Entity& ent, Vec2& collisionVector);
  // Separating axis collision detection between box and circle
  // Pre: &ent = Other entity
  // Post: &collisionVector contains collision vector
  virtual bool collideRotatedBoxCircle(Entity& ent, Vec2& collisionVector);
  // Separating axis collision detection helper functions
  void computeRotatedBox();
  bool projectionsOverlap(Entity& ent);
  bool collideCornerCircle(Vec2 corner, Entity& ent,
                           Vec2& collisionVector);

public:
  Entity();
  virtual ~Entity() {}

  // Getters

  // Return center of scaled Entity as screen x,y.
  virtual const Vec2* getCenter()
  {
    center = Vec2(getCenterX(), getCenterY());
    return &center;
  }

  // Return radius of collision circle.
  virtual float getRadius() const { return radius; }

  // Return RECT structure used for BOX and ROTATED_BOX collision detection.
  virtual const RECT& getEdge() const { return edge; }

  // Return corner c of ROTATED_BOX
  virtual const Vec2* getCorner(UINT c) const
  {
    if (c >= 4) c = 0;
    return &corners[c];
  }

  // Return velocity vector.
  virtual const Vec2 getVelocity() const { return velocity; }

  // Return active.
  virtual bool getActive() const { return active; }

  // Return mass.
  virtual float getMass() const { return mass; }

  // Return gravitational constant.
  virtual float getGravity() const { return gravity; }

  // Return health;
  virtual float getHealth() const { return health; }

  // Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
  virtual entityNS::COLLISION_TYPE getCollisionType() { return collisionType; }

  // Setters
  virtual void setVelocity(Vec2 v) { velocity = v; }

  // Set delta velocity. Added to velocity in update().
  virtual void setDeltaV(Vec2 dv) { deltaV = dv; }

  virtual void setActive(bool a) { active = a; }

  virtual void setHealth(float h) { health = h; }

  virtual void setMass(float m) { mass = m; }

  virtual void setGravity(float g) { gravity = g; }

  virtual void setCollisionRadius(float r) { radius = r; }

  // Update every frame
  // frameTime is used to regulate the speed of movement and animation
  virtual void update(float frameTime);

  virtual bool initialise(Game* game, int width, int height, int nCols,
                          TextureManager* textureManager);

  virtual void activate();

  virtual void ai(float frameTime, Entity& ent);

  virtual bool outsideRect(RECT rect);

  virtual bool collidesWith(Entity& ent, Vec2& collisionVector);

  virtual void damage(int weapon);

  void bounce(Vec2& collisionVector, Entity& ent);

  void gravityForce(Entity* other, float frameTime);
};