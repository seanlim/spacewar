#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsX.h>

#include <XInput.h>
#include <array>
#include <string>
#include <unordered_map>

#include "array.h"
#include "common.h"
#include "gameError.h"

// High-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT)0x02)
#endif

namespace inputNS
{
const int KEYS_ARRAY_LEN = 256;

const UCHAR KEYS_DOWN = 1;
const UCHAR KEYS_PRESSED = 2;
const UCHAR MOUSE = 4;
const UCHAR TEXT_IN = 8;

const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;

} // namespace inputNS

const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0x7FFF);
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;
const DWORD MAX_CONTROLLERS = 4;

// Bit corresponding to gamepad button in state.Gamepad.wButtons
const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

struct ControllerState {
  XINPUT_STATE state;
  XINPUT_VIBRATION vibration;
  float vibrateTimeLeft;
  float vibrateTimeRight;
  bool connected;
};

enum KeyState { Released, JustReleased, Pressed, JustPressed };

struct KeyBinding {
private:
  uint32 keyCode;
  KeyState keyState;

public:
  KeyBinding() : keyCode(0), keyState(KeyState::JustReleased){};
  KeyBinding(uint32 keyCode, KeyState keyState)
      : keyCode(keyCode), keyState(keyState){};
  ~KeyBinding(){};

  friend struct GameCommand;
  friend class Input;
};

struct GameCommand {
private:
  std::string name;
  std::vector<KeyBinding> chord;

public:
  GameCommand() : name(""), chord(0){};
  GameCommand(std::string name, const uint32 keyCode, const KeyState keyState)
      : name(name)
  {
    chord.push_back(KeyBinding(keyCode, keyState));
  };
  GameCommand(std::string name, const KeyBinding& keyBinding) : name(name)
  {
    chord.push_back(keyBinding);
  };
  GameCommand(std::string name, const std::vector<KeyBinding>& chord)
      : name(name), chord(chord){};
  ~GameCommand(){};

  friend class Input;
};

class Input
{
private:
  // Keyboard
  std::unordered_map<GameCommands, GameCommand*> keyMap;
  std::unordered_map<GameCommands, GameCommand*> activeKeyMap;
  std::array<BYTE, inputNS::KEYS_ARRAY_LEN> keyboardState;
  std::array<BYTE, inputNS::KEYS_ARRAY_LEN> keyboardStateBuffer;
  inline const bool isPressed(int keyCode) const
  {
    return (GetAsyncKeyState(keyCode) & 0x8000) ? 1 : 0;
  };

  // Mouse
  int mouseX, mouseY;
  int mouseRawX, mouseRawY;
  RAWINPUTDEVICE Rid[1];
  bool mouseCaptured;
  bool mouseLButton = false;
  bool mouseMButton = false;
  bool mouseRButton = false;
  bool mouseX1Button = false;
  bool mouseX2Button = false;

  // Controllers
  ControllerState controllers[MAX_CONTROLLERS];
  const ControllerState* getControllerState(UINT n)
  {
    return &controllers[(n > MAX_CONTROLLERS - 1) ? MAX_CONTROLLERS - 1 : n];
  }

public:
  Input();
  virtual ~Input();
  void initialise(HWND, bool);

  // Keyboard
  void pollKeys();
  const KeyState getKeyboardKeyState(const uint32 keyCode) const;
  std::vector<GameCommands> getActiveGameCommands();

  // Mouse
  void mouseIn(LPARAM);
  void mouseRawIn(LPARAM);

  void setMouseLButton(bool b) { this->mouseLButton = b; }
  void setMouseMButton(bool b) { this->mouseMButton = b; }
  void setMouseRButton(bool b) { this->mouseRButton = b; }

  void setMouseXButton(WPARAM wParam)
  {
    this->mouseX1Button = (bool)(wParam & MK_XBUTTON1);
    this->mouseX2Button = (bool)(wParam & MK_XBUTTON2);
  }

  int getMouseX() const { return this->mouseX; }
  int getMouseY() const { return this->mouseY; }

  int getMouseRawX() const { return this->mouseRawX; }
  int getMouseRawY() const { return this->mouseRawY; }

  bool getMouseLButton() const { return mouseLButton; }
  bool getMouseMButton() const { return mouseMButton; }
  bool getMouseRButton() const { return mouseRButton; }
  bool getMouseX1Button() const { return mouseX1Button; }
  bool getMouseX2Button() const { return mouseX2Button; }

  void checkControllers();
  void readControllers();

  const WORD getGamepadButtons(UINT n)
  {
    return (*this->getControllerState(n)).state.Gamepad.wButtons;
  }

  bool getGamepadButtonState(UINT n, DWORD bitPattern)
  {
    return (this->getGamepadButtons(n) & bitPattern) != 0;
  }

  BYTE getGamepadLeftTrigger(UINT n)
  {
    return (*this->getControllerState(n)).state.Gamepad.bLeftTrigger;
  }

  BYTE getGamepadRightTrigger(UINT n)
  {
    return (*this->getControllerState(n)).state.Gamepad.bRightTrigger;
  }

  SHORT getGamePadThumbLX(UINT n)
  {
    return (*this->getControllerState(n)).state.Gamepad.sThumbLX;
  }

  SHORT getGamePadThumbLY(UINT n)
  {
    return (*this->getControllerState(n)).state.Gamepad.sThumbLY;
  }

  SHORT getGamePadThumbRX(UINT n)
  {
    return (*this->getControllerState(n)).state.Gamepad.sThumbRX;
  }

  SHORT getGamePadThumbRY(UINT n)
  {
    return (*this->getControllerState(n)).state.Gamepad.sThumbRY;
  }

  // Gamepad vibrate
  // left: low freq, right: high freq
  void gamePadVibrateLeft(UINT n, WORD speed, float sec)
  {
    if (n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
    this->controllers[n].vibration.wLeftMotorSpeed = speed;
    this->controllers[n].vibrateTimeLeft = sec;
  }

  void gamePadVibrateRight(UINT n, WORD speed, float sec)
  {
    if (n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
    this->controllers[n].vibration.wRightMotorSpeed = speed;
    this->controllers[n].vibrateTimeRight = sec;
  }

  void vibrateControllers(float frameTime);
};
