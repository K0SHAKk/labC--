#include "Dust.hpp"
#include "Painter.h"
#include <algorithm>

Dust::Dust(const Velocity& velocity, const Point& center, double radius,
           const Color& color, double lifetime)
    : velocity_{velocity}, center_{center}, radius_{radius}, color_{color},
      lifetime_{lifetime} {}
void Dust::update(double time) {
    const double elapsed = std::min(time, std::max(0., lifetime_));
    center_ = center_ + velocity_.vector() * elapsed;
    lifetime_ = std::max(0., lifetime_ - time);
}
void Dust::draw(Painter& painter) const {
    if (ifAlive()) { painter.draw(center_, radius_, color_); }
}
bool Dust::ifAlive() const { return lifetime_ > 1e-12; }
Point Dust::getCenter() const { return center_; }
double Dust::getLifetime() const { return lifetime_; }

