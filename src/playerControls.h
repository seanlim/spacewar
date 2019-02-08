#pragma once

#include "common.h"
#include "ecs.h"
#include "input.h"
#include "systems/physics.h"

struct CPlayerControlled : public Component<CPlayerControlled> {
	bool enabled = true;
};

class SPlayerControlled : public System
{
protected:
	Input * input;

public:
	SPlayerControlled(Input* _input) : System()
	{
		System::addComponentType(CPlayerControlled::id);
		System::addComponentType(CMotion::id);

		input = _input;
	}
	virtual void updateComponents(float delta, BaseComponent** components)
	{
		CPlayerControlled* playerControlledEntity =
			(CPlayerControlled*)components[0];
		CMotion* motion = (CMotion*)components[1];

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
		}
	}
};