#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"

#include "DEFINITIONS.hpp"
#include "Animation.hpp"

namespace Game {

	class BrickTile
	{
	public:
		BrickTile(gameDataRef data, sf::Vector2f position);

		void burn();

		bool burned();

		void draw();

		sf::Vector2f getPosition() { return brickTile.getPosition(); }

	public:
		PowerUpName powerUp = PowerUpName::None;
		
	private:
		gameDataRef data;

		sf::Sprite brickTile;

		sf::IntRect burningTileAnimationFrames[6] = { BURNING_BRICK_TILE_1, BURNING_BRICK_TILE_2, BURNING_BRICK_TILE_3, BURNING_BRICK_TILE_4, BURNING_BRICK_TILE_5, BURNING_BRICK_TILE_6 };

		Animation burningTileAnimation;

		bool burning = false;

		float burnTime = 0.75f;
		sf::Clock burnTimer;

	};

}