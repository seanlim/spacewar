#pragma once

#include "bullet.h"
#include "common.h"
#include "constants.h"
#include "enemy.h"
#include "input.h"
#include "math/lerp.h"
#include "playerControlled.h"
#include "scene.h"

class Stage : public Scene
{
  TextureManager backgroundTexture, planetTexture, shipTexture, bulletTexture, healthTexture,
      enemyTexture, enemy2Texture, enemy3Texture;

  SPlayerControlled* playerControlSystem;
  SEnemy* enemySystem;
  SBullet* bulletSystem;

  CSprite backgroundSprite, planetSprite, shipSprite, bulletSprite, healthSprite, enemySprite,
      enemy2Sprite, enemy3Sprite;
  CMotion shipMotion, enemyMotion;

  CAnimated planetAnimation, shipAnimation;
  CPlayerControlled playerControlled;
  CCollidable shipCollider, enemyCollider;
  CBulletEmitter shipBulletEmitter, enemyBulletEmitter;
  CEnemy enemy;

  EntityHook backgroundEntity, planetEntity, shipEntity, healthEntity;

  const int numberOfEnemies = 5;
  Array<EntityHook> enemyHooks = {};

  int* selectedShip;
  int healthBar;

public:
	Stage(int* _selectedShip) : Scene() { this->selectedShip = _selectedShip;}
  ~Stage();

  void setupSystems()
  {
    enemySystem = new SEnemy();
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
    if (!enemyTexture.initialise(graphics, ENEMY_ONE))
      Logger::error("Failed to load enemy texture");
    if (!enemy2Texture.initialise(graphics, ENEMY_TWO))
      Logger::error("Failed to load enemy2 texture");
    if (!enemy3Texture.initialise(graphics, ENEMY_THREE))
      Logger::error("Failed to load enemy3 texture");
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

    planetAnimation.animations.push_back(
        {ROTATE, 0.0, 2 * PI, 0.00001, false, false, true});
    planetAnimation.animations.push_back(
        {ALPHA, 0.0, 0.8, 0.02, false, false, false});
    planetAnimation.animations.push_back(
        {SCALE, 0.0, 2, 0.02, false, false, false});

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
    shipAnimation.animations.push_back(
        {SCALE, 1.5, 3.0, 0.05, true, false, false});
    shipCollider.collisionType = BOX;
    shipCollider.collisionResponse = NONE;
    shipBulletEmitter.emitterID = "ship";
    shipBulletEmitter.acceleration = Vec2(0.0, -50);
    shipBulletEmitter.velocity = Vec2(0.0, -300);

    // Bullet
    bulletSprite.startFrame = 0, bulletSprite.endFrame = 1,
    bulletSprite.currentFrame = 0;
    bulletSprite.animates = false;
    bulletSprite.initialise(BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLS,
                            &bulletTexture);
    bulletSprite.setScale(0.3);
    bulletSystem->bulletSprite = bulletSprite;

    // Enemies
    enemySprite.startFrame = 0, enemySprite.endFrame = 1,
    enemySprite.currentFrame = 0;
    enemySprite.initialise(ENEMY_WIDTH, ENEMY_WIDTH, ENEMY_COLS, &enemyTexture);
    enemySprite.setScale(2.5);
    // Hardcoding position
    enemySprite.setPosition(GAME_WIDTH / 2 - enemySprite.getWidth() / 2,
                            -enemySprite.getHeight());
    enemySprite.animates = true;

    enemy2Sprite.startFrame = 0, enemy2Sprite.endFrame = 1,
    enemy2Sprite.currentFrame = 0;
    enemy2Sprite.initialise(ENEMY2_WIDTH, ENEMY2_HEIGHT, ENEMY2_COLS,
                            &enemy2Texture);
    enemy2Sprite.setScale(2.5);
    // Hardcoding position
    enemy2Sprite.setPosition(GAME_WIDTH / 2 - enemySprite.getWidth() / 2,
                             -enemySprite.getHeight());

    enemy2Sprite.animates = true;

    enemy3Sprite.startFrame = 0, enemy3Sprite.endFrame = 1,
    enemy3Sprite.currentFrame = 0;
    enemy3Sprite.initialise(ENEMY3_WIDTH, ENEMY3_HEIGHT, ENEMY3_COLS,
                            &enemy3Texture);
    enemy3Sprite.setScale(2.5);
    // Hardcoding position
    enemy3Sprite.setPosition(GAME_WIDTH / 2 - enemySprite.getWidth() / 2,
                             -enemySprite.getHeight());
    enemy3Sprite.animates = true;

    enemy.enabled = true;
    enemyCollider.collisionType = BOX;
    enemyCollider.collisionResponse = NONE;
    enemyBulletEmitter.emitterID = "enemy";
    enemyBulletEmitter.acceleration = Vec2(0, -50);
    enemyBulletEmitter.velocity = Vec2(0, -300);
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
    gameSystems->addSystem(*enemySystem);
    gameSystems->addSystem(*playerControlSystem);
    backgroundEntity = ecs->makeEntity(backgroundSprite);
    planetEntity = ecs->makeEntity(planetSprite, planetAnimation);
    shipEntity = ecs->makeEntity(shipSprite, shipBulletEmitter, shipAnimation,
                                 shipMotion, shipCollider, playerControlled);
    for (int i = 0; i < numberOfEnemies; i++) {
      enemyHooks.push_back(ecs->makeEntity(enemy, enemySprite, enemyMotion,
                                           enemyCollider, enemyBulletEmitter));
    }
    graphicsSystems->addSystem(*bulletSystem);
    Scene::attach();
  }
  void detach()
  {
    graphicsSystems->removeSystem(*bulletSystem);
    gameSystems->removeSystem(*enemySystem);
    gameSystems->removeSystem(*playerControlSystem);
    ecs->removeEntity(backgroundEntity);
    ecs->removeEntity(planetEntity);
    ecs->removeEntity(shipEntity);

    delete playerControlSystem;
    delete bulletSystem;
    delete enemySystem;

    Scene::detach();
  }
};
