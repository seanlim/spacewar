#pragma once

#include "array.h"
#include "component.h"

class System
{
public:
  enum {
    FLAG_OPTIONAL = 1,
  };
  System() {}
  const Array<uint32>& getComponentTypes() { return componentTypes; }
  const Array<uint32>& getComponentFlags() { return componentFlags; }
  virtual void updateComponents(float delta, BaseComponent** components) {}

  bool isValid()
  {
    for (int i = 0; i < componentFlags.size(); i++) {
      if ((componentFlags[i] & System::FLAG_OPTIONAL) == 0) return true;
    }
    return false;
  }

protected:
  void addComponentType(uint32 componentID, uint32 flag = 0)
  {
    componentTypes.push_back(componentID);
    componentFlags.push_back(flag);
  }

private:
  Array<uint32> componentTypes;
  Array<uint32> componentFlags;
};

class SystemList
{
public:
  inline bool addSystem(System& system)
  {
    if (!system.isValid()) return false;

    systems.push_back(&system);
    return true;
  }

  inline size_t size() { return systems.size(); }

  inline System* operator[](uint32 index) { return systems[index]; }

  bool removeSystem(System& system)
  {
    for (int i = 0; i < systems.size(); i++) {
      if (&system == systems[i]) {
        systems.erase(systems.begin() + i);
        return true;
      }
    }

    return false;
  }

private:
  Array<System*> systems;
};