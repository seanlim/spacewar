#pragma once

#include "common.h"
#include "ecs.h"
#include "scene.h"
#include "textureManager.h"

class SplashScreen : public Scene
{
  float timer = 0;
  TextureManager logoTexture;

  EntityHook logo;
  CSprite logoSprite;
  CAnimated logoAnimation;

public:
  SplashScreen() : Scene() {}
  ~SplashScreen() {}
  void setupSystems() {}
  void setupTextures()
  {
    if (!logoTexture.initialise(graphics, ENGINE_LOGO))
      Logger::error("Failed to load logo texture");
  }
  void setupComponents()
  {
    logoSprite.currentFrame = 0;
    logoSprite.initialise(ENGINE_LOGO_WIDTH, ENGINE_LOGO_HEIGHT, 1,
                          &logoTexture);
    logoSprite.alpha = 0;
    logoSprite.animates = false;
    logoSprite.setPosition(GAME_WIDTH / 2 - (logoSprite.getWidth() / 2),
                           GAME_HEIGHT / 2 - (logoSprite.getHeight() / 2));

    Animation fadeAnimation = {ALPHA, 0, 1, 0.08, true, false, false};
    Animation scaleAnimation = {SCALE, 1.0, 1.3, 0.03, true, false, true};
    logoAnimation.animations.push_back(fadeAnimation);
    logoAnimation.animations.push_back(scaleAnimation);
  }
  void update(float delta)
  {
    timer += 0.01f / delta;

    if (timer >= 450) game->nextScene(this);
  }
  void render() {}
  void attach()
  {
    logo = ecs->makeEntity(logoSprite, logoAnimation);
    Scene::attach();
  }
  void detach()
  {
    ecs->removeEntity(logo);
    Scene::detach();
  }
};