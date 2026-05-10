#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include "ECS/Entity.h"
#include "AssetManager.h"
#include "SoundAssetManager.h"
#include "Window.h"
#include "../Config.h"

using EntityPtr = std::unique_ptr<Entity>;
using EntityPtrs = std::vector<EntityPtr>;

enum class GameState { InProgress, Won, Lost };

class Scene {
public:
  Scene(Window& ParentWindow);

  virtual void HandleEvent(const SDL_Event& E);
  virtual void Tick(float DeltaTime);
  virtual void Render(SDL_Surface* Surface, float DeltaTime);

  GameState GetState() const { return State; }
  void SetState(GameState NewState) {
    State = NewState;
  }

  AssetManager& GetAssetManager();
  SoundAssetManager& GetSoundAssetManager();
  Window& GetWindow() const;

  float GetWidth() const;
  float GetHeight() const;

  void Cleanup() {
    Entities.clear();
  }

  void RemoveEntity(int Index) {
    Entities.erase(Entities.begin() + Index);
  }

#ifdef DRAW_DEBUG_HELPERS
  SDL_Surface* Trajectories{nullptr};
#endif

  static inline int PIXELS_PER_METER{50};
  static inline Vec2 GRAVITY{
    0, 9.8f * PIXELS_PER_METER
  };

protected:
  SoundAssetManager SoundAssets;
  Window& ParentWindow;
  AssetManager Assets;
  EntityPtrs Entities;

private:
  void CheckCollisions();
  GameState State{GameState::InProgress};
};
