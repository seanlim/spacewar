#pragma once
#define WIN32_LEAN_AND_MEAN

#include "common.h"
#include "entity.h"

namespace planetNS
{
const int WIDTH = 128;                    // image width
const int HEIGHT = 128;                   // image height
const int COLLISION_RADIUS = 120 / 2;     // for circular collision
const int X = GAME_WIDTH / 2 - WIDTH / 2; // location on screen
const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
const float MASS = 1.0e14f; // mass
const int TEXTURE_COLS = 2; // texture has 2 columns
const int START_FRAME = 1;  // starts at frame 1
const int END_FRAME = 1;
} // namespace planetNS

class Planet : public Entity
{
public:
  Planet();
};
