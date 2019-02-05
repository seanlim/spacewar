#include "spacewar.h"

SpaceWar::SpaceWar() {}

SpaceWar::~SpaceWar() { this->releaseAll(); }

void SpaceWar::initialise(HWND hwnd)
{
  Game::initialise(hwnd);

  // load textures
  if (!gameTexture.initialise(graphics, TEXTURES_IMAGE))
    throw(
        GameError(gameErrorNS::FATAL_ERROR, "Error initialising game texture"));

  // Nebula
  // Init texture
  if (!nebulaTexture.initialise(graphics, NEBULA_IMAGE))
    throw(GameError(gameErrorNS::FATAL_ERROR,
                    "Error initialising nebula texture"));

  // if (!nebulaImage.initialise(graphics, 0, 0, 0, &nebulaTexture))
  //   throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
  // nebulaImage.setX(0.0);
  // nebulaImage.setY(0.0);

  // if (!nebulaImage.initialise(graphics, 0, 0, 0, &nebulaTexture)) return;

  if (!planet.initialise(this, planetNS::WIDTH, planetNS::HEIGHT, 2,
                         &gameTexture))
    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialising planet"));
  // ship1
  if (!ship1.initialise(this, shipNS::WIDTH, shipNS::HEIGHT,
                        shipNS::TEXTURE_COLS, &gameTexture))
    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
  ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
  ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
  ship1.setX(GAME_WIDTH / 4);
  ship1.setY(GAME_HEIGHT / 4);
  ship1.setVelocity(Vec2(shipNS::SPEED, -shipNS::SPEED)); // Vec2(X, Y)
  // ship2
  if (!ship2.initialise(this, shipNS::WIDTH, shipNS::HEIGHT,
                        shipNS::TEXTURE_COLS, &gameTexture))
    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));
  ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
  ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);
  ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);
  ship2.setY(GAME_HEIGHT / 4);
  ship2.setVelocity(Vec2(-shipNS::SPEED, -shipNS::SPEED)); // Vec2(X, Y)

  CSprite nebulaImage;
  nebulaImage.initialise(GAME_WIDTH, GAME_HEIGHT, 1, &nebulaTexture);
  ecs.makeEntity(nebulaImage);

  CSprite spaceShipSprite;
  spaceShipSprite.initialise(shipNS::WIDTH, shipNS::HEIGHT,
                             shipNS::TEXTURE_COLS, &gameTexture);
  spaceShipSprite.startFrame = shipNS::SHIP1_START_FRAME,
  spaceShipSprite.endFrame = shipNS::SHIP1_END_FRAME;
  spaceShipSprite.currentFrame = shipNS::SHIP1_START_FRAME;
  spaceShipSprite.spriteData.x = GAME_WIDTH / 2;
  spaceShipSprite.spriteData.y = GAME_HEIGHT / 2;

  CMotion spaceShipMotion;
  ecs.makeEntity(spaceShipSprite, spaceShipMotion);

  return;
}

void SpaceWar::update()
{
  planet.update(frameTime);
  ship1.update(frameTime);
  ship2.update(frameTime);
  //
  /////
  ///////
}

void SpaceWar::ai() {}

void SpaceWar::collisions()
{
  Vec2 collisionVector;
  // if collision between ship and planet
  if (ship1.collidesWith(planet, collisionVector)) {
    // bounce off planet
    ship1.bounce(collisionVector, planet);
    ship1.damage(PLANET);
  }
  if (ship2.collidesWith(planet, collisionVector)) {
    // bounce off planet
    ship2.bounce(collisionVector, planet);
    ship2.damage(PLANET);
  }
  // if collision between ships
  if (ship1.collidesWith(ship2, collisionVector)) {
    // bounce off ship
    ship1.bounce(collisionVector, ship2);
    ship1.damage(SHIP);
    // change the direction of the collisionVector for ship2
    ship2.bounce(collisionVector * -1, ship1);
    ship2.damage(SHIP);
  }
}

void SpaceWar::render()
{
  this->graphics->spriteBegin();
  // this->nebulaImage.draw();
  this->planet.draw();
  this->ship1.draw();
  this->ship2.draw();
  this->graphics->spriteEnd();
}

void SpaceWar::releaseAll()
{
  this->nebulaTexture.onLostDevice();
  this->gameTexture.onLostDevice();
  Game::releaseAll();
  return;
}

void SpaceWar::resetAll()
{
  this->nebulaTexture.onResetDevice();
  this->gameTexture.onLostDevice();
  Game::resetAll();
  return;
}
