#pragma once

#include "common.h"
#include "ecs.h "
#include "graphics.h"
#include "input.h"
#include "systems/collision.h"
#include "systems/physics.h"
#include "systems/renderable.h"

class Scene
{
protected:
  bool attached = false;

  HWND hwnd;
  ECS* ecs;
  Graphics* graphics;
  Input* input;

  // Pointers to main system lists
  SystemList* gameSystems;
  SystemList* graphicsSystems;

  // Core systems
  SRenderable* renderSystem;
  SPhysics* physicsSystem;
  SCollision* collisionSystem;

public:
  float delta;
  Scene() {}
  ~Scene() {}
  virtual void initialise(HWND _hwnd, Graphics* _graphics, Input* _input,
                          ECS* _ecs, SystemList* _gameSystems,
                          SystemList* _graphicsSystems)
  {
    this->hwnd = _hwnd;
    this->graphics = _graphics;
    this->ecs = _ecs;
    this->input = _input;
    this->gameSystems = _gameSystems;
    this->graphicsSystems = _graphicsSystems;

    Logger::println("Initialise graphics system...");
    renderSystem = new SRenderable(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN,
                                   this->graphics);
    Logger::println("Initialise physics system ...");
    physicsSystem = new SPhysics();
    Logger::println("Initialise collision system ...");
    collisionSystem = new SCollision();

    attach();

    setupSystems();
    setupTextures();
    setupEntities();

  } // Initialise core systems

  virtual void setupSystems() = 0; // Any custom system setup
  virtual void setupTextures() = 0;
  virtual void setupEntities() = 0;

  virtual void render() = 0; // In-game rendering

  virtual void attach()
  {
    Logger::println("Attaching scene...");
    graphicsSystems->addSystem(*renderSystem);
    gameSystems->addSystem(*physicsSystem);
    gameSystems->addSystem(*collisionSystem);
    attached = true;
  } // Scene running and detached hooks

  virtual void detach()
  {
    Logger::println("Deattached from scene...");
    graphicsSystems->removeSystem(*renderSystem);
    gameSystems->removeSystem(*physicsSystem);
    gameSystems->removeSystem(*collisionSystem);
    attached = false;
  };
};