#pragma once

#include <SFML/Graphics.hpp>

class meteor
{
private:
	// shape
	sf::CircleShape circle;

	// falling speed default value
	int speed = 100;

	bool rangeIntersect(float min0, float max0, float min1, float max1);
public:
	// constructor
	meteor(float rad, sf::Vector2f position, int speed);
	
	// update
	void update(float delta);

	// render
	void render(sf::RenderTarget& window) { window.draw(circle); }

	// collision check with player
	bool isColliding(sf::Vector2f position, sf::Vector2f size);
};

