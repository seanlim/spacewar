#pragma once
#define WIN32_LEAN_AND_MEAN

// clang-format off
#include <windows.h>
#include <Mmsystem.h>
// clang-format on

#include <string>

#include "common.h"
#include "gameError.h"
#include "graphics.h"
#include "input.h"
#include "text.h"
#include "ecs.h"
#include "systems/physics.h"
#include "systems/renderable.h"
#include "systems/collision.h"

namespace gameNS
{
const char FONT[] = "Consolas";
const int POINT_SIZE = 15;
const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;
} // namespace gameNS

class Game
{
protected:
  Graphics* graphics;
  Input* input;
  HWND hwnd;
  HRESULT hr;

  // Performance
  LARGE_INTEGER timeStart;
  LARGE_INTEGER timeEnd;
  LARGE_INTEGER timeFreq;

  float frameTime;
  float fps;
  Text gameText;
  DWORD sleepTime; // Sleep in between frames
  bool paused;
  bool initialised;

  // Debug
  bool showFps = false;
  bool debug = false;

  // ECS
  ECS ecs;

  // System lists
  SystemList gameSystems;
  SystemList graphicsSystems;

  // Core systems
  SRenderable* renderSystem;
  SPhysics* physicsSystem;
  SCollision* collisionSystem;

public:
  Game();
  virtual ~Game();

  Graphics* getGraphics() { return graphics; }
  Input* getInput() { return input; }

  // Handle input
  void handleInput(WPARAM, LPARAM, UINT);

  virtual void initialise(HWND hwnd);
  virtual void run(HWND);
  virtual void releaseAll();
  virtual void resetAll();
  virtual void deleteAll();
  virtual void renderGame();
  virtual void handleLostGraphicsDevice();

  virtual void render() = 0;
  virtual void update() = 0;
  virtual void ai() = 0;
  virtual void collisions() = 0;

  void exitGame();
};