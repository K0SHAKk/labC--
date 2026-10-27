#include "Dust.hpp"
#include "Painter.h"
#include <cmath>
#include <stdexcept>

Dust::Dust(const Velocity velocity, const Point center, double radius,
           const Color color, bool isCollidable, double lifetime)
    : velocity_general{velocity}, center_general{center}, radius_general{radius},
      color_general{color}, isCollidable_general{isCollidable},
      lifetime_general{lifetime} {
    if (!std::isfinite(radius_general) || radius_general <= 0.) {
        throw std::invalid_argument("Dust radius must be finite and positive");
    }
    if (!std::isfinite(lifetime_general) || lifetime_general < 0.) {
        throw std::invalid_argument("Dust lifetime must be finite and nonnegative");
    }
    isAlive_general = lifetime_general > 0.;
}

void Dust::setVelocity(const Velocity& velocity) { velocity_general = velocity; }
Velocity Dust::getVelocity() const { return velocity_general; }

void Dust::draw(Painter& painter) const {
    if (ifAlive()) { painter.draw(center_general, radius_general, color_general); }
}

void Dust::setCenter(const Point& center) { center_general = center; }
Point Dust::getCenter() const { return center_general; }
double Dust::getRadius() const { return radius_general; }
double Dust::getMass() const {
    const double pi = std::acos(-1.);
    return pi * std::pow(radius_general, 3) * 4. / 3.;
}
bool Dust::ifAlive() const { return isAlive_general; }

void Dust::dying(double time) {
    if (!std::isfinite(time) || time < 0.) {
        throw std::invalid_argument("Dust update time must be finite and nonnegative");
    }
    lifetime_general = std::max(0., lifetime_general - time);
    isAlive_general = lifetime_general > 0.;
}

