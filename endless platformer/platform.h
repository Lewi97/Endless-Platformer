#pragma once

#include <SFML/Graphics.hpp>

class platform
{
private:
	sf::RectangleShape rect;
	bool rangeIntersect(float min0, float max0, float min1, float max1);
public:
	// constructor
	platform(sf::Vector2f size, sf::Vector2f position);

	// renderer
	void render(sf::RenderTarget & window) { window.draw(rect); }
	
	// getters
	bool isColliding(sf::Vector2f size, sf::Vector2f position);
	sf::Vector2f getPos()			{ return rect.getPosition(); }

	// setters
	void setColor(sf::Color color)	{ rect.setFillColor(color); }
	void setPos(sf::Vector2f pos)	{ rect.setPosition(pos); }

	bool touched = true;
};

