#pragma once
#define WIN_32_LEAN_AND_MEAN

#include "logger.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

// Constants
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;
const char CLASS_NAME[] = "Ponyo";
const char GAME_TITLE[] = "Breakout!";
const bool FULLSCREEN = false; // windowed or fullscreen
const UINT GAME_WIDTH = 1280;  // width of game in pixels
const UINT GAME_HEIGHT = 720;  // height of game in pixels

// Typedefs
typedef unsigned int uint32;
typedef RECT Rect;

// Safely delete pointer referenced item
#define safeDelete(ptr)                                                        \
  {                                                                            \
    if (ptr) {                                                                 \
      delete (ptr);                                                            \
      (ptr) = NULL;                                                            \
    }                                                                          \
  }
// Safely release pointer referenced item
#define safeRelease(ptr)                                                       \
  {                                                                            \
    if (ptr) {                                                                 \
      (ptr)->Release();                                                        \
      (ptr) = NULL;                                                            \
    }                                                                          \
  }
// Safely delete pointer referenced array
#define safeDeleteArr(ptr)                                                     \
  {                                                                            \
    if (ptr) {                                                                 \
      delete[](ptr);                                                           \
      (ptr) = NULL;                                                            \
    }                                                                          \
  }

// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)                                               \
  {                                                                            \
    if (ptr) {                                                                 \
      ptr->onLostDevice();                                                     \
    }                                                                          \
  }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)                                              \
  {                                                                            \
    if (ptr) {                                                                 \
      ptr->onResetDevice();                                                    \
    }                                                                          \
  }
#define TRANSCOLOR SETCOLOR_ARGB(0, 255, 0, 255) // transparent color (magenta)

// DirectX
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D LPDIRECT3D9
#define LP_TEXTURE LPDIRECT3DTEXTURE9
#define LP_SPRITE LPD3DXSPRITE
#define LP_DXFONT LPD3DXFONT
#define Vec2 D3DXVECTOR2

// Game keyboard commands
enum GameCommands { Quit, toggleFPS, toggleDebug };

// Collision
enum CollisionResponse { BOUNCE, NONE };
enum CollisionType { BOX, CIRCLE, ORIENTED_BOX };
