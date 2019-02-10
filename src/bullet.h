#pragma once

#include "common.h"
#include "ecs.h"
#include "input.h"
#include "systems/physics.h"

//Create bulletEmitted component
struct CBulletEmitter : Component<CBulletEmitter> {
	bool shooting = false;
	float firingRate;
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

	Array<Bullet> bullets;

public:
	Bullet bullet;
	CSprite bulletSprite;
	SBullet(Graphics* _graphics, Game* _game) : System()
	{
		System::addComponentType(CBulletEmitter::id);
		System::addComponentType(CSprite::id);

		this->graphics = _graphics;
		this->game = _game;
	}
	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CBulletEmitter* bulletEmit = (CBulletEmitter*)components[0];
		CSprite* playerPos = (CSprite*)components[1];

		Logger::println("FPS " + std::to_string(0.01f / delta));

		if (bulletEmit->shooting == true && bulletEmit->firingRate > 1.7)
		{
			//Store bullet
			Bullet newBullet = Bullet();
			newBullet.velocity = Vec2(0.0, -120);
			newBullet.position = Vec2(playerPos->getX(),
				playerPos->getY() - playerPos->getHeight());

			// Fire bullet
			bullets.push_back(newBullet);
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i].position += bullets[i].velocity*delta;
			bulletSprite.setPosition(bullets[i].position.x, bullets[i].position.y);
			
			//PlaySound(BULLET_AUDIO, NULL, SND_FILENAME | SND_SYNC);

			graphics->spriteBegin();
			bulletSprite.spriteData.texture =
				bulletSprite.textureManager->getTexture();
			graphics->drawSprite(bulletSprite.spriteData);
			graphics->spriteEnd();
		} 
	}
};