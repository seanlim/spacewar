#pragma once

#include "common.h"
#include "constants.h"
#include "ecs.h"
#include "textureManager.h"
#include <cmath>

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

		Vec2 node1 = Vec2(0, 0);
		Vec2 node2 = Vec2(100, 400);
		Vec2 node3 = Vec2(400, 200);
		Vec2 node4 = Vec2(400, 500);

		for (int i = 0; i < numEnemies; i++)
			enemies.push_back(newEnemy);
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i].enemyPosition = Vec2((rand() % GAME_WIDTH - 64), 0);
			enemies[i].path.addNode(node1);
			enemies[i].path.addNode(node2);
			enemies[i].path.addNode(node3);
			enemies[i].path.addNode(node4);
		}
	}

	virtual Vec2 pathFollowing(Enemy newEnemy) {
		Vec2 target = Vec2();

		// check if there is an path already
		bool isEmpty = newEnemy.path.nodes.empty();
		Vec2 *newVec2 = new Vec2();

		if (!isEmpty) {
			std::vector <Vec2> nodes = newEnemy.path.getNodes();	// if there is a path, store these nodes in a vector: nodes
			target = nodes[newEnemy.currentNode];	// get the current target nodes[0]

			// check distance between enemy position vector and target position vector
			if (distance(newEnemy.enemyPosition, target) <= 5) {
				newEnemy.currentNode += 1;

				if (newEnemy.currentNode >= nodes.size()) {
					newEnemy.currentNode += nodes.size() - 1;
				}
			}
		}

		return !isEmpty ? target : *newVec2;
	}

	// check distance between two vectors
	virtual int distance(Vec2 a, Vec2 b) {
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CMotion* motion = (CMotion*)components[0];
		CCollidable* collider = (CCollidable*)components[1];
		CEnemyInteractable* enemyInteractable = (CEnemyInteractable*)components[2];

		enemySprite->updateCurrentFrame(delta);
		enemy2Sprite->updateCurrentFrame(delta);
		enemy3Sprite->updateCurrentFrame(delta);
		enemyDestroyedSprite->updateCurrentFrame(delta);

		for (int i = 0; i < enemies.size(); i++) {
			Logger::println(enemies[i].enemyVelocity);

			enemies[i].enemyVelocity += pathFollowing(enemies[i]);
			enemies[i].enemyPosition += enemies[i].enemyVelocity * delta;
			enemySprite->setPosition(enemies[i].enemyPosition.x, enemies[i].enemyPosition.y);

			switch (enemies[i].enemyType) {
			case SMALL:
				// Draw enemies
				graphics->spriteBegin();
				enemySprite->spriteData.texture = enemySprite->textureManager->getTexture();
				graphics->drawSprite(enemySprite->spriteData);
				graphics->spriteEnd();

				// Check collision
				enemyCollider.angle = enemySprite->getAngle();
				enemyCollider.center = *enemySprite->getCenter();
				enemyCollider.scale = enemySprite->getScale();
				break;
			case MEDIUM:
				graphics->spriteBegin();
				enemy2Sprite->spriteData.texture = enemy2Sprite->textureManager->getTexture();
				graphics->drawSprite(enemy2Sprite->spriteData);
				graphics->spriteEnd();

				// Check collision
				enemyCollider.angle = enemy2Sprite->getAngle();
				enemyCollider.center = *enemy2Sprite->getCenter();
				enemyCollider.scale = enemy2Sprite->getScale();
				break;
			case LARGE:
				graphics->spriteBegin();
				enemy3Sprite->spriteData.texture = enemy3Sprite->textureManager->getTexture();
				graphics->drawSprite(enemy3Sprite->spriteData);
				graphics->spriteEnd();

				// Check collision
				enemyCollider.angle = enemy3Sprite->getAngle();
				enemyCollider.center = *enemy3Sprite->getCenter();
				enemyCollider.scale = enemy3Sprite->getScale();
				break;
			case DESTROYED:
				graphics->spriteBegin();
				enemyDestroyedSprite->spriteData.texture = enemyDestroyedSprite->textureManager->getTexture();
				graphics->drawSprite(enemyDestroyedSprite->spriteData);
				graphics->spriteEnd();
				break;
			}

			Vec2 collisionVector = Vec2(0, 0);
			if (collider->collideBox(enemyCollider, collisionVector) == true) {
				motion->collidedDelta = collider->bounce(enemyCollider, collisionVector);
				motion->colliding = true;

				enemies[i].enemyType = DESTROYED;
				enemies.erase(enemies.begin() + i);
			}
		}
	}
};