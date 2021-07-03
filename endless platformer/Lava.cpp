#include "Lava.h"

Lava::Lava(sf::Vector2f size, sf::Vector2f position)
{
	lavaShape.setSize(size);
	// lavaShape.setOrigin({ 0.5f,1.f });
	lavaShape.setPosition(position);
	lavaShape.setFillColor(sf::Color::Red);
}

void Lava::update(float delta)
{
	lavaShape.move({ 0.f, speed*delta });
}

void Lava::setRise(float rise)
{
	this->speed = rise * -1.f;
}

void Lava::setPos(float y)
{
	lavaShape.setPosition({ lavaShape.getPosition().x, y });
}

void Lava::increaseRise(float increment)
{
	this->speed -= increment;
}
