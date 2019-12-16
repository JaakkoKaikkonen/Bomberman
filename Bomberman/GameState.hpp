#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "DEFINITIONS.hpp"
#include "State.hpp"
#include "Game.hpp"
#include "Animation.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Player.h"
#include "Bomb.h"
#include "BrickTile.h"
#include "PowerUp.h"
#include <vector>

namespace Game {

	class GameState : public State
	{
	public:
		GameState(gameDataRef data);

		void handleInput();
		void update();
		void draw();
	
	private:
		void generateMap();
		void reset();

	private:
		gameDataRef data;

		sf::Sprite emptyTile;
		sf::Sprite emptyBelowTile;
		sf::Sprite blockTile;

		std::vector<BrickTile*> brickTiles;

		std::vector<PowerUp*> powerUps;

		Player player1;

		Player player2;

		std::vector<Bomb*> bombs;

		bool gameOver = false;

		sf::Text gameOverText;

		int gameField[GAMEFIELD_HEIGHT][GAMEFIELD_WIDTH];

	};

}

