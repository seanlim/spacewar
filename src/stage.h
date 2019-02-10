#pragma once

#include "common.h"
#include "constants.h"
#include "input.h"
#include "math/lerp.h"
#include "scene.h"
#include "bullet.h"
#include "player.h"
#include "systems/animation.h"

class Stage : public Scene
{
	TextureManager backgroundTexture, playerTexture, bulletTexture;

	// Systems
	SBullet* bulletShoot;
	SPlayerShoot* playerShoot;
	SAnimation* playerAnimation;

	// Components
	CMotion bulletMotion, playerMotion;
	CSprite bulletSprite, playerSprite, playerPos;
	CBulletEmitter bulletEmit;
	CPlayerControlled playerInput;
	CAnimated playerShipAnimation;

	// Entities
	EntityHook bullet;
	EntityHook player;

public:
	Stage() : Scene() {}
	~Stage()
	{
		bulletTexture.onLostDevice();
		bulletTexture.onResetDevice();
		playerTexture.onLostDevice();
		playerTexture.onResetDevice();
	}

	void setupSystems()
	{
		bulletShoot = new SBullet(graphics, game);
		playerShoot = new SPlayerShoot(input, game);
		playerAnimation = new SAnimation();
	}

	void setupTextures()
	{
		if (!backgroundTexture.initialise(graphics, BACKGROUND))
			Logger::error("Failed to load background texture");
		if (!playerTexture.initialise(graphics, SHIPS))
			Logger::error("Failed to load ships texture");
		if (!bulletTexture.initialise(graphics, BULLET))
			Logger::error("Failed to load bullet texture");
	}

	void setupEntities()
	{
		//Setup bullet
		bulletSprite.startFrame = 0, bulletSprite.endFrame = 1,
			bulletSprite.currentFrame = 0;
		bulletSprite.animates = false;
		bulletSprite.initialise(BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLS,
			&bulletTexture);
		bulletSprite.setScale(1.5);
		bulletShoot->bulletSprite = bulletSprite;
		
		//Setup player
		playerSprite.startFrame = 0, playerSprite.endFrame = 1,
			playerSprite.currentFrame = 0;
		playerSprite.animates = false;
		playerSprite.initialise(SHIPS_WIDTH, SHIPS_HEIGHT, SHIPS_COLS,
			&playerTexture);
		playerSprite.setScale(1.5);
		playerSprite.setPosition(GAME_WIDTH / 2 - playerSprite.getWidth() / 2,
			(GAME_HEIGHT / 2 - playerSprite.getHeight() / 2) + 70);
		playerShipAnimation.animationType = SCALE;
		playerShipAnimation.startValue = 1.5, playerShipAnimation.endValue = 1.7,
			playerShipAnimation.reverses = true, playerShipAnimation.rate = 0.06;
		playerPos = playerSprite;
	}

	void render()
	{
	}

	void attach()
	{
		graphicsSystems->addSystem(*bulletShoot);
		gameSystems->addSystem(*playerShoot);
		player = ecs->makeEntity(bulletEmit, playerSprite, playerMotion, playerInput);
		Scene::attach();
	}

	void detach()
	{
		graphicsSystems->removeSystem(*bulletShoot);
		gameSystems->removeSystem(*playerShoot);
		ecs->removeEntity(player);
		Scene::detach();
	}
};