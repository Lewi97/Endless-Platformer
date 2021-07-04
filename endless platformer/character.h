#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class character
{
private:
	int speed = 100;
	sf::RectangleShape body;
	bool jumping = false;
	float yTarget = 0.f;

public:
	// constructor
	character(sf::Vector2f position, int speed, sf::Color color);
	
	// movement
	void move(float x);
	void fall(float delta);
	void jump(float jumpStrength);

	// update
	void update(float delta);
	
	// rendering
	void render(sf::RenderWindow& window);
	
	// setter
	bool falling = false;
	void setPos(sf::Vector2f pos) { body.setPosition(pos); }
	void stopJumping() { jumping = false; }

	// getters
	bool isJumping()		{ return jumping;				}
	bool isFalling()		{ return falling;				}
	sf::Vector2f getSize()	{ return body.getSize();		}
	sf::Vector2f getPos()	{ return body.getPosition();	}
};

