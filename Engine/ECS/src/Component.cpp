#include <iostream>
#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/TransformComponent.h"
#include "Engine/Scene.h"

Scene& Component::GetScene() const {
  return GetOwner()->GetScene();
}

AssetManager& Component::GetAssetManager() const {
  return GetScene().GetAssetManager();
}

SoundAssetManager& Component::GetSoundAssetManager() const {
  return GetScene().GetSoundAssetManager();
}

Vec2 Component::GetOwnerPosition() const {
  TransformComponent* Transform{
    GetOwner()->GetComponent<TransformComponent>()
  };
  if (!Transform) {
    std::cerr << "Error: attempted to get position"
      " of an entity with no transform component\n";
    return {0, 0};
  }
  return Transform->GetPosition();
}

void Component::SetOwnerPosition(const Vec2& Pos) const {
  TransformComponent* Transform{
    GetOwner()->GetComponent<TransformComponent>()
  };
  if (!Transform) {
    std::cerr << "Error: attempted to set position"
      " of an entity with no transform component\n";
  } else {
    Transform->SetPosition(Pos);
  }
}

float Component::GetOwnerScale() const {
  TransformComponent* Transform{
    GetOwner()->GetComponent<TransformComponent>()
  };
  if (!Transform) {
    std::cerr << "Error: attempted to get scale"
      " of an entity with no transform component\n";
    return 1.0;
  }
  return Transform->GetScale();
}
