#pragma once
#include <SDL3/SDL.h>

class Entity;
class Scene;
class AssetManager;
class SoundAssetManager;
struct Vec2;

class Component {
 public:
  Component(Entity* Owner) : Owner(Owner) {}
  virtual void Initialize() {}
  virtual void HandleEvent(const SDL_Event& E) {}
  virtual void Tick(float DeltaTime) {}
  virtual void Render(SDL_Surface* Surface, float DeltaTime) {}
  virtual void DrawDebugHelpers(SDL_Surface* Surface) {}
  virtual ~Component() = default;

  void SetIsEnabled(bool Enabled) {
    isEnabled = Enabled;
  }
  bool GetIsEnabled() const {
    return isEnabled;
  }

  Entity* GetOwner() const { return Owner; }
  Scene& GetScene() const;
  AssetManager& GetAssetManager() const;
  SoundAssetManager& GetSoundAssetManager() const;
  Vec2 GetOwnerPosition() const;
  void SetOwnerPosition(const Vec2& Pos) const;

  float GetOwnerScale() const;

private:
  Entity* Owner{nullptr};
  bool isEnabled{true};
};
