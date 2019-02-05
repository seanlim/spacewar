#include "ship.h"

Ship::Ship() : Entity()
{
  spriteData.width = shipNS::WIDTH; // size of Ship1
  spriteData.height = shipNS::HEIGHT;
  spriteData.x = shipNS::X; // location on screen
  spriteData.y = shipNS::Y;
  spriteData.rect.bottom =
      shipNS::HEIGHT; // rectangle to select parts of an image
  spriteData.rect.right = shipNS::WIDTH;
  velocity.x = 0; // velocity X
  velocity.y = 0; // velocity Y
  frameDelay = shipNS::SHIP_ANIMATION_DELAY;
  startFrame = shipNS::SHIP1_START_FRAME; // first frame of ship animation
  endFrame = shipNS::SHIP1_END_FRAME;     // last frame of ship animation
  currentFrame = startFrame;
  radius = shipNS::WIDTH / 2.0;
  shieldOn = false;
  mass = shipNS::MASS;
  collisionType = entityNS::CIRCLE;
}

bool Ship::initialise(Game* game, int width, int height, int ncols,
                      TextureManager* textureManager)
{
  shield.initialise(game->getGraphics(), width, height, ncols, textureManager);
  shield.setFrames(shipNS::SHIELD_START_FRAME, shipNS::SHIELD_END_FRAME);
  shield.setCurrentFrame(shipNS::SHIELD_START_FRAME);
  shield.setFrameDelay(shipNS::SHIELD_ANIMATION_DELAY);
  shield.setLoop(false); // do not loop animation
  return (Entity::initialise(game, width, height, ncols, textureManager));
}

void Ship::draw()
{
  Image::draw(); // draw ship
  if (shieldOn)
    // draw shield using colorFilter 50% alpha
    shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
}

void Ship::update(float frameTime)
{
  Entity::update(frameTime);
  spriteData.angle += frameTime * shipNS::ROTATION_RATE; // rotate the ship
  spriteData.x += frameTime * velocity.x;                // move ship along X
  spriteData.y += frameTime * velocity.y;                // move ship along Y

  // Bounce off walls
  if (spriteData.x > GAME_WIDTH - shipNS::WIDTH) // if hit right screen edge
  {
    spriteData.x = GAME_WIDTH - shipNS::WIDTH; // position at right screen edge
    velocity.x = -velocity.x;                  // reverse X direction
  } else if (spriteData.x < 0)                 // else if hit left screen edge
  {
    spriteData.x = 0;         // position at left screen edge
    velocity.x = -velocity.x; // reverse X direction
  }
  if (spriteData.y > GAME_HEIGHT - shipNS::HEIGHT) // if hit bottom screen edge
  {
    spriteData.y =
        GAME_HEIGHT - shipNS::HEIGHT; // position at bottom screen edge
    velocity.y = -velocity.y;         // reverse Y direction
  } else if (spriteData.y < 0)        // else if hit top screen edge
  {
    spriteData.y = 0;         // position at top screen edge
    velocity.y = -velocity.y; // reverse Y direction
  }
  if (shieldOn) {
    shield.update(frameTime);
    if (shield.getAnimationComplete()) {
      shieldOn = false;
      shield.setAnimationComplete(false);
    }
  }
}

void Ship::damage(WEAPON weapon) { shieldOn = true; }
