#pragma once
#define WIN32_LEAN_AND_MEAN

#include "common.h"
#include "constants.h"
#include "ecs.h"
#include "playerControls.h"
#include "scene.h"
#include "textureManager.h"
#include "tileMap.h"

class Breakout : public Scene
{
  TextureManager tileTexture, paddleBallTexture;

  SBreakOutTileMap* tileMapSystem;
  SPlayerControlled* playerControlSystem;

public:
  Breakout();
  virtual ~Breakout();

  void setupSystems();
  void setupTextures();
  void setupEntities();

  void render();
};