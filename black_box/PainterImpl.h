#pragma once
#include "../Color.h"
#include "View.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
    class RenderTarget;
}

class PainterImpl {
  public:
    PainterImpl(sf::RenderTarget& window, const View& view);
    ~PainterImpl();
    void draw(const Point& center, double radius, const Color& color);
    void draw(const Point& topLeft, const Point& bottomRight,
              const Color& color);

  private:
    sf::Color toSFMLColor(const Color& color) const;
    sf::Vector2f toVector(const Point& point) const;

  private:
    sf::RenderTarget& window;

    const View& view;
};
