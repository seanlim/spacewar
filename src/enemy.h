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

public:
	CSprite enemySprite;
	CMotion enemyMotion;

	SEnemy(Graphics* _graphics) : System()
	{
		System::addComponentType(CMotion::id);
		System::addComponentType(CCollidable::id);
		System::addComponentType(CEnemyInteractable::id);

		this->graphics = _graphics;
		//setEnemyLocation(1);
	}

	//inline void setEnemyLocation(int value)
	//{
	//	for (int row = 0; row < rows; row++) {
	//		for (int column = 0; column < columns; column++) {
	//			enemyMap[row][column] = 1;
	//		}
	//	}
	//}

	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CMotion* motion = (CMotion*)components[0];
		CCollidable* collider = (CCollidable*)components[1];
		CEnemyInteractable* enemyInteractable = (CEnemyInteractable*)components[2];

		// Draw enemies
		enemySprite.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
		Logger::println("HELLO PLES LOG");

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