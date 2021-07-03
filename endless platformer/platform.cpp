#include "platform.h"

platform::platform(sf::Vector2f size, sf::Vector2f position)
{
	rect.setSize(size);
	rect.setPosition(position);
}

bool platform::isColliding(sf::Vector2f size, sf::Vector2f position)
{
	sf::Vector2f pos = rect.getPosition();
	sf::Vector2f rectSize = rect.getSize();
	return	rangeIntersect(position.x, position.x + size.x, pos.x, pos.x + rectSize.x) &&
			rangeIntersect(position.y, position.y + size.y, pos.y, pos.y + rectSize.y);
}

bool platform::rangeIntersect(float min0, float max0, float min1, float max1)
{
	return	std::max(min0, max0) >= std::min(min1,max1) &&
			std::min(min0, max0 )<= std::max(min1, max1);
}