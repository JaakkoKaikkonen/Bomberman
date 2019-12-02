#pragma once

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"
#include "State.hpp"
#include "Game.hpp"
#include "Animation.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Player.h"
#include "Bomb.h"
#include "BrickTile.h"
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
		gameDataRef data;

		sf::Sprite emptyTile;
		sf::Sprite emptyBelowTile;
		sf::Sprite blockTile;

		std::vector<BrickTile*> brickTiles;

		Player player;

		std::vector<Bomb*> bombs;

		bool gameOver = false;

		sf::Text gameOverText;

		int gameField[GAMEFIELD_HEIGHT][GAMEFIELD_WIDTH] = {
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		{2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 2},
		{2, 0, 2, 0, 2, 0, 2, 0, 2, 1, 2, 1, 2, 0, 2},
		{2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2},
		{2, 1, 2, 1, 2, 1, 2, 0, 2, 0, 2, 1, 2, 1, 2},
		{2, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
		{2, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2},
		{2, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 2},
		{2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 2, 1, 2},
		{2, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 2},
		{2, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 2},
		{2, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 2},
		{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
		};

	};

}

