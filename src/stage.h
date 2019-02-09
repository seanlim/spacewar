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

	// Components
	CMotion bulletMotion, playerMotion;
	CSprite bulletSprite, playerSprite, playerPos;
	CBulletEmitter bulletEmit;
	CPlayerControlled playerInput;
	CCollidable bulletCollision;

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
		bulletSprite.startFrame = 0, bulletSprite.endFrame = 1,
			bulletSprite.currentFrame = 0;
		bulletSprite.animates = false;
		bulletSprite.initialise(BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLS,
			&bulletTexture);
		bulletSprite.setScale(1.5);
		//bulletMotion.velocity = Vec2(0.0, -50);
		//bulletMotion.friction = 0.0;
		bulletShoot->bulletSprite = bulletSprite;
		
		playerSprite.startFrame = 0, playerSprite.endFrame = 1,
			playerSprite.currentFrame = 0;
		playerSprite.animates = false;
		playerSprite.initialise(SHIPS_WIDTH, SHIPS_HEIGHT, SHIPS_COLS,
			&playerTexture);
		playerSprite.setScale(1.5);
		playerSprite.setPosition(GAME_WIDTH / 2 - playerSprite.getWidth() / 2,
			(GAME_HEIGHT / 2 - playerSprite.getHeight() / 2) + 70);
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