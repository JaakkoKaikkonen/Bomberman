#pragma once

#include <SFML\Graphics.hpp>
#include "Game.hpp"

#include "DEFINITIONS.hpp"
#include "Player.h"
#include "Animation.hpp"

namespace Game {

	class PowerUp
	{
	public:
		PowerUp(gameDataRef data, PowerUpName powerUp, sf::Vector2f position);

		bool collides(Player& player);

		void burn();

		bool burned();

		void draw();

		sf::Vector2f getPosition() { return powerUpSprite.getPosition(); };

	private:
		gameDataRef data;

		sf::Sprite powerUpSprite;

		PowerUpName powerUp;

		sf::IntRect burnAnimationFrames[7] = { POWER_UP_BUNR_1, POWER_UP_BUNR_2, POWER_UP_BUNR_3, POWER_UP_BUNR_4, POWER_UP_BUNR_5, POWER_UP_BUNR_6, POWER_UP_BUNR_7 };
		Animation burnAnimation;

		bool burning = false;

		float burnTime = 1.0f;
		sf::Clock burnTimer;


	};

}

