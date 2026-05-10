#pragma once
#include "../Vec2.h"

class Entity;

class Command {
 public:
  virtual void Execute(Entity* Target) {}
  virtual ~Command() = default;
};

class MovementCommand : public Command {
public:
  MovementCommand(Vec2 Velocity)
  : Velocity{Velocity} {}
  void Execute(Entity* Target) override;
  Vec2 Velocity;
};

class JumpCommand : public Command {
 public:
  JumpCommand(Vec2 Impulse)
    : Impulse(Impulse) {}
  void Execute(Entity* Target) override;
  Vec2 Impulse;
};
