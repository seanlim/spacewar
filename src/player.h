#pragma once

#include "common.h"
#include "ecs.h"
#include "input.h"
#include "systems/physics.h"

//Create keyboardControl component
struct CPlayerControlled : public Component<CPlayerControlled> {
	bool enabled = true;
};

//Create player system
class SPlayerShoot : public System
{
	Input* input;
	Game* game;
	CSprite bulletSprite;

public:
	SPlayerShoot(Input* _input, Game* _game) : System()
	{
		System::addComponentType(CBulletEmitter::id);
		System::addComponentType(CSprite::id);
		System::addComponentType(CMotion::id);
		System::addComponentType(CPlayerControlled::id);
		//System::addComponentType(CCollidable::id);

		this->input = _input;
		this->game = _game;
	}
	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CBulletEmitter* bulletEmit = (CBulletEmitter*)components[0];
		CSprite* playerSprite = (CSprite*)components[1];
		CMotion* motion = (CMotion*)components[2];
		CPlayerControlled* playerControlledEntity = (CPlayerControlled*)components[3];
		//CCollidable* collider = (CCollidable*)components[4];

		if (playerControlledEntity->enabled) {
			if (input->getKeyboardKeyState(VK_LEFT)) {
				motion->velocity.x += -50.0;
			}
			if (input->getKeyboardKeyState(VK_RIGHT)) {
				motion->velocity.x += 50.0;
			}
			if (input->getKeyboardKeyState(VK_UP)) {
				motion->velocity.y += -50.0;
			}
			if (input->getKeyboardKeyState(VK_DOWN)) {
				motion->velocity.y += 50.0;
			}
			if (input->getKeyboardKeyState(VK_SPACE) == JustPressed)
			{
				bulletEmit->shooting = true;

				PlaySound(BULLET_AUDIO, NULL, SND_FILENAME | SND_ASYNC);

				if (bulletEmit->shooting == true)
				{
					bulletEmit->firingRate = 0.01f / delta;
				}
			}
			if (input->getKeyboardKeyState(VK_SPACE) == JustReleased)
			{
				bulletEmit->shooting = false;
			}
		}

		if (playerSprite->getX() > GAME_WIDTH)
		{
			playerSprite->setPosition(0, playerSprite->getY());
		}

		if (playerSprite->getX() < 0)
		{
			playerSprite->setPosition(GAME_WIDTH, playerSprite->getY());
		}

		if (playerSprite->getY() > GAME_HEIGHT)
		{
			playerSprite->setPosition(playerSprite->getX(), 0);
		}

		if (playerSprite->getY() < 0)
		{
			playerSprite->setPosition(playerSprite->getX(), GAME_HEIGHT);
		}

		/*
		// Check collision
		CCollidable playerCollider; 
		playerCollider.angle = bulletSprite.getAngle();
		playerCollider.center = *bulletSprite.getCenter();
		playerCollider.scale = bulletSprite.getScale();
		playerCollider.edge = { -(bulletSprite.spriteData.width / 2),
			-(bulletSprite.spriteData.height / 2),
			(bulletSprite.spriteData.width / 2),
			(bulletSprite.spriteData.height / 2) };
		Vec2 collisionVector = Vec2(0, 0);
		if (collider->collideBox(playerCollider, collisionVector) == true) {
			motion->collidedDelta =
				collider->bounce(playerCollider, collisionVector);
			motion->colliding = true;
		}
		*/
	}
};
