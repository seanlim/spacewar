
#include "game.h"
#include "menu.h"
#include "splash.h"
#include "stage.h"
#include "stageSelect.h"

class Main : public Game
{
  // Instantiate game scenes here
  // Breakout* breakout = new Breakout();
  SplashScreen* splash = new SplashScreen();
  Menu* menu = new Menu(&selectedShip);
  Stage* stage = new Stage(&selectedShip);
  StageSelect* stageSelect = new StageSelect(&selectedStage);

  int selectedShip;
  int selectedStage;

public:
  void setupRootScene()
  {
    // Init and set root scene
    this->setScene(splash);
  }

  void nextScene(Scene* currentScene)
  {
    if (currentScene == splash) {
      this->setScene(stageSelect);
    } else if (currentScene == stageSelect) {
      this->setScene(menu);
    } else if (currentScene == menu) {
      this->setScene(stage);
    }
  }
};