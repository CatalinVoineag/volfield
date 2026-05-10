#include <iostream>
#include "Engine/ECS/Commands.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/PhysicsComponent.h"
#include "Engine/ECS/TransformComponent.h"

void MovementCommand::Execute(Entity* Target) {
  if (!Target) return;
  PhysicsComponent* Physics{
    Target->GetComponent<PhysicsComponent>()
  };
  if (Physics) {
    Physics->SetVelocity({
      Velocity.x,
      Physics->GetVelocity().y
    });
  } else {
    std::cerr << "Error: MovementCommand "
      "requires a PhysicsComponent on entity\n";
  }
}

void JumpCommand::Execute(Entity* Target) {
  if (!Target) return;
  PhysicsComponent* Physics{
    Target->GetComponent<PhysicsComponent>()
  };
  if (Physics) {
    Physics->ApplyImpulse(Impulse);
  } else {
    std::cerr << "Error: JumpCommand "
      "requires a PhysicsComponent on entity\n";
  }
}
