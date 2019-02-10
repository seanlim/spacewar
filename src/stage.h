#pragma once

#include "bullet.h"
#include "common.h"
#include "constants.h"
#include "input.h"
#include "math/lerp.h"
#include "playerControlled.h"
#include "scene.h"

class Stage : public Scene
{
  TextureManager backgroundTexture, planetTexture, shipTexture, bulletTexture, healthTexture;

  SPlayerControlled* playerControlSystem;
  SBullet* bulletSystem;

  CSprite backgroundSprite, planetSprite, shipSprite, bulletSprite, healthSprite;
  CMotion shipMotion;
  CAnimated planetAnimation, shipAnimation;
  CPlayerControlled playerControlled;
  CCollidable shipCollider;
  CBulletEmitter shipBulletEmitter;

  EntityHook backgroundEntity, planetEntity, shipEntity, healthEntity;

  int* selectedShip;
  int healthBar;

public:
	Stage(int* _selectedShip) : Scene() { this->selectedShip = _selectedShip;}
  ~Stage();

  void setupSystems()
  {
    playerControlSystem = new SPlayerControlled(input);
    bulletSystem = new SBullet(graphics, game);
  }
  void setupTextures()
  {
    if (!backgroundTexture.initialise(graphics, BACKGROUND))
      Logger::error("Failed to load background texture");
    if (!planetTexture.initialise(graphics, PLANET))
      Logger::error("Failed to  load planet texture");
    if (!shipTexture.initialise(graphics, SHIPS))
      Logger::error("Failed to load ship textures ");
    if (!bulletTexture.initialise(graphics, BULLET))
      Logger::error("Failed to load bullet texture");
	if (!healthTexture.initialise(graphics, HEALTH))
		Logger::error("Failed to load health texture");
  }

  void setupComponents()
  {

    // Background
    backgroundSprite.startFrame = 0, backgroundSprite.endFrame = 0,
    backgroundSprite.currentFrame = 0;
    backgroundSprite.animates = false;
    backgroundSprite.initialise(BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
                                BACKGROUND_COLS, &backgroundTexture);
    backgroundSprite.setScale(0.5);
    backgroundSprite.setPosition(0, 0);

	// Health
	healthSprite.startFrame = 0, healthSprite.endFrame = 6,
		healthSprite.currentFrame = 0; 
	healthSprite.animates = false;
	healthSprite.initialise(HEALTH_WIDTH, HEALTH_HEIGHT,
		HEALTH_COLS, &healthTexture);
	healthSprite.setScale(0.7);
	healthSprite.setPosition(0, GAME_HEIGHT - healthSprite.getHeight());

    // Planet
    planetSprite.startFrame = 0, planetSprite.endFrame = 5,
    planetSprite.currentFrame = 0;
    planetSprite.initialise(PLANET_WIDTH, PLANET_HEIGHT, PLANET_COLS,
                            &planetTexture);
    planetSprite.setScale(0);
    planetSprite.setPosition(GAME_WIDTH / 2 - planetSprite.getWidth() / 2, 700);
    planetSprite.frameDelay = 5.0;
    planetSprite.animates = true;
    planetSprite.alpha = 0.9;
    planetSprite.alpha = 0;

	/*
    planetAnimation.animations.push_back(
        {ROTATE, 0.0, 2 * PI, 0.00001, false, false, true});
    planetAnimation.animations.push_back(
        {ALPHA, 0.0, 0.8, 0.02, false, false, false});
    planetAnimation.animations.push_back(
        {SCALE, 0.0, 2, 0.02, false, false, false});
	*/
    // Ship
    shipSprite.startFrame = 0, shipSprite.endFrame = 9,
    shipSprite.currentFrame = *selectedShip;
    shipSprite.animates = false;
    shipSprite.initialise(SHIPS_WIDTH, SHIPS_HEIGHT, SHIPS_COLS, &shipTexture);
    shipSprite.setScale(1.5);
    shipSprite.setPosition(GAME_WIDTH / 2 - shipSprite.getWidth() / 2,
                           (GAME_HEIGHT / 2 - shipSprite.getHeight() / 2) + 70);
    shipMotion.velocity = Vec2(0, 0);
    shipBulletEmitter.firingRate = 1.0;
    playerControlled.speed = 80;
    playerControlled.sensitivity = 25;
//    shipAnimation.animations.push_back(
  //      {SCALE, 1.5, 3.0, 0.05, true, false, false});
    shipCollider.collisionType = BOX;
    shipCollider.collisionResponse = NONE;
    shipBulletEmitter.acceleration = Vec2(0.0, -50);
    shipBulletEmitter.velocity = Vec2(0.0, -300);

    bulletSprite.startFrame = 0, bulletSprite.endFrame = 1,
    bulletSprite.currentFrame = 0;
    bulletSprite.animates = false;
    bulletSprite.initialise(BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLS,
                            &bulletTexture);
    bulletSprite.setScale(1.5);
    bulletSystem->bulletSprite = bulletSprite;
  }

  void render() 
  {
	  graphics->spriteBegin();
	  healthSprite.spriteData.texture = healthSprite.textureManager->getTexture();
	  graphics->drawSprite(healthSprite.spriteData);
	  graphics->spriteEnd();
  }
  void update(float delta) 
  {
	  healthSprite.currentFrame = healthBar;
	  healthSprite.setRect();
  }

  void attach()
  {
    gameSystems->addSystem(*playerControlSystem);
    backgroundEntity = ecs->makeEntity(backgroundSprite);
    planetEntity = ecs->makeEntity(planetSprite, planetAnimation);
    shipEntity = ecs->makeEntity(shipSprite, shipBulletEmitter, shipAnimation,
                                 shipMotion, shipCollider, playerControlled);
    graphicsSystems->addSystem(*bulletSystem);
    Scene::attach();
  }
  void detach()
  {
    graphicsSystems->removeSystem(*bulletSystem);
    gameSystems->removeSystem(*playerControlSystem);
    ecs->removeEntity(backgroundEntity);
    ecs->removeEntity(planetEntity);
    ecs->removeEntity(shipEntity);
    delete playerControlSystem;
    delete bulletSystem;
    Scene::detach();
  }
};