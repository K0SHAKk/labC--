#include "Color.h"
#include <cmath>
#include <istream>

Color::Color() = default;

Color::Color(double red, double green, double blue)
    : r{red}, g{green}, b{blue} {}

double Color::red() const {
    return r;
}

double Color::green() const {
    return g;
}

double Color::blue() const {
    return b;
}

std::istream& operator>>(std::istream& stream, Color& color) {
    double r, g, b;
    if (stream >> r >> g >> b) {
        auto valid = [](double v) { return std::isfinite(v) && v >= 0. && v <= 1.; };
        if (valid(r) && valid(g) && valid(b)) {
            color = Color(r, g, b);
        } else {
            stream.setstate(std::ios::failbit);
        }
    }
    return stream;
}
