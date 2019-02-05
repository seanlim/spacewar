#pragma once
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include <D3dx9core.h>

class Text
{
  Graphics* graphics;
  D3DXMATRIX matrix;
  float angle;
  COLOR_ARGB color;
  RECT fontRect;
  LP_DXFONT dxFont;

public:
  Text();
  virtual ~Text();

  bool initialise(Graphics* g, int height, bool bold, bool italic,
                  const std::string& fontName);
  int print(const std::string& str, int x, int y);
  int print(const std::string& str, int x, int y, UINT format);

  // Getters
  float getDegrees() { return angle * (180.0f / (float)PI); }
  float getRadians() { return angle; }
  float getFontColor() { return color; }

  // Setters
  void setDegrees(float deg) { angle = deg * ((float)PI / 180.0f); }
  void setRadians(float rad) { angle = rad; }
  void setFontColor(COLOR_ARGB c) { color = c; }

  void onLostDevice();
  void onResetDevice();
};