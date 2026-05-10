#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include "Config.h"
#include "Engine/Window.h"
#include "Volfield/VolfieldScene.h"
#include "Volfield/MenuScene.h"

#ifdef WITH_EDITOR
#include "Editor/Scene.h"
#include "Editor/Window.h"
#endif

enum class SceneEnum {GameScene, MenuScene};

int main(int argc, char** argv) {
   if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    CheckSDLError("SDL_Init");
    return 1;
  }

  if (!TTF_Init()) {
    CheckSDLError("TTF_Init");
    return 1;
  }

  SceneEnum SceneState = SceneEnum::MenuScene;

#ifdef WITH_EDITOR
  Editor::Window EditorWindow;
  Editor::Scene EditorScene{EditorWindow};
  // Enable mouse button events for clicks that
  // change input focus
  SDL_SetHint(
    SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1"
  );
#endif
  Window GameWindow;
  VolfieldScene GameScene{GameWindow};
  MenuScene Menu{GameWindow};

  Uint64 LastTick{SDL_GetPerformanceCounter()};
  SDL_Event E;
  while (true) {
    while (SDL_PollEvent(&E)) {
#ifdef WITH_EDITOR
      EditorScene.HandleEvent(E);
#endif

      if (E.type == UserEvents::GAME_SCENE) {
        SceneState = SceneEnum::GameScene;
      } else if (E.type == UserEvents::MENU_SCENE) {
        SceneState = SceneEnum::MenuScene;
      }

      if (SceneState == SceneEnum::GameScene) {
        GameScene.HandleEvent(E);
      } else if (SceneState == SceneEnum::MenuScene) {
        Menu.HandleEvent(E);
      }

      if (
        E.type == SDL_EVENT_QUIT ||
        E.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
      ) {
        GameScene.Cleanup();
        Menu.Cleanup();
        TTF_Quit();
        SDL_Quit();
        return 0;
      }
    }

    Uint64 CurrentTick{SDL_GetPerformanceCounter()};
    float DeltaTime{
      static_cast<float>(CurrentTick - LastTick) /
        static_cast<float>(SDL_GetPerformanceFrequency())
    };
    LastTick = CurrentTick;

#ifdef WITH_EDITOR
    EditorScene.Tick(DeltaTime);
    EditorWindow.Render();
    EditorScene.Render(EditorWindow.GetSurface());
    EditorWindow.Update();
#endif

    if (SceneState == SceneEnum::GameScene) {
      GameScene.Tick(DeltaTime);
    } else if (SceneState == SceneEnum::MenuScene) {
      Menu.Tick(DeltaTime);
    }

    GameWindow.Render();

    if (SceneState == SceneEnum::GameScene) {
      GameScene.Render(GameWindow.GetSurface(), DeltaTime);
    } else if (SceneState == SceneEnum::MenuScene) {
      Menu.Render(GameWindow.GetSurface(), DeltaTime);
    }

    GameWindow.Update();
  }

  return 0;
}

