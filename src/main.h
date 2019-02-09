
#include "game.h"
#include "splash.h"

class Main : public Game
{
  // Instantiate game scenes here
  // Breakout* breakout = new Breakout();
  SplashScreen* splash = new SplashScreen();

public:
  void setupRootScene()
  {
    // Init and set root scene
    // this->setScene(breakout);
    this->setScene(splash);
  }
};