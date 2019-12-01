#include <SFML/Graphics.hpp>

namespace Collision {

	bool checkSpriteCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
	
		return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
	}

	bool checkSpriteCollision(const sf::Sprite& sprite1, const float& scale1, const sf::Sprite& sprite2, const float& scale2) {

		sf::FloatRect rect1 = sprite1.getGlobalBounds();
		sf::FloatRect rect2 = sprite2.getGlobalBounds();

		rect1.left += (rect1.width - rect1.width * scale1) / 2;
		rect1.top += (rect1.height - rect1.height * scale1) / 2;
		rect2.left += (rect2.width - rect2.width * scale2) / 2;
		rect2.top += (rect2.height - rect2.height * scale2) / 2;

		rect1.width *= scale1;
		rect1.height *= scale1;
		rect2.width *= scale2;
		rect2.height *= scale2;

		return rect1.intersects(rect2);
	}

	bool bomberManWallCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2, const float& scale2top, const float& scale2bottom, const float& scale2Right, const float& scale2Left) {

		sf::FloatRect rect1 = sprite1.getGlobalBounds();
		sf::FloatRect rect2 = sprite2.getGlobalBounds();

		rect2.top += rect2.height * scale2top;
		rect2.left += rect2.width * scale2Left;

		rect2.height = rect2.height * (1 - scale2top) - rect2.height * scale2bottom;
		rect2.width = rect2.width * (1 - scale2Left) - rect2.width * scale2Right;

		return rect1.intersects(rect2);
	}


	float distanceBetween(const float& x1, const float& y1, const float& x2, const float& y2) {
		float x = x1 - x2;
		float y = y1 - y2;
		return sqrt(x * x + y * y);
	}

	float distanceBetween(const sf::Vector2f& vector1, const sf::Vector2f& vector2) {
		sf::Vector2f vector = vector1 - vector2;
		return sqrt(vector.x * vector.x + vector.y * vector.y);
	}

}
