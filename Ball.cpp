#include "Ball.hpp"
#include "Painter.h"
#include <cmath>
#include <stdexcept>

Ball::Ball(const Velocity& velocity, const Point& center, double radius,
           const Color& color, bool isCollidable)
    : velocity_{velocity}, center_{center}, radius_{radius}, color_{color},
      isCollidable_{isCollidable} {
    if (!std::isfinite(radius) || radius <= 0.) {
        throw std::invalid_argument("Ball radius must be finite and positive");
    }
}
void Ball::setVelocity(const Velocity& velocity) { velocity_ = velocity; }
Velocity Ball::getVelocity() const { return velocity_; }
void Ball::draw(Painter& painter) const { painter.draw(center_, radius_, color_); }
void Ball::setCenter(const Point& center) { center_ = center; }
Point Ball::getCenter() const { return center_; }
double Ball::getRadius() const { return radius_; }
double Ball::getMass() const {
    const double pi = std::acos(-1.);
    return pi * std::pow(radius_, 3) * 4. / 3.;
}
Color Ball::getColor() const { return color_; }
bool Ball::ifCollidable() const { return isCollidable_; }

