#pragma once
#include <memory>
#include <vector>
#include <SDL3/SDL.h>
#include <iostream>

#include "Component.h"
#include "Commands.h"

class Scene;
using ComponentPtr = std::unique_ptr<Component>;
using ComponentPtrs = std::vector<ComponentPtr>;

class Entity {
public:
  Entity(Scene& Scene) : OwningScene{Scene} {}
  Scene& GetScene() const { return OwningScene; }

  virtual void HandleEvent(const SDL_Event& E) {
    for (ComponentPtr& C : Components) {
      C->HandleEvent(E);
    }
  }

  virtual void Tick(float DeltaTime) {
    for (ComponentPtr& C : Components) {
      C->Tick(DeltaTime);
    }
  }

  virtual void Render(SDL_Surface* Surface, float DeltaTime) {
    for (ComponentPtr& C : Components) {
      C->Render(Surface, DeltaTime);
    }
#ifdef DRAW_DEBUG_HELPERS
    for (ComponentPtr& C : Components) {
      C->DrawDebugHelpers(Surface);
    }
#endif
  }

  virtual void HandleCollision(Entity& Other) {}

  virtual ~Entity() = default;

  virtual void HandleCommand(
    std::unique_ptr<Command> Cmd
  ) {
    Cmd->Execute(this);
  }

  template <typename CType, typename... CArgs>
  requires std::derived_from<CType, Component>
  CType* AddComponent(CArgs&&... ConstructorArgs) {
    ComponentPtr& NewComponent{
      Components.emplace_back(
        std::make_unique<CType>(
          this,
          std::forward<CArgs>(ConstructorArgs)...
        )
      )
    };
    NewComponent->Initialize();
    return static_cast<CType*>(NewComponent.get());
  }

  template <typename CType>
  requires std::derived_from<CType, Component>
  CType* GetComponent() {
    for (const ComponentPtr& C : Components) {
      if (auto Ptr{dynamic_cast<CType*>(
        C.get()
      )}) {
        return Ptr;
      }
    }
    return nullptr;
  }

  void RemoveComponent(Component* PtrToRemove) {
    for (size_t i{0}; i < Components.size(); ++i) {
      if (Components[i].get() == PtrToRemove) {
        Components.erase(Components.begin() + i);
        return;
      }
    }

    std::cout << "Warning: Attempted to remove "
      "a component not found on this entity.\n";
  }

protected:
  ComponentPtrs Components;
  Scene& OwningScene;
};
