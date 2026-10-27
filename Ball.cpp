#include "Ball.hpp"
#include <cmath>
#include <stdexcept>

Ball::Ball(const Velocity velocity, const Point center, double radius,
           const Color color, bool isCollidable)
    : velocity_general{velocity}, center_general{center}, radius_general{radius},
      color_general{color}, isCollidable_general{isCollidable} {
    if (!std::isfinite(radius_general) || radius_general <= 0.) {
        throw std::invalid_argument("Ball radius must be finite and positive");
    }
}
void Ball::setVelocity(const Velocity& velocity) { velocity_general = velocity; }
Velocity Ball::getVelocity() const { return velocity_general; }
void Ball::draw(Painter& painter) const {
    painter.draw(center_general, radius_general, color_general);
}
void Ball::setCenter(const Point& center) { center_general = center; }
Point Ball::getCenter() const { return center_general; }
double Ball::getRadius() const { return radius_general; }
double Ball::getMass() const {
    const double pi = std::acos(-1.);
    return pi * std::pow(radius_general, 3) * 4. / 3.;
}
Color Ball::getColor() const { return color_general; }
bool Ball::ifCollidable() const { return isCollidable_general; }

