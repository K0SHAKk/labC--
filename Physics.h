#pragma once
#include "Ball.hpp"
#include "Dust.hpp"
#include <cstddef>
#include <vector>

class Physics {
  public:
    explicit Physics(double timePerTick = 0.001);
    void setWorldBox(const Point& topLeft, const Point& bottomRight);
    void update(std::vector<Ball>& balls, std::vector<Dust>& dusts,
                std::size_t ticks) const;
  private:
    void collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const;
    void collideWithBox(std::vector<Ball>& balls, std::vector<Dust>& dusts) const;
    void move(std::vector<Ball>& balls) const;
    bool processCollision(Ball& a, Ball& b, double distance2) const;
    void spawnDust(std::vector<Dust>& dusts, const Point& contact,
                   const Color& color) const;
    Point topLeft;
    Point bottomRight;
    double timePerTick;
};

