#pragma once

#include "common.h"
#include "constants.h"
#include "input.h"
#include "math/lerp.h"
#include "scene.h"
#include "systems/animation.h"

class Stage : public Scene
{
  TextureManager backgroundTexture, earthTexture;

public:
  Stage() : Scene() {}
  ~Stage();

  void setupSystems() {}
  void setupTextures()
  {
    if (!backgroundTexture.initialise(graphics, BACKGROUND))
      Logger::error("Failed to load background texture");
    if (!planetTexture.initialise(graphics, PLANET))
      Logger::error("Failed ot load planet texture");
  }

  void setupEntities() {}

  void render() {}
  void update(float delta) {}
  void attach() { Scene::attach(); }
  void detach() { Scene::detach(); }
};