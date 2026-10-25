#include "World.h"
#include "black_box/Application.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 5) {
        std::cerr << "Usage: physics <model.txt> [--snapshot <seconds 0..10> <image.png>]\n";
        return 1;
    }
    try {
        World world(argv[1]);
        if (argc == 5) {
            if (std::string(argv[2]) != "--snapshot") {
                throw std::invalid_argument("Unknown option");
            }
            std::size_t end = 0;
            const double seconds = std::stod(argv[3], &end);
            if (end != std::string(argv[3]).size() || !std::isfinite(seconds) ||
                seconds < 0. || seconds > 10.) {
                throw std::invalid_argument("Snapshot time must be in [0, 10]");
            }
            Application::saveSnapshot(world, seconds, argv[4]);
            std::cout << "Saved " << argv[4] << "; t=" << seconds
                      << " s; balls=" << world.getBalls().size()
                      << "; dust=" << world.getDusts().size() << '\n';
        } else {
            Application app("Physics - Lab 5 - Golovkov A. O.");
            app.run(world);
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 2;
    }
    return 0;
}

