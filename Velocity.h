#pragma once
#include "Point.h"
#include <cmath>

class Velocity {
  public:
    inline Velocity() = default;

    // Construct a velocity from its magnitude and angle in radians.
    inline Velocity(double abs, double angle) {
        const double x = std::cos(angle);
        const double y = std::sin(angle);
        vec = Point{x, y} * abs;
    }

    inline Velocity(const Point& vector) {
        setVector(vector);
    }

    inline void setVector(const Point& vector) {
        vec = vector;
    }

    inline Point vector() const {
        return vec;
    }

  private:
    Point vec;
};

inline std::istream& operator>>(std::istream& stream, Velocity& velocity) {
    Point vector;
    if (stream >> vector) { velocity.setVector(vector); }
    return stream;
}
