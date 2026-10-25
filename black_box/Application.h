#pragma once
#include "View.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <chrono>
#include <memory>

class World;

class Application {
  public:
    Application(const char* name);
    void run(World& world);
    static void saveSnapshot(World& world, double seconds, const std::string& path);

  private:
    void processEvents();
    void updateWorld(World& world);
    void drawWorld(const World& world);

  private:
    sf::RenderWindow window;
    View view;
    std::chrono::steady_clock::time_point time;
    double totalTime{};
};
