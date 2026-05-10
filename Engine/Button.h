#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "Text.h"

enum class ButtonState {
  Normal = 0,
  Hover = 1,
  Active = 2,
  Disabled = 3
};

class Scene;

class Button {
  public:
    Button(
      Scene& ParentScene,
      const std::string& Text,
      SDL_Rect Rect
    ) : ButtonText{Text, 20},
        Rect{Rect},
        ParentScene{ParentScene} {}

    virtual void HandleLeftClick() {}
    void HandleEvent(const SDL_Event& E);
    void Render(SDL_Surface* Surface);
    void Tick(float DeltaTime) {}

    ButtonState GetState() const {
      return State;
    }

    void SetState(ButtonState NewState) {
      State = NewState;
    }

  private:
    Scene& ParentScene;
    ButtonState State{ButtonState::Normal};
    Text ButtonText;
    SDL_Rect Rect;
};

