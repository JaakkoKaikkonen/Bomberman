#pragma once

#include <SFML/Graphics.hpp>

namespace Collision {

	bool checkSpriteCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
	bool checkSpriteCollision(const sf::Sprite& sprite1, const float& scale1, const sf::Sprite& sprite2, const float& scale2);
	bool bomberManCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2, const float& scale2top, const float& scale2bottom, const float& scale2Right, const float& scale2Left);

	float distanceBetween(const float& x1, const float& y1, const float& x2, const float& y2);
	float distanceBetween(const sf::Vector2f& vector1, const sf::Vector2f& vector2);

}

