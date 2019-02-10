#pragma once

#include "bullet.h"
#include "common.h"
#include "ecs.h"
#include "input.h"

struct CPlayerControlled : public Component<CPlayerControlled> {
  bool enabled = true;
  float speed;
  float sensitivity;
};

class SPlayerControlled : public System
{
protected:
  Input* input;

public:
  SPlayerControlled(Input* _input) : System()
  {
    System::addComponentType(CPlayerControlled::id);
    System::addComponentType(CMotion::id);
    System::addComponentType(CBulletEmitter::id);

    input = _input;
  }
  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CPlayerControlled* playerControlled = (CPlayerControlled*)components[0];
    CMotion* motion = (CMotion*)components[1];
    CBulletEmitter* bulletEmitter = (CBulletEmitter*)components[2];

    if (playerControlled->enabled) {
      if (input->getKeyboardKeyState(VK_LEFT) == JustPressed) {
        motion->velocity.x +=
            -(playerControlled->speed + playerControlled->sensitivity);
      } else if (input->getKeyboardKeyState(VK_LEFT) == Pressed) {
        motion->velocity.x += -playerControlled->speed;
      }

      if (input->getKeyboardKeyState(VK_RIGHT) == JustPressed) {
        motion->velocity.x +=
            (playerControlled->speed + playerControlled->sensitivity);
      } else if (input->getKeyboardKeyState(VK_RIGHT) == Pressed) {
        motion->velocity.x += playerControlled->speed;
      }

      if (input->getKeyboardKeyState(VK_UP) == JustPressed) {
        motion->velocity.y +=
            -(playerControlled->speed + playerControlled->sensitivity);
      } else if (input->getKeyboardKeyState(VK_UP) == Pressed) {
        motion->velocity.y += -playerControlled->speed;
      }

      if (input->getKeyboardKeyState(VK_DOWN) == JustPressed) {
        motion->velocity.y +=
            (playerControlled->speed + playerControlled->sensitivity);
      } else if (input->getKeyboardKeyState(VK_DOWN) == Pressed) {
        motion->velocity.y += playerControlled->speed;
      }

      if (input->getKeyboardKeyState(VK_SPACE) == JustPressed) {
		PlaySound(BULLET_AUDIO, NULL, SND_FILENAME | SND_ASYNC);
        bulletEmitter->firing = true;
      }
    }
  }
};