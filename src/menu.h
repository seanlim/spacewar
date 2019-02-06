#pragma once

#include "common.h"
#include "constants.h"
#include "math/lerp.h"
#include "scene.h"
#include "systems/animation.h"

class Menu : public Scene
{

  TextureManager backgroundTexture, spaceShipTexture;

  SAnimation* menuAnimation;

public:
  Menu() {}
  ~Menu();

  void setupSystems()
  {
    menuAnimation = new SAnimation();
    gameSystems->addSystem(*menuAnimation);
  }
  void setupTextures()
  {
    if (!backgroundTexture.initialise(graphics, BACKGROUND))
      Logger::error("Failed to load background texture");
    if (!spaceShipTexture.initialise(graphics, SHIPS))
      Logger::error("Failed to load ships texture");
  }
  void setupEntities()
  {
    CSprite shipSprite;
    shipSprite.startFrame = 0, shipSprite.endFrame = 9,
    shipSprite.currentFrame = 0;
    shipSprite.animates = false;
    shipSprite.initialise(SHIPS_WIDTH, SHIPS_HEIGHT, SHIPS_COLS,
                          &spaceShipTexture);
    shipSprite.setScale(1.3);
    shipSprite.setPosition(GAME_WIDTH / 2 - shipSprite.getWidth() / 2,
                           GAME_HEIGHT / 2 - shipSprite.getHeight() / 2);
    CAnimated shipAnimation;
    shipAnimation.animationType = SCALE;
    shipAnimation.startValue = 1.0, shipAnimation.endValue = 1.3,
    shipAnimation.reverses = true, shipAnimation.rate = 0.06;
    ecs->makeEntity(shipSprite, shipAnimation);
  }

  void render() {}
};
