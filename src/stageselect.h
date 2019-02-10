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
class StageSelect;
class SStageSelect : public System
{
  Input* input;
  Game* game;
  StageSelect* stageselect;
  int* selectedStage;

public:
  // uint32* currentStageSelected;
  // int stageselectedNow = 0;
  // StageNumber stagenumber;

  SStageSelect(Input* _input, int* _selectedStage, Game* _game,
               StageSelect* _stageselect)
      : System()
  {
    System::addComponentType(CStageSelectControlled::id);
    System::addComponentType(CSprite::id);

    this->input = _input;
    this->game = _game;
    this->stageselect = _stageselect;
    this->selectedStage = _selectedStage;
  }
  virtual void updateComponents(float delta, BaseComponent** components)
  {
    CStageSelectControlled* controlComponent =
        (CStageSelectControlled*)components[0];
    CSprite* stagenumberSprite = (CSprite*)components[1];
    int j = 4;
    int k = 1;
    if (*selectedStage <= 4 ) {
      stagenumberSprite->setPosition(
          ((GAME_WIDTH / 11) * (2 * ((*selectedStage) + 1))) -
              stagenumberSprite->getWidth() ,
          ((GAME_HEIGHT / 7) * 4) - stagenumberSprite->getHeight() / 2 );
    } else if (*selectedStage >= 5)
		{

		stagenumberSprite->setPosition(
          ((GAME_WIDTH / 11) * (2 * (((*selectedStage)-5) + 1))) -
              stagenumberSprite->getWidth(),
          ((GAME_HEIGHT / 7) * 5) - stagenumberSprite->getHeight() / 2);
	  }

        if (input->getKeyboardKeyState(VK_LEFT) == JustPressed) {
      if (*selectedStage > 0) *selectedStage -= 1;
      Logger::println(*selectedStage);
    } else if (input->getKeyboardKeyState(VK_RIGHT) == JustPressed) {
      if (*selectedStage < 9) *selectedStage += 1;
      Logger::println(*selectedStage);
    }

	if (input->getKeyboardKeyState(VK_DOWN) == JustPressed) {
      if (*selectedStage <= 4) *selectedStage += 5;
	}

	if (input->getKeyboardKeyState(VK_UP) == JustPressed) {
          if (*selectedStage >= 5) *selectedStage -= 5;
        }
    if (input->getKeyboardKeyState(VK_SPACE) == JustPressed) {

      game->nextScene((Scene*)this->stageselect);
    }
  }
};

class StageSelect : public Scene
{
  TextureManager backgroundTexture, promptTexture, stagenumberTexture,
      gameTitleTexture, yellowboxTexture;

  // Systems
  // SAnimation* menuAnimation;
  SStageSelect* stageSelect;

  // Components
  CSprite backgroundImage, promptSprite, titleSprite, stagenumberSprite, yellowboxSprite;
  CAnimated stagenumberAnimation, titleAnimation;
  CStageSelectControlled stageSelectControls;

  // Entities
  EntityHook background;
  EntityHook title;
  EntityHook stagenumber;

  int* selectedStage;

public:
  StageSelect(int* _selectedStage) : Scene()
  {
    this->selectedStage = _selectedStage;
  }
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
    yellowboxTexture.onLostDevice();
    yellowboxTexture.onResetDevice();
  }

  void setupSystems()
  {

    stageSelect = new SStageSelect(input, selectedStage, game, this);
    // stageSelect->currentStageSelected = &currentStageSelected;
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
    if (!yellowboxTexture.initialise(graphics, YELLOW_BOX))
      Logger::error("Fail to load yellow box texture");
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
    /*stagenumberSprite.initialise(STAGE_NUMBER_WIDTH, STAGE_NUMBER_HEIGHT,
                                 STAGE_NUMBER_COLS, &stagenumberTexture);
    stagenumberSprite.setScale(1.5);
    stagenumberSprite.setPosition(
        GAME_WIDTH / 2 - stagenumberSprite.getWidth() / 2,
        (GAME_HEIGHT / 2 - stagenumberSprite.getHeight() / 2) + 70);*/

    stagenumberSprite.currentFrame = 0;
    stagenumberSprite.initialise(STAGE_NUMBER_WIDTH, STAGE_NUMBER_HEIGHT,
                                 STAGE_NUMBER_COLS, &stagenumberTexture);
    stagenumberSprite.setScale(1.5);
   /* stagenumberSprite.setPosition(
        (GAME_WIDTH / 11) * 2 - stagenumberSprite.getWidth(),
        ((GAME_HEIGHT / 7) * 4) - stagenumberSprite.getHeight() / 2);
    stagenumberAnimation.animations.push_back(
        {SCALE, 1.6, 1.8, 0.06, true, false, true});*/

    /*int j = 4;
    int k = 1;
    for (int i = 0; i < (stagenumberSprite.endFrame + 1); i++) {
      stagenumberSprite.currentFrame = i;
      stagenumberSprite.initialise(STAGE_NUMBER_WIDTH, STAGE_NUMBER_HEIGHT,
                                   STAGE_NUMBER_COLS, &stagenumberTexture);
      stagenumberSprite.setScale(1.5);
      stagenumberSprite.setPosition(
          ((GAME_WIDTH / 11) * (k * 2)) - stagenumberSprite.getWidth(),
          ((GAME_HEIGHT / 7) * j) - stagenumberSprite.getHeight() / 2);
      k = k + 1;
      if (((i + 1) % 5) == 0) {
        j = j + 1;
        k = 1;
        stagenumberAnimation.animations.push_back(
            {SCALE, 1.5, 1.7, 0.06, true, false, true});
      }
    }*/

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

	yellowboxSprite.startFrame = 0, yellowboxSprite.endFrame = 0,
    yellowboxSprite.currentFrame = 0;
    yellowboxSprite.animates = false;
        yellowboxSprite.initialise(YELLOWBOX_WIDTH, YELLOWBOX_HEIGHT,
                                   YELLOWBOX_COL, &yellowboxTexture);
    yellowboxSprite.setScale(0.25);
        stagenumberAnimation.animations.push_back(
            {SCALE, 0.25, 0.45, 0.06, true, false, true});

  }

  void update(float delta) {}

  void render()
  {

    int j = 4;
    int k = 1;
    promptSprite.spriteData.texture = promptSprite.textureManager->getTexture();
    for (int i = 0; i < (stagenumberSprite.endFrame + 1); i++) {
      stagenumberSprite.currentFrame = i;
      stagenumberSprite.initialise(STAGE_NUMBER_WIDTH, STAGE_NUMBER_HEIGHT,
                                   STAGE_NUMBER_COLS, &stagenumberTexture);
      stagenumberSprite.setScale(1.5);
      stagenumberSprite.setPosition(
          ((GAME_WIDTH / 11) * (k * 2)) - stagenumberSprite.getWidth(),
          ((GAME_HEIGHT / 7) * j) - stagenumberSprite.getHeight() / 2);
      stagenumberAnimation.animations.push_back(
          {SCALE, 1.5, 1.7, 0.06, true, false, true});
      k = k + 1;
      if (((i + 1) % 5) == 0) {
        j = j + 1;
        k = 1;
      } else {
      }
      graphics->spriteBegin();
      graphics->drawSprite(stagenumberSprite.spriteData);
      graphics->spriteEnd();
    }

    graphics->spriteBegin();

    graphics->drawSprite(promptSprite.spriteData);

    graphics->spriteEnd();
  }
  void attach()
  {
    gameSystems->addSystem(*stageSelect);
    background = ecs->makeEntity(backgroundImage);
    title = ecs->makeEntity(titleSprite);
    stagenumber = ecs->makeEntity(yellowboxSprite, stagenumberAnimation,
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