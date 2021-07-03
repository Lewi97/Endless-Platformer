#pragma once

#include <SFML/Graphics.hpp>

class Lava
{
private:
	sf::RectangleShape lavaShape;
	float speed = -10.f;
public:
	// constructor
	Lava(sf::Vector2f size, sf::Vector2f position);

	// renderer
	void render(sf::RenderTarget& window) { window.draw(lavaShape); }

	// update
	void update(float delta);

	// getters
	float getPosition() { return lavaShape.getPosition().y; }
	
	// setters
	void setRise(float rise);
	void setPos(float y);
	void increaseRise(float increment);
};

