#pragma once

#include <SFML/Graphics.hpp>

class platform
{
private:
	sf::RectangleShape rect;
	bool rangeIntersect(float min0, float max0, float min1, float max1);
public:
	platform(sf::Vector2f size, sf::Vector2f position);
	bool isColliding(sf::Vector2f size, sf::Vector2f position);
	void render(sf::RenderTarget & window) { window.draw(rect); }
	void setColor(sf::Color color) { rect.setFillColor(color); }

	bool touched = true;
};

