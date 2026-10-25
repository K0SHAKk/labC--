#include "Physics.h"
#include <algorithm>
#include <cmath>
#include <iterator>

namespace {
double dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
}
Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}
void Physics::setWorldBox(const Point& a, const Point& b) {
    topLeft = a;
    bottomRight = b;
}
void Physics::update(std::vector<Ball>& balls, std::vector<Dust>& dusts,
                     std::size_t ticks) const {
    for (std::size_t i = 0; i < ticks; ++i) {
        for (Dust& dust : dusts) { dust.update(timePerTick); }
        dusts.erase(std::remove_if(dusts.begin(), dusts.end(),
                                  [](const Dust& d) { return !d.ifAlive(); }),
                    dusts.end());
        move(balls);
        collideWithBox(balls, dusts);
        collideBalls(balls, dusts);
    }
}
void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        ball.setCenter(ball.getCenter() + ball.getVelocity().vector() * timePerTick);
    }
}
void Physics::spawnDust(std::vector<Dust>& dusts, const Point& contact,
                        const Color& color) const {
    const double pi = std::acos(-1.);
    for (int i = 0; i < 10; ++i) {
        dusts.emplace_back(Velocity(180. + 25. * i, 2. * pi * i / 10.),
                           contact, 8., color, 0.8);
    }
}
void Physics::collideWithBox(std::vector<Ball>& balls,
                             std::vector<Dust>& dusts) const {
    for (Ball& ball : balls) {
        if (!ball.ifCollidable()) { continue; }
        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        Point v = ball.getVelocity().vector();
        // Both axes must be checked for a corner collision.
        if ((p.x < topLeft.x + r && v.x < 0.) ||
            (p.x > bottomRight.x - r && v.x > 0.)) {
            v.x = -v.x;
            spawnDust(dusts, {p.x < topLeft.x + r ? topLeft.x : bottomRight.x, p.y},
                      ball.getColor());
        }
        if ((p.y < topLeft.y + r && v.y < 0.) ||
            (p.y > bottomRight.y - r && v.y > 0.)) {
            v.y = -v.y;
            spawnDust(dusts, {p.x, p.y < topLeft.y + r ? topLeft.y : bottomRight.y},
                      ball.getColor());
        }
        ball.setVelocity(v);
    }
}
void Physics::collideBalls(std::vector<Ball>& balls,
                           std::vector<Dust>& dusts) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {
        if (!a->ifCollidable()) { continue; }
        for (auto b = std::next(a); b != balls.end(); ++b) {
            if (!b->ifCollidable()) { continue; }
            const double d2 = distance2(a->getCenter(), b->getCenter());
            const double r = a->getRadius() + b->getRadius();
            if (d2 < r * r && processCollision(*a, *b, d2)) {
                const Point contact = a->getCenter() +
                    (b->getCenter() - a->getCenter()) * (a->getRadius() / r);
                spawnDust(dusts, contact, a->getColor());
            }
        }
    }
}
bool Physics::processCollision(Ball& a, Ball& b, double d2) const {
    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();
    Point normal;
    if (d2 > 1e-20) {
        normal = (b.getCenter() - a.getCenter()) / std::sqrt(d2);
    } else {
        const Point relative = aV - bV;
        const double speed2 = dot(relative, relative);
        normal = speed2 > 1e-20 ? relative / std::sqrt(speed2) : Point{1., 0.};
    }
    const double approach = dot(aV - bV, normal);
    if (approach <= 0.) { return false; }
    const double p = 2. * approach / (a.getMass() + b.getMass());
    // Preserve the course engine's rule: the supplied models were generated for it.
    a.setVelocity(aV - normal * p * a.getMass());
    b.setVelocity(bV + normal * p * b.getMass());
    return true;
}

