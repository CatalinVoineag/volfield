#pragma once
#include "../Engine/Button.h"

constexpr static int OFFSET{50};

class PlayButton : public Button {
  public:
    constexpr static int WIDTH{100};
    constexpr static int HEIGHT{50};
    PlayButton(Scene& ParentScene, int X, int Y)
    : Button {
        ParentScene,
        "PLAY",
        {X, Y, WIDTH, HEIGHT}
      }
    {}

    void HandleLeftClick() override {
      using namespace UserEvents;
      SDL_Event E;
      E.type = GAME_SCENE;
      SDL_PushEvent(&E);
    }
};

class SettingsButton : public Button {
  public:
    constexpr static int WIDTH{100};
    constexpr static int HEIGHT{50};
    SettingsButton(Scene& ParentScene)
    : Button {
        ParentScene,
        "SETTINGS",
        {
          static_cast<int>(ParentScene.GetWidth()) - WIDTH,
          static_cast<int>(ParentScene.GetHeight()) - HEIGHT + PlayButton::HEIGHT + OFFSET,
          WIDTH, HEIGHT
        }
      }
    {}

    void HandleLeftClick() override {
      // using namespace UserEvents;
      // SDL_Event E;
      // E.type = SAVE_AND_PLAY_LEVEL;
      // SDL_PushEvent(&E);
    }
};
