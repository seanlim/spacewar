#pragma once

#include "common.h"
#include "ecs.h "
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "stack.h"
#include "systems/collision.h"
#include "systems/physics.h"
#include "systems/renderable.h"

class Game;
class Scene
{
protected:
  bool attached = false;

  HWND hwnd;
  ECS* ecs;
  Graphics* graphics;
  Input* input;
  Game* game;

  // Pointers to main system lists
  SystemList* gameSystems;
  SystemList* graphicsSystems;

  Stack<EntityHook> entities;

public:
  float delta;
  Scene() {}
  ~Scene() {}
  virtual void initialise(HWND _hwnd, Game* _game, Graphics* _graphics,
                          Input* _input, ECS* _ecs, SystemList* _gameSystems,
                          SystemList* _graphicsSystems)
  {
    this->hwnd = _hwnd;
    this->game = _game;
    this->graphics = _graphics;
    this->ecs = _ecs;
    this->input = _input;
    this->gameSystems = _gameSystems;
    this->graphicsSystems = _graphicsSystems;

    setupSystems();
    setupTextures();
    setupComponents();

    attach(); // Attach scene entities

  } // Initialise core systems

  virtual void setupSystems() = 0; // Custom system setup
  virtual void setupTextures() = 0;
  virtual void setupComponents() = 0;

  virtual void render() = 0; // In-game draw calls

  virtual void update(float delta) = 0;

  // Attach all components and custom systems
  virtual void attach()
  {
    Logger::println("Attached scene...");
    attached = true;
  } // Scene running and detached hooks

  // Detach all components and systems
  virtual void detach()
  {
    Logger::println("Detaching from scene...");
    attached = false;
  };
};