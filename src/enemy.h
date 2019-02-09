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
	enum EnemyType { SMALL, MEDIUM, LARGE, DESTROYED };
	
	struct Path {
		// Every point in a path is stored in the nodes vector as a Vec2(x,y)
		Vec2 node;
		std::vector <Vec2> nodes;
		int radius = 20;

		// PATH FINDING
		virtual void addNode(Vec2 node) {
			nodes.push_back(node);
		}

		virtual std::vector <Vec2> getNodes() {
			return nodes;
		}
	};

	struct Enemy {

		Vec2 enemyVelocity = Vec2(0, 100);
		Vec2 enemyPosition;
		EnemyType enemyType = SMALL;
		Path path;
		int currentNode = 0;
		int pathDir = -1;
	};

	CSprite *enemySprite, *enemy2Sprite, *enemy3Sprite, *enemyDestroyedSprite;
	CCollidable enemyCollider;
	CAnimated enemyAnimation;

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

	virtual Vec2 pathFollowing(Enemy newEnemy) {
		Vec2 target = Vec2(0,0);
		bool isEmpty = newEnemy.path.nodes.empty();

		if (!isEmpty) {
			std::vector <Vec2> nodes = newEnemy.path.getNodes();
			target = nodes[newEnemy.currentNode];

			if (distance(newEnemy.enemyPosition, target) <= newEnemy.path.radius) {
				newEnemy.currentNode += newEnemy.pathDir;

				if (newEnemy.currentNode >= nodes.size || newEnemy.currentNode < 0) {
					newEnemy.pathDir *= -1;
					newEnemy.currentNode += newEnemy.pathDir;
				}
			}
		}

		return target != NULL ? seek(target) : new Vec2();
	}

	virtual int distance(Vec2 a, Vec2 b) {
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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
			enemies[i].enemyVelocity += pathFollowing(enemies[i]);
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

// Contain a vector of points and methods to manage this vector.
class SPath : public System 
{
private:
	Vec2 node;

public:
	// Every point in a path is stored in the nodes vector as a Vec2(x,y)
	std::vector <Vec2> nodes;

	virtual void addNode(Vec2 node) {
		nodes.push_back(node);
	}

	virtual std::vector <Vec2> getNodes() {
		return nodes;
	}
};