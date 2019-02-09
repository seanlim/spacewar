#pragma once

#include "common.h"
#include "constants.h"
#include "ecs.h"
#include "textureManager.h"

struct CEnemyInteractable : public Component<CEnemyInteractable> {
public:
};

class SEnemy : public System
{
private:
	Graphics * graphics;
	int MIN_MAX_RAND(int min, int max) { return rand() % (max - min + 1) + min; }


public:
	enum EnemyType {
		SMALL,
		MEDIUM,
		LARGE,
		DESTROYED
	};
	
	struct Enemy {
		Vec2 enemyVelocity = Vec2(0, 100);
		Vec2 enemyPosition;
		EnemyType enemyType = SMALL;
	};

	CSprite *enemySprite, *enemy2Sprite, *enemy3Sprite, *enemyDestroyedSprite;
	CCollidable enemyCollider;

	int numEnemies = 10;
	Array<Enemy> enemies = {};

	float timer = 0;

	SEnemy(Graphics* _graphics) : System()
	{
		System::addComponentType(CMotion::id);
		System::addComponentType(CCollidable::id);
		System::addComponentType(CEnemyInteractable::id);

		enemyCollider.collisionType = CIRCLE;
		enemyCollider.collisionResponse = NONE;
		this->graphics = _graphics;
		init();
	}

	virtual void init()
	{
		Enemy newEnemy;
			
		for (int i = 0; i < numEnemies; i++)
			enemies.push_back(newEnemy);
		for (int i = 0; i < enemies.size(); i++)
			enemies[i].enemyPosition = Vec2((rand() % GAME_WIDTH - 64), 0);
	}

	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CMotion* motion = (CMotion*)components[0];
		CCollidable* collider = (CCollidable*)components[1];
		CEnemyInteractable* enemyInteractable = (CEnemyInteractable*)components[2];

		enemySprite->updateCurrentFrame(delta);
		//enemy2Sprite->updateCurrentFrame(delta);
		//enemy3Sprite->updateCurrentFrame(delta);
		//enemyDestroyedSprite->updateCurrentFrame(delta);

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i].enemyPosition += enemies[i].enemyVelocity * delta;
			enemySprite->setPosition(enemies[i].enemyPosition.x, enemies[i].enemyPosition.y);

			// Draw enemies
			switch (enemies[i].enemyType) {
			case SMALL:
				graphics->spriteBegin();
				enemySprite->spriteData.texture = enemySprite->textureManager->getTexture();
				graphics->drawSprite(enemySprite->spriteData);
				graphics->spriteEnd();
				break;
			case MEDIUM:
				graphics->spriteBegin();
				enemy2Sprite->spriteData.texture = enemy2Sprite->textureManager->getTexture();
				graphics->drawSprite(enemy2Sprite->spriteData);
				graphics->spriteEnd();
				break;
			case LARGE:
				graphics->spriteBegin();
				enemy3Sprite->spriteData.texture = enemy3Sprite->textureManager->getTexture();
				graphics->drawSprite(enemy3Sprite->spriteData);
				graphics->spriteEnd();
				break;
			case DESTROYED:
				graphics->spriteBegin();
				enemyDestroyedSprite->spriteData.texture = enemyDestroyedSprite->textureManager->getTexture();
				graphics->drawSprite(enemyDestroyedSprite->spriteData);
				graphics->spriteEnd();
				break;
			}

			// Check collision
			enemyCollider.angle = enemySprite->getAngle();
			enemyCollider.center = *enemySprite->getCenter();
			enemyCollider.scale = enemySprite->getScale();

			Vec2 collisionVector = Vec2(0, 0);
			if (collider->collideBox(enemyCollider, collisionVector) == true) {
				//motion->collidedDelta =
				//	collider->bounce(enemyCollider, collisionVector);
				motion->colliding = true;

				enemies[i].enemyType = DESTROYED;
				enemies.erase(enemies.begin() + i);
			}
		}
	}
};