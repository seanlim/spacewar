#pragma once
#define NOMINMAX

#include "array.h"
#include "common.h"
#include "component.h"
#include "map.h"
#include "system.h"
#include <algorithm>
#include <string>

// pair (Component id, Component index)
typedef std::pair<uint32, uint32> ComponentReference;
// pair (ECSEntity id, components)
typedef std::pair<uint32, Array<ComponentReference>> ECSEntity;

// TODO: Integrate ECS Listener w/ ECS class
// class ECSListener
// {
// public:
//   virtual void onMakeEntity(EntityHook hook) {}
//   virtual void onRemoveEntity(EntityHook hook) {}
//   virtual void onAddComponent(EntityHook hook, uint32 id) {}
//   virtual void onRemoveComponent(EntityHook hook, uint32 id) {}

//   const Array<uint32>& getComponentIDs() { return componentIDs; }
//   inline bool shouldNotifyOnAllComponentOperations()
//   {
//     return notifyOnAllComponentOperations;
//   }
//   inline bool shouldNotifyOnAllEntityOperations()
//   {
//     return notifyOnAllEntityOperations;
//   }

// protected:
//   void setNotificationSettings(bool shouldNotifyOnAllComponentOperations,
//                                bool shouldNotifyOnAllEntityOperations)
//   {
//     notifyOnAllComponentOperations = shouldNotifyOnAllComponentOperations;
//     notifyOnAllEntityOperations = shouldNotifyOnAllEntityOperations;
//   }

//   void addComponentID(uint32 id) { componentIDs.push_back(id); }

// private:
//   Array<uint32> componentIDs;
//   bool notifyOnAllComponentOperations = false;
//   bool notifyOnAllEntityOperations = false;
// };

class ECS
{
public:
  ECS() {}
  ~ECS();

  // Entities
  EntityHook makeEntity(BaseComponent** entityComponents,
                        const uint32* componentIDs, size_t numComponents);

  template <class A> EntityHook makeEntity(A& c1)
  {
    BaseComponent* components[] = {&c1};
    uint32 componentIDs[] = {A::id};
    return makeEntity(components, componentIDs, 1);
  }
  template <class A, class B> EntityHook makeEntity(A& c1, B& c2)
  {
    BaseComponent* components[] = {&c1, &c2};
    uint32 componentIDs[] = {A::id, B::id};
    return makeEntity(components, componentIDs, 2);
  }
  template <class A, class B, class C>
  EntityHook makeEntity(A& c1, B& c2, C& c3)
  {
    BaseComponent* components[] = {&c1, &c2, &c3};
    uint32 componentIDs[] = {A::id, B::id, C::id};
    return makeEntity(components, componentIDs, 3);
  }
  template <class A, class B, class C, class D>
  EntityHook makeEntity(A& c1, B& c2, C& c3, D& c4)
  {
    BaseComponent* components[] = {&c1, &c2, &c3, &c4};
    uint32 componentIDs[] = {A::id, B::id, C::id, D::id};
    return makeEntity(components, componentIDs, 4);
  }

  void removeEntity(EntityHook hook);

  // Components
  template <class Component>
  void addComponent(EntityHook hook, Component* component)
  {
    addComponentInternal(hook, componentsFrom(hook), Component::id, component);

    // TODO: Notify listeners
  }

  template <class Component> void removeComponent(EntityHook hook)
  {
    // TODO: Notify listeners
    return removeComponentInternal(hook, Component::id);
  }

  template <class Component> Component* getComponent(EntityHook hook)
  {
    return (Component*)getComponentInternal(componentsFrom(hook),
                                            Component::id);
  }

  BaseComponent* getComponentByType(EntityHook hook, uint32 componentID)
  {
    return getComponentInternal(componentsFrom(hook), componentID);
  }

  // Systems
  void updateSystems(SystemList& systems, float delta);

private:
  // (Component id, concrete components)
  Map<uint32, Array<uint32>> components;
  Array<ECSEntity*> entities;

  // Inline entity hook helpers
  inline ECSEntity* entityFrom(EntityHook hook) { return (ECSEntity*)hook; }
  inline uint32 entityIDFrom(EntityHook hook)
  {
    return entityFrom(hook)->first;
  }
  inline Array<ComponentReference>& componentsFrom(EntityHook hook)
  {
    return entityFrom(hook)->second;
  }

  void deleteComponent(uint32 componentID, uint32 index);
  bool removeComponentInternal(EntityHook hook, uint32 index);
  void addComponentInternal(EntityHook hook,
                            Array<ComponentReference>& entityComponents,
                            uint32 componentID, BaseComponent* component);
  BaseComponent*
  getComponentInternal(Array<ComponentReference>& entityComponents,
                       uint32 componentID);

  void updateComplexSystem(uint32 index, SystemList& systems, float delta,
                           const Array<uint32>& componentTypes,
                           Array<BaseComponent*>& componentParam,
                           Array<Array<uint32>*>& componentArrays);
  uint32 getLeastCommonComponentID(const Array<uint32>& componentTypes,
                                   const Array<uint32>& componentFlags);
};