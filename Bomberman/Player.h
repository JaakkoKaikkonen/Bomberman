#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"

namespace Game {

	class Player
	{
	public:
		Player(gameDataRef data);

		void move(Dir dir);

		void update();

		void goToPreviousPosition();

		void draw();

		void kill() { dying = true; }

		sf::Sprite getSprite() { return player; }

		sf::Vector2f getPosition() { return player.getPosition(); }

	public:
		bool dead = false;
		bool dying = false;

	private:
		gameDataRef data;

		sf::Sprite player;

		sf::IntRect walkingDownAnimationFrames [4] = { BOMBERMAN_1_WALK_DOWN_2, BOMBERMAN_1_WALK_DOWN_1, BOMBERMAN_1_WALK_DOWN_2, BOMBERMAN_1_WALK_DOWN_3 };
		Animation walkingDownAnimation;

		sf::IntRect walkingUpAnimationFrames [4] = { BOMBERMAN_1_WALK_UP_2, BOMBERMAN_1_WALK_UP_1, BOMBERMAN_1_WALK_UP_2, BOMBERMAN_1_WALK_UP_3 };
		Animation walkingUpAnimation;

		sf::IntRect walkingRightAnimationFrames [4] = { BOMBERMAN_1_WALK_RIGHT_1, BOMBERMAN_1_WALK_RIGHT_2, BOMBERMAN_1_WALK_RIGHT_1, BOMBERMAN_1_WALK_RIGHT_3 };
		Animation walkingRightAnimation;

		sf::IntRect walkingLeftAnimationFrames [4] = { BOMBERMAN_1_WALK_LEFT_1, BOMBERMAN_1_WALK_LEFT_2, BOMBERMAN_1_WALK_LEFT_1, BOMBERMAN_1_WALK_LEFT_3 };
		Animation walkingLeftAnimation;

		sf::IntRect dyingAnimationFrames [6] = { BOMBERMAN_1_DEAD_1, BOMBERMAN_1_DEAD_2, BOMBERMAN_1_DEAD_3, BOMBERMAN_1_DEAD_4, BOMBERMAN_1_DEAD_5, BOMBERMAN_1_DEAD_6 };
		Animation dyingAnimation;

		Dir dir = Dir::Down;

		sf::Vector2f previousPosition;

		float speed = 3.0f;

		bool moving = false;
		bool moved = false;

		int dyingTimer = 60;

	};

}

