#include "Engine/Scene.h"
#include "Engine/ECS/CollisionComponent.h"
#include <iostream>

Scene::Scene(Window& ParentWindow)
  : ParentWindow{ParentWindow} {
#ifdef DRAW_DEBUG_HELPERS
  Trajectories = SDL_CreateSurface(
    Config::Engine::WINDOW_WIDTH,
    Config::Engine::WINDOW_HEIGHT,
    SDL_PIXELFORMAT_RGBA32
  );
#endif
}

void Scene::HandleEvent(const SDL_Event& E) {
  for (EntityPtr& Entity : Entities) {
    Entity->HandleEvent(E);
  }
}

void Scene::Tick(float DeltaTime) {
  for (EntityPtr& Entity : Entities) {
    Entity->Tick(DeltaTime);
  }
  CheckCollisions();
}

void Scene::Render(SDL_Surface* Surface, float DeltaTime) {
  for (EntityPtr& Entity : Entities) {
    Entity->Render(Surface, DeltaTime);
  }

#ifdef DRAW_DEBUG_HELPERS
  if (Trajectories) {
    SDL_BlitSurface(
      Trajectories, nullptr, Surface, nullptr
    );
  }
#endif
}

AssetManager& Scene::GetAssetManager() {
  return Assets;
}

SoundAssetManager& Scene::GetSoundAssetManager() {
  return SoundAssets;
}

Window& Scene::GetWindow() const {
  return ParentWindow;
}

float Scene::GetWidth() const {
  return static_cast<float>(
    Config::Engine::WINDOW_WIDTH);
}

float Scene::GetHeight() const {
  return static_cast<float>(
    Config::Engine::WINDOW_HEIGHT);
}

void Scene::CheckCollisions() {
  for (size_t i{0}; i < Entities.size(); ++i) {
    CollisionComponent* ColA{
      Entities[i]
      ->GetComponent<CollisionComponent>()};
    if (!(ColA && ColA->GetIsEnabled())) { continue; }

    for (size_t j{i + 1}; j < Entities.size();
         ++j) {
      CollisionComponent* ColB{
        Entities[j]
        ->GetComponent<CollisionComponent>()};

      if (!(ColB && ColB->GetIsEnabled())) { continue; }

      if (ColA->IsCollidingWith(*ColB)) {
        Entities[i]->HandleCollision(
          *Entities[j]);
        Entities[j]->HandleCollision(
          *Entities[i]);
      }
    }
  }
}
