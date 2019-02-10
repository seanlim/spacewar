
#include "game.h"
#include "menu.h"
#include "splash.h"
#include "stage.h"

class Main : public Game
{
  // Instantiate game scenes here
  // Breakout* breakout = new Breakout();
  SplashScreen* splash = new SplashScreen();
  Menu* menu = new Menu(&selectedShip);
  Stage* stage = new Stage(&selectedShip, &healthBar);

  int selectedShip;
  int healthBar;

public:
  void setupRootScene()
  {
    // Init and set root scene
    this->setScene(splash);
  }

  void nextScene(Scene* currentScene)
  {
    if (currentScene == splash) {
      this->setScene(menu);
    } else if (currentScene == menu) {
      this->setScene(stage);
    }
  }
};