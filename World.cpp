#include "World.h"
#include "Painter.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace { constexpr double timePerTick = 0.001; }
World::World(const std::string& worldFilePath) {
    std::ifstream stream(worldFilePath);
    if (!stream) { throw std::runtime_error("Cannot open model: " + worldFilePath); }
    std::string line;
    if (!std::getline(stream, line)) { throw std::runtime_error("Empty model"); }
    std::istringstream header(line);
    if (!(header >> topLeft >> bottomRight) ||
        !(header >> std::ws).eof() || topLeft.x >= bottomRight.x ||
        topLeft.y >= bottomRight.y) {
        throw std::runtime_error("Invalid world bounds on line 1");
    }
    physics.setWorldBox(topLeft, bottomRight);
    std::size_t lineNumber = 1;
    while (std::getline(stream, line)) {
        ++lineNumber;
        std::istringstream row(line);
        row >> std::ws;
        if (row.eof()) { continue; }
        Point center;
        Velocity velocity;
        Color color;
        double radius;
        bool isCollidable;
        if (!(row >> center >> velocity >> color >> radius >> std::boolalpha >> isCollidable) ||
            !(row >> std::ws).eof() || !std::isfinite(radius) || radius <= 0. ||
            (isCollidable && (2. * radius >= bottomRight.x - topLeft.x ||
                              2. * radius >= bottomRight.y - topLeft.y))) {
            throw std::runtime_error("Invalid ball on line " + std::to_string(lineNumber));
        }
        balls.emplace_back(velocity, center, radius, color, isCollidable);
    }
    if (balls.empty()) { throw std::runtime_error("Model contains no balls"); }
}
void World::show(Painter& painter) const {
    painter.draw(topLeft, bottomRight, Color(1., 1., 1.));
    for (const Ball& ball : balls) { ball.draw(painter); }
    for (const Dust& dust : dusts) { dust.draw(painter); }
}
void World::update(double time) {
    if (!std::isfinite(time) || time < 0.) {
        throw std::invalid_argument("Time must be finite and nonnegative");
    }
    time += restTime;
    const auto ticks = static_cast<std::size_t>(std::floor((time + 1e-12) / timePerTick));
    restTime = std::max(0., time - static_cast<double>(ticks) * timePerTick);
    physics.update(balls, dusts, ticks);
}

