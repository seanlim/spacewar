#include "breakout.h"

Breakout::Breakout() {}

Breakout::~Breakout()
{
  this->tileTexture.onLostDevice();
  this->tileTexture.onResetDevice();
}

void Breakout::setupSystems()
{
  // Set collision system bounds
  collisionSystem->setBounds({marginX, 0, GAME_WIDTH - marginX, GAME_HEIGHT});

  // Tile map system
  tileMapSystem = new SBreakOutTileMap(this->hwnd, GAME_WIDTH, GAME_HEIGHT,
                                       FULLSCREEN, graphics);
  graphicsSystems->addSystem(*tileMapSystem);

  // Player controls
  playerControlSystem = new SPlayerControlled(input);
  gameSystems->addSystem(*playerControlSystem);
}
void Breakout::setupTextures()
{
  // Load textures
  if (!tileTexture.initialise(graphics, TILE_IMAGE))
    Logger::error("Failed to load tile textures");
  if (!paddleBallTexture.initialise(graphics, PADDLE_BALL))
    Logger::error("Failed to load paddle and ball textures");
}

void Breakout::setupEntities()
{
  // Init tile sprite
  CSprite tileSprite;
  tileSprite.startFrame = 0, tileSprite.endFrame = 0,
  tileSprite.currentFrame = 0;
  tileSprite.initialise(64, 64, 1, &tileTexture);
  tileSprite.setScale(0.5);
  tileMapSystem->tileSprite = tileSprite;

  // Init ball
  CSprite ballSprite;
  ballSprite.startFrame = 0, ballSprite.endFrame = 7,
  ballSprite.currentFrame = 0;
  ballSprite.initialise(32, 32, 4, &paddleBallTexture);
  ballSprite.setScale(0.6);
  ballSprite.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
  CMotion ballMotion;
  ballMotion.velocity = Vec2(0.0, 200);
  ballMotion.friction = 0.0;
  CCollidable ballCollision;
  ballCollision.collisionType = CIRCLE;
  ballCollision.collisionResponse = BOUNCE;
  CTileMapCollider tileMapCollider;

  ecs->makeEntity(ballSprite, ballMotion, ballCollision, tileMapCollider);

  // Init paddle
  CSprite paddleSprite;
  paddleSprite.startFrame = 7, paddleSprite.endFrame = 11,
  paddleSprite.currentFrame = 8;
  paddleSprite.initialise(128, 32, 1, &paddleBallTexture);
  paddleSprite.setScale(0.6);
  paddleSprite.setPosition(marginX, GAME_HEIGHT - paddleSprite.getHeight());
  CMotion paddleMotion;
  CPlayerControlled paddleControls;
  CCollidable paddleCollision;
  paddleCollision.collisionType = ORIENTED_BOX;
  paddleCollision.collisionResponse = NONE;
  ecs->makeEntity(paddleControls, paddleSprite, paddleMotion, paddleCollision);

  return;
}

void Breakout::render()
{
  // graphics->spriteBegin();
  // graphics->spriteEnd();
}