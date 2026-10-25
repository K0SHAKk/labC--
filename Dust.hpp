#pragma once
#include "Color.h"
#include "Point.h"
#include "Velocity.h"
class Painter;

// Visual particles never enter the collision solver.
class Dust {
  public:
    Dust(const Velocity& velocity, const Point& center, double radius,
         const Color& color, double lifetime = 0.8);
    void update(double time);
    void draw(Painter& painter) const;
    bool ifAlive() const;
    Point getCenter() const;
    double getLifetime() const;
  private:
    Velocity velocity_;
    Point center_;
    double radius_;
    Color color_;
    double lifetime_;
};

