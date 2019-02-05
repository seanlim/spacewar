#pragma once
#define WIN32_LEAN_AND_MEAN

#define D3D_DEBUG_INFO

#include "common.h"
#include "gameError.h"
#include "math/vector2.h"

// Colors
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a, r, g, b)                                              \
  ((COLOR_ARGB)((((a)&0xff) << 24) | (((r)&0xff) << 16) | (((g)&0xff) << 8) |  \
                ((b)&0xff)))

namespace graphicsNS
{
const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
const COLOR_ARGB FILTER =
    D3DCOLOR_ARGB(0, 0, 0, 0); // use to specify drawing with colorFilter
const COLOR_ARGB ALPHA25 =
    D3DCOLOR_ARGB(64, 255, 255, 255); // AND with color to get 25% alpha
const COLOR_ARGB ALPHA50 =
    D3DCOLOR_ARGB(128, 255, 255, 255); // AND with color to get 50% alpha
const COLOR_ARGB BACK_COLOR = BLACK;   // background color of game

enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
} // namespace graphicsNS

struct SpriteData {
  int width;  // width of sprite in pixels
  int height; // height of sprite in pixels
  float x;    // screen location (top left corner of sprite)
  float y;
  float scale;         // <1 smaller, >1 bigger
  float angle;         // rotation angle in radians
  RECT rect;           // used to select an image from a larger texture
  LP_TEXTURE texture;  // pointer to texture
  bool flipHorizontal; // true to flip sprite horizontally (mirror)
  bool flipVertical;   // true to flip sprite vertically
};

class Graphics
{
private:
  LP_3D direct3d;
  LP_3DDEVICE device3d;
  LP_SPRITE sprite;
  D3DPRESENT_PARAMETERS d3dpp;
  D3DDISPLAYMODE pMode;

  HRESULT result;
  HWND hwnd;
  COLOR_ARGB backColor;
  bool fullscreen;
  int width, height;

  void initD3Dpp();

public:
  Graphics();
  virtual ~Graphics();

  // Getters
  LP_3D get3D() { return direct3d; }
  LP_3DDEVICE get3DDevice() { return device3d; }
  HDC getDC() { return GetDC(hwnd); }
  LP_SPRITE getSprite() { return sprite; }

  void setBackColor(COLOR_ARGB c);
  void releaseAll();
  void initialise(HWND hw, int width, int height, bool fullscreen);
  bool isAdapterCompatible();

  HRESULT showBackbuffer();
  HRESULT reset();
  HRESULT getDeviceState();
  HRESULT beginScene();

  HRESULT endScene();
  HRESULT loadTexture(const char*, COLOR_ARGB, UINT&, UINT&, LP_TEXTURE&);
  void Graphics::drawSprite(const SpriteData&,
                            COLOR_ARGB color = graphicsNS::WHITE);

  void spriteBegin() { sprite->Begin(D3DXSPRITE_ALPHABLEND); }

  void spriteEnd() { sprite->End(); }
};