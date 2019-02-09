#pragma once

#include "common.h"
#include "constants.h"
#include "input.h"
#include "math/lerp.h"
#include "scene.h"
#include "systems/animation.h"
#include "enemy.h"
#include "playerControls.h"

class Stage : public Scene
{

  TextureManager backgroundTexture, spaceShipTexture, 
	  enemyTexture, enemy2Texture, enemy3Texture, enemyDestroyedTexture,
	  planetTexture;

  // Systems
  SEnemy* enemySystem;
  SPlayerControlled* playerControlSystem;

  // Components
  CSprite enemySprite, enemy2Sprite, enemy3Sprite, enemyDestroyedSprite,
	  playerSprite;
  CMotion playerMotion;
  CPlayerControlled playerControls;
  CCollidable playerCollision;
  CEnemyInteractable enemyCollider;

  // Entities
  EntityHook enemy;

public:

  Stage() : Scene() {}
  ~Stage();

  void setupSystems()
  {
	  // Enemy system
	  enemySystem = new SEnemy(graphics);
	  enemySystem->enemySprite = &enemySprite;

	  // Player controls
	  playerControlSystem = new SPlayerControlled(input);
  }

  void setupTextures()
  {
    if (!backgroundTexture.initialise(graphics, BACKGROUND))
      Logger::error("Failed to load background texture");
    if (!spaceShipTexture.initialise(graphics, SHIPS))
      Logger::error("Failed to load ships texture");
	if (!enemyTexture.initialise(graphics, ENEMY_ONE))
		Logger::error("Failed to load enemy 1 texture");
	if (!enemy2Texture.initialise(graphics, ENEMY_TWO))
		Logger::error("Failed to load enemy 2 texture");
	if (!enemy3Texture.initialise(graphics, ENEMY_THREE))
		Logger::error("Failed to load enemy 3 texture");
	if (!enemyDestroyedTexture.initialise(graphics, ENEMY_EXPLOSION))
		Logger::error("Failed to load enemy destroyed texture");
    if (!planetTexture.initialise(graphics, PLANET))
      Logger::error("Failed to load planet texture");
  }

  void setupComponents()
  {
	  // Init enemies
	  enemySprite.startFrame = 0, enemySprite.endFrame = 1,
		  enemySprite.currentFrame = 0;
	  enemySprite.initialise(16, 16, 2, &enemyTexture);
	  enemySprite.setScale(2.5);
	  enemySprite.animates = true;

	  enemy2Sprite.startFrame = 0, enemy2Sprite.endFrame = 1,
		  enemy2Sprite.currentFrame = 0;
	  enemy2Sprite.initialise(32, 16, 2, &enemy2Texture);
	  enemy2Sprite.setScale(2.5);
	  enemySprite.animates = true;

	  enemy3Sprite.startFrame = 0, enemy3Sprite.endFrame = 1,
		  enemy3Sprite.currentFrame = 0;
	  enemy3Sprite.initialise(32, 32, 2, &enemy3Texture);
	  enemy3Sprite.setScale(2.5);
	  enemySprite.animates = true;

	  enemyDestroyedSprite.startFrame = 0, enemyDestroyedSprite.endFrame = 5,
		  enemyDestroyedSprite.currentFrame = 0;
	  enemyDestroyedSprite.initialise(16, 16, 5, &enemyDestroyedTexture);
	  enemyDestroyedSprite.setScale(2.5);
	  enemyDestroyedSprite.animates = true;

	  // Init player
	  playerSprite.startFrame = 0, playerSprite.endFrame = 0,
		  playerSprite.currentFrame = 0;
	  playerSprite.initialise(64, 64, 10, &spaceShipTexture);
	  playerSprite.setScale(1);
	  playerSprite.setPosition(GAME_WIDTH / 3, GAME_HEIGHT / 2);
	  playerCollision.collisionType = ORIENTED_BOX;
	  playerCollision.collisionResponse = NONE;

	  return;
  }

  void render() {}
  void update(float delta) {}

  void attach() 
  { 
	  graphicsSystems->addSystem(*enemySystem);
	  gameSystems->addSystem(*playerControlSystem);
	  enemy = ecs->makeEntity(playerControls, playerSprite, playerMotion, playerCollision, enemyCollider);
	  Scene::attach(); 
  }
  void detach() 
  { 
	  Scene::detach(); 
	  graphicsSystems->removeSystem(*enemySystem);
	  ecs->removeEntity(enemy);
  }
};
