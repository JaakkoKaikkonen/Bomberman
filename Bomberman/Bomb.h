#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"
#include "BrickTile.h"

namespace Game {

	class Bomb
	{
	public:
		Bomb(gameDataRef data, sf::Vector2f position);

		void update();

		void explode(int gameField[GAMEFIELD_HEIGHT][GAMEFIELD_WIDTH], std::vector<BrickTile*> brickTiles);

		void draw();

		sf::Vector2f getPosition() { return bomb.getPosition(); }

	public:

		bool shouldExplode = false;

		bool exploded = false;

		int explosionTimer = 30;

	private:
		gameDataRef data;

		sf::Sprite bomb;

		sf::Sprite explosionMiddle;
		sf::IntRect explosionMiddleAnimationFrames[5] = { EXPLOSION_MIDDLE_1, EXPLOSION_MIDDLE_2, EXPLOSION_MIDDLE_3, EXPLOSION_MIDDLE_4, EXPLOSION_MIDDLE_5 };
		Animation explosionMiddleAnimation;
		sf::Sprite explosionUp;
		sf::IntRect explosionUpAnimationFrames[5] = { EXPLOSION_UP_1, EXPLOSION_UP_2, EXPLOSION_UP_3, EXPLOSION_UP_4, EXPLOSION_UP_5 };
		Animation explosionUpAnimation;
		sf::Sprite explosionDown;
		sf::IntRect explosionDownAnimationFrames[5] = { EXPLOSION_DOWN_1, EXPLOSION_DOWN_2, EXPLOSION_DOWN_3, EXPLOSION_DOWN_4, EXPLOSION_DOWN_5 };
		Animation explosionDownAnimation;
		sf::Sprite explosionRight;
		sf::IntRect explosionRightAnimationFrames[5] = { EXPLOSION_RIGHT_1, EXPLOSION_RIGHT_2, EXPLOSION_RIGHT_3, EXPLOSION_RIGHT_4, EXPLOSION_RIGHT_5 };
		Animation explosionRightAnimation;
		sf::Sprite explosionLeft;
		sf::IntRect explosionLeftAnimationFrames[5] = { EXPLOSION_LEFT_1, EXPLOSION_LEFT_2, EXPLOSION_LEFT_3, EXPLOSION_LEFT_4, EXPLOSION_LEFT_5 };
		Animation explosionLeftAnimation;

		sf::IntRect bombAnimationFrames[3] = { BOMB_1, BOMB_2, BOMB_3 };

		Animation bombAnimation;

		int lifeTime = 120;

		

		

	};

}
