#include "meteor.h"

meteor::meteor(float rad, sf::Vector2f position, int speed)
{
	this->circle.setRadius(rad);
	this->circle.setPosition(position);
	this->circle.setFillColor(sf::Color::Red);
	this->circle.setOrigin(this->circle.getRadius(), this->circle.getRadius());
	this->speed = speed;
}

void meteor::update(float delta)
{
	this->circle.move({0.f, this->speed * delta});
}

// thanks http://www.jeffreythompson.org/collision-detection/circle-rect.php for explaining circle rectangle collision
bool meteor::isColliding(sf::Vector2f position, sf::Vector2f size)
{
	// set edges for testing
	float testX = this->circle.getPosition().x;
	float testY = this->circle.getPosition().y;
	float cx = testX;
	float cy = testY;

	float rx = position.x;
	float ry = position.y;

	float rw = size.x;
	float rh = size.y;

	// check which edge is closest
	if (cx < rx)			testX = rx;			// test left edge
	else if (cx > rx + rw)	testX = rx + rw;	// right edge
	if (cy < ry)			testY = ry;			// top edge
	else if (cy > ry + rh)	testY = ry + rh;	// bottom edge

	// using pythagorean theorem to check distance to closest edge
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	// check if distance is lesser than radius, if so the objects must be touching
	if (distance <= this->circle.getRadius()) {
		return true;
	}
	return false;
}
