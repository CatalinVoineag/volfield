#pragma once
#include <iostream>
#include <cmath>

struct IntVec2 {
  int x;
  int y;

  float GetLength() const {
    return std::sqrt(x * x + y * y);
  }
};
