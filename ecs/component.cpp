#include "component.h"

Array<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>>*
    BaseComponent::componentTypes;

uint32 BaseComponent::registerComponentType(ComponentCreateFunction createFn,
                                            ComponentFreeFunction freeFn,
                                            size_t size)
{
  if (componentTypes == nullptr)
    componentTypes = new Array<
        std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>>();

  uint32 componentID = componentTypes->size();
  componentTypes->push_back(
      std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>(
          createFn, freeFn, size));
  return componentID;
}