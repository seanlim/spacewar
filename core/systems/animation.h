#pragma once

#include "common.h"
#include "ecs.h"
#include "systems/renderable.h"

enum AnimationType { SCALE, ALPHA, BLINK };

struct Animation {
  AnimationType animationType;
  float startValue;
  float endValue;
  float rate = 0.01;
  bool reverses;
  bool reversed = false;
  bool repeats;
};

struct CAnimated : public Component<CAnimated> {
  Array<Animation> animations;
};

// TODO: Animation curve, this animation system is currently restricted to
// linear animations
class SAnimation : public System
{
public:
  SAnimation() : System()
  {
    System::addComponentType(CAnimated::id);
    System::addComponentType(CSprite::id);
  }

  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CAnimated* animated = (CAnimated*)components[0];
    CSprite* sprite = (CSprite*)components[1];

    for (int i = 0; i < animated->animations.size(); i++) {
      Animation animation = animated->animations[i];
      switch (animation.animationType) {
        // Scale animation
      case SCALE: {
        const float start = animation.startValue;
        float centerX = sprite->getCenterX(), centerY = sprite->getCenterY();
        const float newScale =
            lerp(sprite->getScale(), animation.endValue, animation.rate);

        sprite->setScale(newScale);

        sprite->setPosition(centerX - (sprite->getWidth() / 2),
                            centerY - (sprite->getHeight() / 2));

        if (abs(sprite->getScale() - animation.endValue) < 0.01) {
          if (animation.reverses == true && animation.reversed == false) {
            animation.reversed = true;
            animation.startValue = animation.endValue;
            animation.endValue = start;
          } else if (animation.repeats) {
            if (animation.reversed) {
              sprite->setScale(animation.endValue);
              animation.reversed = false;
            } else {
              float cX = sprite->getCenterX(), cY = sprite->getCenterY();
              sprite->setScale(animation.startValue);
              sprite->setPosition(cX - (sprite->getWidth() / 2),
                                  cY - (sprite->getHeight() / 2));
            }
          }
        }
      } break;
      // Alpha animation
      case ALPHA: {
        const float newAlpha =
            lerp(sprite->alpha, animation.endValue, animation.rate);
        sprite->alpha = newAlpha;
        if (abs(sprite->alpha - animation.endValue) < 0.01) {
          if (animation.reverses == true && animation.reversed == false) {
            animation.reversed = true;
            float start = animation.startValue;
            animation.startValue = animation.endValue;
            animation.endValue = start;
          } else if (animation.repeats) {
            sprite->alpha = animation.startValue;
          }
        }

      } break;
	  // Blinking animation
	  case BLINK: {
	  }
      }

      animated->animations[i] = animation;
    }
  }
};