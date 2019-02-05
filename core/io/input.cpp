#include "input.h"

Input::Input()
{

  this->mouseX, this->mouseY, this->mouseRawX, this->mouseRawY = 0;

  this->mouseLButton, this->mouseMButton, this->mouseRButton,
      this->mouseX1Button, this->mouseX2Button = false;

  for (int i = 0; i < MAX_CONTROLLERS; i++)
    this->controllers[i].vibrateTimeLeft,
        this->controllers[i].vibrateTimeRight = 0;

  // Set default keymap
  keyMap.clear();

  // Define chord for quit
  Array<KeyBinding> quitChord;
  quitChord.push_back(KeyBinding(VK_SHIFT, KeyState::Pressed));
  quitChord.push_back(KeyBinding(0x51, KeyState::JustPressed));

  keyMap[GameCommands::Quit] = new GameCommand("Quit", quitChord);
  keyMap[GameCommands::toggleFPS] =
      new GameCommand("Toggle FPS", VK_F2, KeyState::JustPressed);
  keyMap[GameCommands::toggleDebug] =
      new GameCommand("Toggle debug", VK_F3, KeyState::JustPressed);
}

Input::~Input()
{
  if (this->mouseCaptured) ReleaseCapture();
}

void Input::initialise(HWND hwnd, bool capture)
{
  try {
    this->mouseCaptured = capture;

    this->Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    this->Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    this->Rid[0].dwFlags = RIDEV_INPUTSINK;
    this->Rid[0].hwndTarget = hwnd;

    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

    if (this->mouseCaptured) SetCapture(hwnd);

    // Clear controller memory
    ZeroMemory(this->controllers, sizeof(ControllerState), *MAX_CONTROLLERS);
    this->checkControllers();
  } catch (...) {
    throw(
        GameError(gameErrorNS::FATAL_ERROR, "Error initialising input system"));
  }
}

void Input::pollKeys()
{
  // Update state and buffer
  keyboardStateBuffer = keyboardState;
  for (int i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
    keyboardState[i] = isPressed(i);

  // Update bindings
  bool isActive = false;
  activeKeyMap.clear();

  for (auto x : keyMap) {
    isActive = true;
    for (auto y : x.second->chord) {
      if (getKeyboardKeyState(y.keyCode) != y.keyState) {
        isActive = false;
        break;
      }
    }

    if (isActive)
      activeKeyMap.insert(
          std::pair<GameCommands, GameCommand*>(x.first, x.second));
  }
}

const KeyState Input::getKeyboardKeyState(const uint32 keyCode) const
{
  const int curr = keyboardState[keyCode];
  const int prev = keyboardStateBuffer[keyCode];

  if (prev == 1) {
    if (curr == 1)
      return KeyState::Pressed;
    else
      return KeyState::JustReleased;
  } else {
    if (curr == 1)
      return KeyState::JustPressed;
    else
      return KeyState::Released;
  }
}

std::vector<GameCommands> Input::getActiveGameCommands()
{
  std::vector<GameCommands> activeGameCommands;
  activeGameCommands.reserve(activeKeyMap.size());
  for (auto x : activeKeyMap) activeGameCommands.push_back(x.first);
  return activeGameCommands;
}

void Input::mouseIn(LPARAM lParam)
{
  this->mouseX = GET_X_LPARAM(lParam);
  this->mouseY = GET_Y_LPARAM(lParam);
}

void Input::mouseRawIn(LPARAM lParam)
{
  UINT dwSize = 40;
  static BYTE lpb[40];

  GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
                  sizeof(RAWINPUTHEADER));

  RAWINPUT* raw = (RAWINPUT*)lpb;

  if (raw->header.dwType == RIM_TYPEMOUSE) {
    this->mouseRawX = raw->data.mouse.lLastX;
    this->mouseRawY = raw->data.mouse.lLastY;
  }
}

void Input::checkControllers()
{
  for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
    this->controllers[i].connected =
        XInputGetState(i, &controllers[i].state) == ERROR_SUCCESS;
  }
}

void Input::readControllers()
{
  for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
    if (this->controllers[i].connected &&
        XInputGetState(i, &controllers[i].state) == ERROR_DEVICE_NOT_CONNECTED)
      this->controllers[i].connected = false;
  }
}

void Input::vibrateControllers(float frameTime)
{
  for (int i = 0; i < MAX_CONTROLLERS; i++) {
    if (this->controllers[i].connected) {
      this->controllers[i].vibrateTimeLeft -= frameTime;
      if (this->controllers[i].vibrateTimeLeft < 0)
        this->gamePadVibrateLeft(i, 0, 0);
      this->controllers[i].vibrateTimeRight -= frameTime;
      if (this->controllers[i].vibrateTimeRight < 0)
        this->gamePadVibrateRight(i, 0, 0);
    }
  }
}
