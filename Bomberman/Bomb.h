#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"
#include "BrickTile.h"
#include "Player.h"
#include "PowerUp.h"

namespace Game {

	class Bomb
	{
	public:
		struct ExplosionAnimationFrame {
			sf::Vector2f position;
			sf::Sprite* sprite = nullptr;
		};

		Bomb(gameDataRef data, Player& player, sf::Vector2i position, int blastRadius);

		void update();

		void move(sf::Vector2i position);

		void explode(int gameField[GAMEFIELD_WIDTH][GAMEFIELD_HEIGHT], std::vector<BrickTile*>& brickTiles, std::vector<PowerUp*>& powerUps, std::vector<Bomb*>& bombs);

		bool hits(Player& player);

		bool outOfScreen();

		void draw();

		sf::Vector2f getPosition() { return bomb.getPosition(); }

	public:

		bool shouldExplode = false;

		bool exploded = false;

		int explosionTimer = 30;

		sf::Vector2i normalizedPos;

		int lifeTime = 120;

		bool moving = false;
		

	private:
		gameDataRef data;

		Player& player;

		sf::Sprite bomb;
		sf::IntRect bombAnimationFrames[3] = { BOMB_1, BOMB_2, BOMB_3 };
		Animation bombAnimation;

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
		sf::Sprite explosionVerticalPart;
		sf::IntRect explosionVerticalPartAnimationFrames[5] = { EXPLOSION_VERTICAL_PART_1, EXPLOSION_VERTICAL_PART_2, EXPLOSION_VERTICAL_PART_3, EXPLOSION_VERTICAL_PART_4, EXPLOSION_VERTICAL_PART_5 };
		Animation explosionVerticalPartAnimation;
		sf::Sprite explosionHorizontalPart;
		sf::IntRect explosionHorizontalPartAnimationFrames[5] = { EXPLOSION_HORIZONTAL_PART_1, EXPLOSION_HORIZONTAL_PART_2, EXPLOSION_HORIZONTAL_PART_3, EXPLOSION_HORIZONTAL_PART_4, EXPLOSION_HORIZONTAL_PART_5 };
		Animation explosionHorizontalPartAnimation;


		std::vector<ExplosionAnimationFrame*> explosionAnimationFrameList;

		int blastRadius;

		int explosionDelayAfterHitByExplosion = 5;

		bool hitByExplosion = false;


		float bombMoveSpeed = 7.0f;

		float moveAnimationPercentage = 0.0f;
		float moveAnimationPercentageStep;

		sf::Vector2f startPosition;

		sf::Vector2i newPosition;

	};

}
