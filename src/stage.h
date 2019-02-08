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

  TextureManager backgroundTexture, spaceShipTexture, enemyTexture;

  // Systems
  SEnemy* enemySystem;
  SPlayerControlled* playerControlSystem;

public:
  Stage() : Scene() {}
  ~Stage();

  void setupSystems() 
  {
	  // Enemy System
	  enemySystem = new SEnemy(graphics);
	  graphicsSystems->addSystem(*enemySystem);

	  // Player controls
	  playerControlSystem = new SPlayerControlled(input);
	  gameSystems->addSystem(*playerControlSystem);
  }

  void setupTextures()
  {
    if (!backgroundTexture.initialise(graphics, BACKGROUND))
      Logger::error("Failed to load background texture");
    if (!spaceShipTexture.initialise(graphics, SHIPS))
      Logger::error("Failed to load ships texture");
	if (!enemyTexture.initialise(graphics, ENEMY_ONE))
	  Logger::error("Failed to load enemy texture");

	Logger::println("Test");
  }

  void setupEntities() 
  {
	  // Init enemies
	  CSprite enemySprite;
	  enemySprite.startFrame = 0, enemySprite.endFrame = 1,
		  enemySprite.currentFrame = 0;
	  enemySprite.initialise(16, 16, 2, &enemyTexture);
	  enemySprite.setScale(2.5);
	  enemySystem->enemySprite = enemySprite;
	  CMotion enemyMotion;
	  enemyMotion.velocity = Vec2(0.0, 200);
	  enemyMotion.friction = 0.0;
	  enemySystem->enemyMotion = enemyMotion;

	  // Init player
	  CSprite playerSprite;
	  playerSprite.startFrame = 0, playerSprite.endFrame = 0,
		  playerSprite.currentFrame = 0;
	  playerSprite.initialise(64, 64, 10, &spaceShipTexture);
	  playerSprite.setScale(1);
	  playerSprite.setPosition(GAME_WIDTH / 3, GAME_HEIGHT / 2);
	  CMotion playerMotion;
	  CPlayerControlled playerControls;
	  CCollidable playerCollision;
	  playerCollision.collisionType = ORIENTED_BOX;
	  playerCollision.collisionResponse = NONE;
	  CEnemyInteractable enemyCollider;

	  ecs->makeEntity(playerControls, playerSprite, playerMotion, enemyCollider);

	  return;
  }

  void render() {}
  void attach() { Scene::attach(); }
  void detach() { Scene::detach(); }
};