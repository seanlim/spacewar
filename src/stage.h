#pragma once

#include "common.h"
#include "constants.h"
#include "input.h"
#include "math/lerp.h"
#include "scene.h"
#include "systems/animation.h"

class Stage : public Scene
{
  TextureManager backgroundTexture, spaceShipTexture;

public:
  Stage() : Scene() {}
  ~Stage();

  void setupSystems() {}
  void setupTextures()
  {
    if (!backgroundTexture.initialise(graphics, BACKGROUND))
      Logger::error("Failed to load background texture");
    if (!spaceShipTexture.initialise(graphics, SHIPS))
      Logger::error("Failed to load ships texture");
  }

  void setupEntities() {}

  void render() {}
  void attach() { Scene::attach(); }
  void detach() { Scene::detach(); }
};