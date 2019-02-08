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
	float timer = 0;

public:
	CSprite enemySprite;

	SEnemy(Graphics* _graphics) : System()
	{
		System::addComponentType(CMotion::id);
		System::addComponentType(CCollidable::id);
		System::addComponentType(CEnemyInteractable::id);

		this->graphics = _graphics;
		//setEnemies(1);
		enemySprite.setPosition(MIN_MAX_RAND(0, GAME_WIDTH - 64), 0);
	}

	inline void setEnemies(int value)
	{
	}

	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CMotion* motion = (CMotion*)components[0];
		CCollidable* collider = (CCollidable*)components[1];
		CEnemyInteractable* enemyInteractable = (CEnemyInteractable*)components[2];

		// Draw enemies
		graphics->spriteBegin();	
		enemySprite.spriteData.texture = enemySprite.textureManager->getTexture();
		graphics->drawSprite(enemySprite.spriteData);
		graphics->spriteEnd();
					
		// Check collision
		CCollidable enemyCollider;
		enemyCollider.angle = enemySprite.getAngle();
		enemyCollider.center = *enemySprite.getCenter();
		enemyCollider.scale = enemySprite.getScale();

		Vec2 collisionVector = Vec2(0, 0);
		if (collider->collideBox(enemyCollider, collisionVector) == true) {
			motion->collidedDelta =
				collider->bounce(enemyCollider, collisionVector);
			motion->colliding = true;
		}
	}
};