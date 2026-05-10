#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Config.h"
#include "Engine/Window.h"
#include "Volfield/VolfieldScene.h"

#ifdef WITH_EDITOR
#include "Editor/Scene.h"
#include "Editor/Window.h"
#endif

int main(int argc, char** argv) {
   if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    CheckSDLError("SDL_Init");
    return 1;
  }

  if (!TTF_Init()) {
    CheckSDLError("TTF_Init");
    return 1;
  }

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

  Uint64 LastTick{SDL_GetPerformanceCounter()};
  SDL_Event E;
  while (true) {
    while (SDL_PollEvent(&E)) {
#ifdef WITH_EDITOR
      EditorScene.HandleEvent(E);
#endif
      GameScene.HandleEvent(E);
      if (
        E.type == SDL_EVENT_QUIT ||
        E.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
      ) {
        GameScene.Cleanup();
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

    GameScene.Tick(DeltaTime);
    GameWindow.Render();
    GameScene.Render(GameWindow.GetSurface(), DeltaTime);
    GameWindow.Update();
  }

  return 0;
}

