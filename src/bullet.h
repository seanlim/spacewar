#pragma once

#include "common.h"
#include "ecs.h"
#include "input.h"
#include "systems/physics.h"

//Create bulletEmitted component
struct CBulletEmitter : Component<CBulletEmitter> {
	bool shooting = false;
};

//Create bullet system
class SBullet : public System
{
	struct Bullet {
		Vec2 velocity;
		Vec2 position;
	};
	Graphics* graphics;
	Game* game;

	Array<Bullet* > bullets;

public:
	Bullet bullet;
	CSprite bulletSprite;
	SBullet(Graphics* _graphics, Game* _game) : System()
	{
		System::addComponentType(CMotion::id);
		System::addComponentType(CBulletEmitter::id);
		System::addComponentType(CSprite::id);
		System::addComponentType(CCollidable::id);

		this->graphics = _graphics;
		this->game = _game;
	}
	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CMotion* motion = (CMotion*)components[0];
		CBulletEmitter* bulletEmit = (CBulletEmitter*)components[1];
		CSprite* playerPos = (CSprite*)components[2];
		CCollidable* bulletCollision = (CCollidable*)components[3];

		while (bulletEmit->shooting)
		{
			bulletSprite.setPosition(playerPos->getX(),
				playerPos->getY() - playerPos->getHeight());
			graphics->spriteBegin();
			bulletSprite.spriteData.texture =
				bulletSprite.textureManager->getTexture();
			graphics->drawSprite(bulletSprite.spriteData);
			graphics->spriteEnd();

			//Store bullet
			Bullet *newBullet = new Bullet(bullet);
			newBullet->velocity = Vec2(0.0, -50);
			newBullet->position = Vec2(playerPos->getX(),
				playerPos->getY() - playerPos->getHeight());

			// Fire bullet
			bullets.push_back(newBullet);

			// Collision for bullet
			bulletCollision->collisionType = ORIENTED_BOX;
			bulletCollision->collisionResponse = NONE;
		}

	}
};