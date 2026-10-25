#include "Application.h"
#include "../World.h"
#include "../Painter.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <map>

Application::Application(const char* name)
    : window{{1280u, 960u},
             name,
             sf::Style::Titlebar | sf::Style::Close,
             sf::ContextSettings(0, 0, 4)},
      view({0., 0.}, 1280, 960, 0.2) {
    window.setFramerateLimit(60);
}

void Application::run(World& world) {
    time = std::chrono::steady_clock::now();
    totalTime = 0.;

    while (window.isOpen()) {
        processEvents();
        if (!window.isOpen()) { break; }
        updateWorld(world);
        drawWorld(world);
    }
}

void Application::processEvents() {
    for (sf::Event event{}; window.pollEvent(event); /**/) {
        if (event.type == sf::Event::EventType::Closed) {
            window.close();
        } else if (event.type == sf::Event::EventType::MouseWheelScrolled) {
            view.onZoom(event.mouseWheelScroll.delta);
        }
    }
}

void Application::updateWorld(World& world) {
    const auto currentTime = std::chrono::steady_clock::now();
    const double delta =
        std::chrono::duration_cast<std::chrono::duration<double>>(currentTime -
                                                                  time)
            .count();
    time = currentTime;

    if (totalTime < 10.) {
        const double step = std::min(delta, 10. - totalTime);
        world.update(step);
        totalTime += step;
    }
}

void Application::drawWorld(const World& world) {
    Painter painter(window, view);
    world.show(painter);
    window.display();
}

void Application::saveSnapshot(World& world, double seconds, const std::string& path) {
    world.update(seconds);
    sf::RenderTexture image;
    if (!image.create(1280, 960)) {
        throw std::runtime_error("Cannot create snapshot render texture");
    }
    View snapshotView({0., 0.}, 1280, 960, 0.2);
    Painter painter(image, snapshotView);
    world.show(painter);
    image.display();
    if (!image.getTexture().copyToImage().saveToFile(path)) {
        throw std::runtime_error("Cannot save snapshot: " + path);
    }
}
