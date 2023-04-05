#include "Utilities.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>

void centerOrigin(sf::Shape& shape) {
	sf::FloatRect bounds = shape.getLocalBounds();
	shape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(sf::Text& text) {
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}


float length(const sf::Vector2f& v) {
	return sqrt(v.x * v.x + v.y * v.y);
}


float dist(const sf::Vector2f& a, const sf::Vector2f& b) {
	return length(b - a);
}