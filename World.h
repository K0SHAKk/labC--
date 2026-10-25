#pragma once
#include "Ball.hpp"
#include "Dust.hpp"
#include "Physics.h"
#include <string>
#include <vector>
class Painter;

class World {
  public:
    explicit World(const std::string& worldFilePath);
    void show(Painter& painter) const;
    void update(double time);
    const std::vector<Ball>& getBalls() const { return balls; }
    const std::vector<Dust>& getDusts() const { return dusts; }
  private:
    Point topLeft;
    Point bottomRight;
    Physics physics;
    std::vector<Ball> balls;
    std::vector<Dust> dusts;
    double restTime = 0.;
};

