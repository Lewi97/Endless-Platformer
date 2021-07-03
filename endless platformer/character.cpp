#include "character.h"

character::character(sf::Vector2f position, int speed, sf::Color color)
{
	body.setSize({ 10.f,10.f });
	body.setPosition(position);
	body.setFillColor(color);
	body.setOutlineColor(sf::Color::Black);
	this->speed = speed;
}

void character::move(float x)
{
	body.move({ x * this->speed,0.f });
}

void character::fall(float delta)
{
	body.move({ 0.f, (this->speed * 2) * delta });
}

void character::render(sf::RenderWindow& window)
{
	window.draw(body);
}

void character::jump(float jumpStrength)
{
	yTarget = body.getPosition().y - jumpStrength;
	jumping = true;
}

void character::update(float delta)
{
	if (jumping)
	{
		//body.move({ 0.f, -(this->speed) * delta });  // use fall function with inverse delta | only once it works lol
		fall(-delta);
		if (body.getPosition().y < yTarget) 
		{ 
			jumping = false; 
		}
	}
}