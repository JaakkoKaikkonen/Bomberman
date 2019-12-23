#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "DEFINITIONS.hpp"
#include "State.hpp"
#include "Game.hpp"
#include "Animation.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Bomb.h"
#include "Player.h"
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
		void punchBomb(Player player);
		void kickBomb(Player player);

		void generateMap();
		void reset();


	private:
		gameDataRef data;

		sf::Sprite emptyTile;
		sf::Sprite emptyBelowTile;
		sf::Sprite blockTile;

		std::vector<BrickTile*> brickTiles;

		std::vector<PowerUp*> powerUps;

		std::vector<Player*> players;

		bool player1Connected, player2Connected, player3Connected, player4Connected;

		const sf::Vector2f player1StartPosition;
		const sf::Vector2f player2StartPosition;
		const sf::Vector2f player3StartPosition;
		const sf::Vector2f player4StartPosition;

		std::vector<Bomb*> bombs;

		bool gameOver = false;

		sf::Text gameOverText;

		int gameField[GAMEFIELD_WIDTH][GAMEFIELD_HEIGHT];

	};

}

