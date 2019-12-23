#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Animation.hpp"
#include "BrickTile.h"
//#include "Bomb.h"
#include <vector>

namespace Game {

	class Player
	{
	public:
		Player(gameDataRef data, sf::Vector2f position, int playerNumber);

		void move(Dir dir);

		void update();

		void goToPreviousPosition();

		void punch();

		void draw();

		void kill() { dying = true; }

		sf::Sprite getSprite() { return player; }

		sf::Vector2f getPosition() { return player.getPosition(); }

		void setPosition(sf::Vector2f position) { player.setPosition(position); }

		void reset();

	public:
		bool dead = false;
		bool dying = false;

		int blastRadius = 2;

		float speed = 3.0f;

		int bombLimit = 1;
		int bombCount = 0;

		bool punchPowerUp = false;
		bool kickPowerUp = false;

		Dir dir = Dir::Down;

	private:
		gameDataRef data;

		sf::Sprite player;

		sf::IntRect walkingDownAnimationFrames[4];
		Animation walkingDownAnimation;

		sf::IntRect walkingUpAnimationFrames[4];
		Animation walkingUpAnimation;

		sf::IntRect walkingRightAnimationFrames[4];
		Animation walkingRightAnimation;

		sf::IntRect walkingLeftAnimationFrames[4];
		Animation walkingLeftAnimation;

		sf::IntRect dyingAnimationFrames[6];
		Animation dyingAnimation;

		sf::IntRect punchUpFrame, punchDownFrame, punchRightFrame, punchLeftFrame;

		sf::Vector2f previousPosition;

		bool moving = false;
		bool moved = false;

		int dyingTime = 60;
		int dyingTimer = 0;

		int punchTimer = 20;
		int punchTime = 20;

	};

}

