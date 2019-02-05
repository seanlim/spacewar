#pragma once

#include "array.h"
#include "common.h"
#include <tuple>

#define ENTITY_NULL_HANDLE nullptr;

typedef void* EntityHook;

struct BaseComponent; // Forward definition of BaseComponent

// Create and free function pointers
typedef uint32 (*ComponentCreateFunction)(Array<uint32>& memory,
                                          EntityHook entity,
                                          BaseComponent* comp);
typedef void (*ComponentFreeFunction)(BaseComponent* comp);

struct BaseComponent {
public:
  // Add component type
  static uint32 registerComponentType(ComponentCreateFunction createFn,
                                      ComponentFreeFunction freeFn,
                                      size_t size);

  EntityHook entity = ENTITY_NULL_HANDLE; // Blind reference to attached entity

  // Sugar for getters
  inline static ComponentCreateFunction getTypeCreateFunction(uint32 id)
  {
    return std::get<0>((*componentTypes)[id]);
  }

  inline static ComponentFreeFunction getTypeFreeFunction(uint32 id)
  {
    return std::get<1>((*componentTypes)[id]);
  }
  inline static size_t getTypeSize(uint32 id)
  {
    return std::get<2>((*componentTypes)[id]);
  }

private:
  static Array<std::tuple<ComponentCreateFunction, ComponentFreeFunction,
                          size_t>>* componentTypes;
};

template <typename T> struct Component : BaseComponent {
  static const ComponentCreateFunction createFunction;
  static const ComponentFreeFunction freeFunction;
  static const uint32 id;
  static const size_t size;
};

template <typename Component>
uint32 CreateComponent(Array<uint32>& memory, EntityHook entity,
                       BaseComponent* comp)
{
  uint32 index = memory.size();
  memory.resize(index + Component::size);
  Component* component = new (&memory[index]) Component(*(Component*)comp);
  component->entity = entity;
  return index;
}

template <typename Component> void FreeComponent(BaseComponent* comp)
{
  Component* component = (Component*)comp;
  component->~Component();
}

template <typename T>
const ComponentCreateFunction Component<T>::createFunction(CreateComponent<T>);
template <typename T>
const ComponentFreeFunction Component<T>::freeFunction(FreeComponent<T>);

template <typename T> const size_t Component<T>::size(sizeof(T));

template <typename T>
const uint32 Component<T>::id(BaseComponent::registerComponentType(
    CreateComponent<T>, FreeComponent<T>, sizeof(T)));
