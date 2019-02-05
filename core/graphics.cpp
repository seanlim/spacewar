#include "graphics.h"

Graphics::Graphics()
{
  this->direct3d, device3d = NULL;
  this->fullscreen = false;
  this->width = GAME_WIDTH;
  this->height = GAME_HEIGHT;
  this->setBackColor(graphicsNS::BACK_COLOR);
}

void Graphics::initD3Dpp()
{
  try {
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;

    // Switch to 24-bit color on fullscreen
    d3dpp.BackBufferFormat = fullscreen ? D3DFMT_X8R8G8B8 : D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 1;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = !fullscreen;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  } catch (...) {
    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialising D3D params"));
  }
}

void Graphics::initialise(HWND hw, int w, int h, bool fs)
{
  hwnd = hw;
  width = w;
  height = h;
  fullscreen = fs;

  // Init Direct3D
  direct3d = Direct3DCreate9(D3D_SDK_VERSION);
  if (direct3d == NULL)
    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialising Direct3D"));

  initD3Dpp();

  if (fullscreen) {
    if (isAdapterCompatible())
      // set the refresh rate with a compatible one
      d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
    else
      throw(GameError(gameErrorNS::FATAL_ERROR,
                      "The graphics device does not support the specified "
                      "resolution and/or format."));
  }

  // determine if graphics card supports harware texturing and lighting and
  // vertex shaders
  D3DCAPS9 caps;
  DWORD behavior;
  result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
  // If device doesn't support HW T&L or doesn't support 1.1 vertex
  // shaders in hardware, then switch to software vertex processing.
  if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
      caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
    behavior =
        D3DCREATE_SOFTWARE_VERTEXPROCESSING; // use software only processing
  else
    behavior =
        D3DCREATE_HARDWARE_VERTEXPROCESSING; // use hardware only processing

  // create Direct3D device
  result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                  behavior, &d3dpp, &device3d);

  if (FAILED(result))
    throw(
        GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

  result = D3DXCreateSprite(this->device3d, &sprite);

  if (FAILED(result))
    throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating sprite"));
}

bool Graphics::isAdapterCompatible()
{
  UINT modes =
      direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);
  for (UINT i = 0; i < modes; i++) {
    result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
                                        d3dpp.BackBufferFormat, i, &pMode);
    if (pMode.Height == d3dpp.BackBufferHeight &&
        pMode.Width == d3dpp.BackBufferWidth &&
        pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
      return true;
  }
  return false;
}

void Graphics::setBackColor(COLOR_ARGB c) { this->backColor = c; }

HRESULT Graphics::showBackbuffer()
{
  result = E_FAIL;

  result = device3d->Present(NULL, NULL, NULL, NULL);

  return result;
}

HRESULT Graphics::getDeviceState()
{
  this->result = E_FAIL;

  if (device3d == NULL) return this->result;

  this->result = this->device3d->TestCooperativeLevel();
  return result;
}

HRESULT Graphics::reset()
{
  result = E_FAIL;
  this->initD3Dpp();
  // Reset graphics device
  this->result = this->device3d->Reset(&d3dpp);
  return result;
}

HRESULT Graphics::beginScene()
{
  this->result = E_FAIL;
  if (this->device3d == NULL) return this->result;
  this->device3d->Clear(0, NULL, D3DCLEAR_TARGET, this->backColor, 1.0F, 0);
  this->result = this->device3d->BeginScene();
  return this->result;
}

HRESULT Graphics::endScene()
{
  this->result = E_FAIL;
  if (this->device3d) this->result = this->device3d->EndScene();
  return this->result;
}

HRESULT Graphics::loadTexture(const char* filename, COLOR_ARGB transcolor,
                              UINT& width, UINT& height, LP_TEXTURE& texture)
{
  D3DXIMAGE_INFO info;
  result = E_FAIL;

  try {
    if (filename == NULL) {

      texture = NULL;
      return D3DERR_INVALIDCALL;
    }

    result = D3DXGetImageInfoFromFile(filename, &info);
    if (result != D3D_OK) return result;

    width = info.Width;
    height = info.Height;
    result = D3DXCreateTextureFromFileEx(
        device3d, filename, info.Width, info.Height,
        1,               // mip-map levels (1 for no chain)
        0,               // usage
        D3DFMT_UNKNOWN,  // surface format (default)
        D3DPOOL_DEFAULT, // memory class for the texture
        D3DX_DEFAULT,    // image filter
        D3DX_DEFAULT,    // mip filter
        transcolor,      // color key for transparency
        &info,           // bitmap file info (from loaded file)
        NULL,            // color palette
        &texture);       // destination texture
  } catch (...) {
    throw(
        GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
  }
  return result;
}

void Graphics::drawSprite(const SpriteData& spriteData, COLOR_ARGB color)
{
  if (spriteData.texture == NULL) return;
  // Center of sprite
  Vec2 spriteCenter = Vec2((float)(spriteData.width / 2 * spriteData.scale),
                           (float)(spriteData.height / 2 * spriteData.scale));
  // Screen position of the sprite
  Vec2 translate = Vec2((float)spriteData.x, (float)spriteData.y);
  // Scaling
  Vec2 scaling(spriteData.scale, spriteData.scale);

  if (spriteData.flipHorizontal) {
    scaling.x *= -1;
    spriteCenter.x -= (float)(spriteData.width * spriteData.scale);
    translate.x += (float)(spriteData.width * spriteData.scale);
  }

  if (spriteData.flipVertical) {
    scaling.y *= -1;
    spriteCenter.y -= (float)(spriteData.height * spriteData.scale);
    translate.y += (float)(spriteData.height * spriteData.scale);
  }

  D3DXMATRIX matrix;
  D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, &scaling, &spriteCenter,
                             (float)spriteData.angle, &translate);

  sprite->SetTransform(&matrix);

  sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

Graphics::~Graphics() { releaseAll(); }

void Graphics::releaseAll()
{
  safeRelease(device3d);
  safeRelease(direct3d);
}
