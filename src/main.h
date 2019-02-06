
#include "game.h"
#include "menu.h"

class Main : public Game
{
  // Instantiate game scenes here
  // Breakout* breakout = new Breakout();
  Menu* menu = new Menu();

public:
  void setupRootScene()
  {
    // Init and set root scene
    // this->setScene(breakout);
    this->setScene(menu);
  }
};