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

		this->input = _input;
		this->game = _game;
	}
	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CBulletEmitter* bulletEmit = (CBulletEmitter*)components[0];
		CSprite* playerSprite = (CSprite*)components[1];
		CMotion* motion = (CMotion*)components[2];
		CPlayerControlled* playerControlledEntity = (CPlayerControlled*)components[3];

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
			}
			if (input->getKeyboardKeyState(VK_SPACE) == JustReleased)
			{
				bulletEmit->shooting = false;
			}

		}
	}
};
