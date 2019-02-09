#pragma once

#include "common.h"
#include "constants.h"
#include "input.h"
#include "math/lerp.h"
#include "menu.h"
#include "scene.h"
#include "stage.h"
#include "systems/animation.h"

struct CStageSelectControlled : Component<CStageSelectControlled> {
};

class SStageSelect : public System
{
  Input* input;
  Stage* stageScene;
  Game* game;

public:
  uint32* currentStageSelected;
  SStageSelect(Input* _input, Stage* _stageScene, Game* _game) : System()
  {
    System::addComponentType(CStageSelectControlled::id);
    System::addComponentType(CSprite::id);

    this->input = _input;
    this->stageScene = _stageScene;
    this->game = _game;
  }
  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CStageSelectControlled* controlComponent =
        (CStageSelectControlled*)components[0];
    CSprite* stagenumberSprite = (CSprite*)components[1];

    if (input->getKeyboardKeyState(VK_LEFT) == JustPressed) {
      if (stagenumberSprite->currentFrame > stagenumberSprite->startFrame)
        stagenumberSprite->currentFrame -= 1;
      stagenumberSprite->setRect();
    } else if (input->getKeyboardKeyState(VK_RIGHT) == JustPressed) {
      if (stagenumberSprite->currentFrame < stagenumberSprite->endFrame)
        stagenumberSprite->currentFrame += 1;
      stagenumberSprite->setRect();
    }

    if (input->getKeyboardKeyState(VK_SPACE) == JustPressed) {
      game->setScene(stageScene);
    }
  }
};

class StageSelect : public Scene
{
  TextureManager backgroundTexture, promptTexture, stagenumberTexture, gameTitleTexture;

  // Systems
  // SAnimation* menuAnimation;
  SStageSelect* stageSelect;

  // Components
  CSprite backgroundImage, promptSprite, titleSprite, stagenumberSprite;
  CAnimated stagenumberAnimation, titleAnimation;
  CStageSelectControlled stageSelectControls;

  // Entities
  EntityHook background;
  EntityHook title;
  EntityHook stagenumber;

  Stage* stageScene = new Stage();
  uint32 currentStageSelected = 0;
  //int stagebutton[rows][columns];

public:
  StageSelect() : Scene() {}
  ~StageSelect()
  {
    backgroundTexture.onLostDevice();
    backgroundTexture.onResetDevice();
    promptTexture.onLostDevice();
    promptTexture.onResetDevice();
    stagenumberTexture.onLostDevice();
    stagenumberTexture.onResetDevice();
    gameTitleTexture.onLostDevice();
    gameTitleTexture.onResetDevice();
  }

  void setupSystems()
  {

    stageSelect = new SStageSelect(input, stageScene, game);
    stageSelect->currentStageSelected = &currentStageSelected;
    /*for (int row = 0; row < rows; row++) {
      for (int column = 0; column < columns; column++) {
        stagebutton[row][column] = 1;
      }
    }*/

  }
  void setupTextures()
  {
    if (!backgroundTexture.initialise(graphics, BACKGROUND))
      Logger::error("Failed to load background texture");
    if (!stagenumberTexture.initialise(graphics, STAGE_NUMBER))
      Logger::error("Failed to load stage number texture");
    if (!promptTexture.initialise(graphics, PROMPT))
      Logger::error("Fail to load prompt texture");
    if (!gameTitleTexture.initialise(graphics, GAME_LOGO))
      Logger::error("Fail to load game title texture");
	
  }

  void setupComponents()
  {
    backgroundImage.startFrame = 0, backgroundImage.endFrame = 0,
    backgroundImage.currentFrame = 0;
    backgroundImage.animates = false;
    backgroundImage.initialise(BACKGROUND_WIDTH, BACKGROUND_HEIGHT,
                               BACKGROUND_COLS, &backgroundTexture);
    backgroundImage.setScale(0.5);
    backgroundImage.setPosition(0, 0);

    stagenumberSprite.startFrame = 0, stagenumberSprite.endFrame = 9,
    stagenumberSprite.currentFrame = 0;
    stagenumberSprite.animates = false;
    stagenumberSprite.initialise(STAGE_NUMBER_WIDTH, STAGE_NUMBER_HEIGHT,
                                 STAGE_NUMBER_COLS, &stagenumberTexture);
    stagenumberSprite.setScale(1.5);
    stagenumberSprite.setPosition(
        GAME_WIDTH / 2 - stagenumberSprite.getWidth() / 2,
        (GAME_HEIGHT / 2 - stagenumberSprite.getHeight() / 2) + 70);


	/*stagenumberAnimation.animationType = SCALE;
    stagenumberAnimation.startValue = 1.5, stagenumberAnimation.endValue = 1.7,
    stagenumberAnimation.reverses = true, stagenumberAnimation.rate = 0.06;*/

	titleSprite.startFrame = 0, titleSprite.endFrame = 2;
    titleSprite.currentFrame = 0;
    titleSprite.animates = true;
    titleSprite.initialise(GAME_LOGO_WIDTH, GAME_LOGO_HEIGHT, GAME_LOGO_COLS,
                           &gameTitleTexture);
    titleSprite.frameDelay = 0.1;
    titleSprite.setScale(0.7);
    titleSprite.setPosition(GAME_WIDTH / 2 - titleSprite.getWidth() / 2, 50);

    promptSprite.startFrame = 0, promptSprite.endFrame = 0,
    promptSprite.currentFrame = 0;
    promptSprite.animates = false;
    promptSprite.initialise(PROMPT_WIDTH, PROMPT_HEIGHT, PROMPT_COLS,
                            &promptTexture);
    promptSprite.setScale(0.7);
    promptSprite.setPosition(GAME_WIDTH / 2 - promptSprite.getWidth() / 2,
                             GAME_HEIGHT / 2 + promptSprite.getHeight() * 3);
    
	/*for (int column = 0; column < columns; column++) {
      for (int row = 0; row < rows; row++) {

        stagenumberSprite.setPosition((GAME_WIDTH/6)*column,
                               (GAME_LOGO_HEIGHT/2)*row);
        currentStageSelected = currentStageSelected + 1;

        if (stagebutton[row][column] == 1) {
          graphics->spriteBegin();
          stagenumberSprite.currentFrame = currentStageSelected; 
          stagenumberSprite.spriteData.texture =
              stagenumberSprite.textureManager->getTexture();
          graphics->drawSprite(stagenumberSprite.spriteData);
          graphics->spriteEnd();
		  }
		}
	  }

    /*for (int i = 0; i < STAGE_NUMBER_COLS + 1; i++)
    {
		stageOneSprite.currentFrame = i;
		stageOneSprite.animates = false;
		stageOneSprite.initialise(STAGE_NUMBER_WIDTH, STAGE_NUMBER_HEIGHT,
                            STAGE_NUMBER_COLS, &stagenumberTexture);
		stageOneSprite.setScale(0.2);
		stageOneSprite.setPosition((GAME_WIDTH / 9) * i, GAME_HEIGHT / 2);


    }*/
  }

  void update(float delta){}

  void render()
  {
    promptSprite.spriteData.texture = promptSprite.textureManager->getTexture();
    graphics->spriteBegin();

    graphics->drawSprite(promptSprite.spriteData);

    graphics->spriteEnd();
  }
  void attach()
  {
    gameSystems->addSystem(*stageSelect);
    background = ecs->makeEntity(backgroundImage);
    title = ecs->makeEntity(titleSprite);
    stagenumber = ecs->makeEntity(stagenumberSprite, stagenumberAnimation,
                                  stageSelectControls);
	  Scene::attach(); 
  }
  void detach()
  {
    gameSystems->removeSystem(*stageSelect);
    ecs->removeEntity(title);
    ecs->removeEntity(background);
    ecs->removeEntity(stagenumber);
	  Scene::detach();
  }
};