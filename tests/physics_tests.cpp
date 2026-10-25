#include "World.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
void require(bool condition, const char* message) {
    if (!condition) { throw std::runtime_error(message); }
}
bool near(double a, double b, double eps = 1e-8) { return std::abs(a - b) < eps; }
Ball ball(Point p, Point v, double r = 1., bool collidable = true) {
    return Ball(Velocity(v), p, r, Color(1., 0.5, 0.), collidable);
}
Physics engine() {
    Physics p;
    p.setWorldBox({-100., -100.}, {100., 100.});
    return p;
}
void mass() {
    auto a = ball({}, {}, 2.);
    require(near(a.getMass(), 32. * std::acos(-1.) / 3.), "Sphere mass");
    require(near(a.getMass() / ball({}, {}).getMass(), 8.), "Mass scales as radius cubed");
    bool rejected = false;
    try { ball({}, {}, 0.); } catch (const std::invalid_argument&) { rejected = true; }
    require(rejected, "Zero radius must be rejected");
}
void streams() {
    std::istringstream input("1 2 -3 4 0.2 0.3 0.4");
    Point p;
    Velocity v;
    Color c;
    require(bool(input >> p >> v >> c), "Read composed objects");
    require(near(p.x, 1.) && near(p.y, 2.) && near(v.vector().x, -3.) &&
            near(c.blue(), 0.4), "Parsed components");
    std::istringstream bad("0.5 2 0");
    require(!(bad >> c), "Out-of-range color must fail");
    std::istringstream partial("9");
    require(!(partial >> p) && near(p.x, 1.), "Partial point must not overwrite the object");
}
void elastic() {
    auto physics = engine();
    std::vector<Ball> balls{ball({-0.9, 0.}, {3., 0.}), ball({0.9, 0.}, {-1., 0.})};
    std::vector<Dust> dust;
    const double ma = balls[0].getMass(), mb = balls[1].getMass();
    const double momentum = 3. * ma - mb, energy = 9. * ma + mb;
    physics.update(balls, dust, 1);
    const double va = balls[0].getVelocity().vector().x;
    const double vb = balls[1].getVelocity().vector().x;
    require(near(va, -1.) && near(vb, 3.), "Equal mass balls exchange normal velocities");
    require(near(ma * va + mb * vb, momentum) && near(ma * va * va + mb * vb * vb, energy),
            "Momentum and kinetic energy conservation");
    require(dust.size() == 10, "One particle burst per collision");
    physics.update(balls, dust, 1);
    require(dust.size() == 10, "Separating overlapped balls must not collide again");
}
void transparent() {
    auto physics = engine();
    std::vector<Ball> balls{ball({-1., 0.}, {3., 0.}, 1., false),
                            ball({0., 0.}, {-2., 0.}),
                            ball({99., 10.}, {10., 0.}, 1., false)};
    std::vector<Dust> dust;
    physics.update(balls, dust, 500);
    require(near(balls[0].getVelocity().vector().x, 3.) &&
            near(balls[1].getVelocity().vector().x, -2.), "Transparent ball crosses another ball");
    require(near(balls[2].getCenter().x, 104.) &&
            near(balls[2].getVelocity().vector().x, 10.), "Transparent ball crosses the wall");
    require(dust.empty(), "Transparent objects must not generate collision effects");
}
void wall() {
    auto physics = engine();
    std::vector<Ball> balls{ball({98.999, 98.999}, {10., 20.})};
    std::vector<Dust> dust;
    physics.update(balls, dust, 1);
    require(near(balls[0].getVelocity().vector().x, -10.) &&
            near(balls[0].getVelocity().vector().y, -20.), "Both axes reflect in a corner");
    require(dust.size() == 20, "Two contacts create two bursts");
}
void dust() {
    auto physics = engine();
    std::vector<Ball> balls{ball({98.999, 0.}, {10., 0.})};
    std::vector<Dust> dusts;
    physics.update(balls, dusts, 1);
    require(dusts.size() == 10, "Wall contact emits ten particles");
    const Point before = dusts.front().getCenter();
    balls.clear();
    physics.update(balls, dusts, 400);
    require(dusts.size() == 10 && dusts.front().getCenter().x > before.x,
            "Particles move during their lifetime");
    physics.update(balls, dusts, 400);
    require(dusts.empty(), "All particles are removed after 0.8 seconds");
}
void models() {
    for (const char* name : {"smile.txt", "elephant.txt"}) {
        World world(std::string(MODEL_DIR) + "/" + name);
        const auto initial = world.getBalls();
        const auto count = world.getBalls().size();
        world.update(10.);
        require(count > 0 && world.getBalls().size() == count, "Loaded model retains all balls");
        for (const Ball& b : world.getBalls()) {
            require(std::isfinite(b.getCenter().x) && std::isfinite(b.getCenter().y) &&
                    std::isfinite(b.getVelocity().vector().x), "Ten second simulation stays finite");
        }
        if (std::string(name) == "smile.txt") {
            std::size_t outline = 0;
            for (const Ball& b : world.getBalls()) {
                const Point c = b.getCenter();
                const double radius = std::sqrt(c.x * c.x + c.y * c.y);
                if (near(b.getRadius(), 50.) && std::abs(radius - 800.) < 20.) { ++outline; }
            }
            require(count == 50 && outline == 40, "Smile has its forty-ball circular outline at 10 seconds");
        } else {
            require(count == 15, "Elephant contains fifteen model objects");
            for (std::size_t i = 0; i < initial.size(); ++i) {
                if (!initial[i].ifCollidable()) {
                    const Point expected = initial[i].getCenter() + initial[i].getVelocity().vector() * 10.;
                    require(distance2(expected, world.getBalls()[i].getCenter()) < 1e-12,
                            "Transparent elephant components follow uninterrupted trajectories");
                }
            }
        }
        std::cout << name << ": " << count << " balls; t=10 s; dust=" << world.getDusts().size() << '\n';
    }
}
void parser() {
    const std::string path = "invalid-model-test.txt";
    for (const char* content : {"", "1 1 -1 -1\n", "-10 -10 10 10\n1 2 3\n",
                                "-10 -10 10 10\n0 0 1 0 1 0 0 -1 true\n",
                                "-10 -10 10 10\n0 0 1 0 1 0 0 1 maybe\n"}) {
        { std::ofstream out(path); out << content; }
        bool rejected = false;
        try { World world(path); } catch (const std::runtime_error&) { rejected = true; }
        require(rejected, "Malformed model must report an error");
    }
    std::remove(path.c_str());
    bool rejected = false;
    try { World world(path); } catch (const std::runtime_error&) { rejected = true; }
    require(rejected, "Missing model must report an error");
}
void timing() {
    const std::string path = std::string(MODEL_DIR) + "/smile.txt";
    World one(path), many(path);
    one.update(1.);
    for (int i = 0; i < 2500; ++i) { many.update(0.0004); }
    require(one.getDusts().size() == many.getDusts().size(), "Particle lifetime is independent of frames");
    for (std::size_t i = 0; i < one.getBalls().size(); ++i) {
        require(distance2(one.getBalls()[i].getCenter(), many.getBalls()[i].getCenter()) < 1e-12,
                "Sub-tick time remainder preserves trajectories");
    }
    for (double invalid : {-1., std::numeric_limits<double>::infinity()}) {
        bool rejected = false;
        try { one.update(invalid); } catch (const std::invalid_argument&) { rejected = true; }
        require(rejected, "Invalid time must be rejected");
    }
}
void coincident() {
    auto physics = engine();
    std::vector<Ball> balls{ball({-0.001, 0.}, {1., 0.}), ball({0.001, 0.}, {-1., 0.})};
    std::vector<Dust> dust;
    physics.update(balls, dust, 1);
    for (const auto& b : balls) {
        require(std::isfinite(b.getVelocity().vector().x), "Coincident centers never produce NaN");
    }
    require(near(balls[0].getVelocity().vector().x, -1.), "Coincident collision resolves velocities");
}
}
int main(int argc, char** argv) {
    const std::map<std::string, std::function<void()>> cases{
        {"mass", mass}, {"streams", streams}, {"elastic", elastic},
        {"transparent", transparent}, {"wall", wall}, {"dust", dust},
        {"models", models}, {"parser", parser}, {"timing", timing}, {"coincident", coincident}};
    try {
        if (argc != 2 || !cases.count(argv[1])) { throw std::runtime_error("Unknown test"); }
        cases.at(argv[1])();
        std::cout << "PASS " << argv[1] << '\n';
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
