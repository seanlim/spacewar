#pragma once

#include "breakout.h"
#include "game.h"

class Main : public Game
{
  Breakout* breakout = new Breakout();

public:
  void setupRootScene() { this->setScene(breakout); }
};