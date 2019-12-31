#include "Game.hpp"
#include "GameState.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>

namespace Game {

	Game::Game(int width, int height, std::string title) {

		sf::Image icon;
		icon.loadFromFile(TOP_ICON_FILEPATH);

		data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
		//_data->window.setVerticalSyncEnabled(true);
		//_data->window.setFramerateLimit(60);

		data->window.setPosition(sf::Vector2i(500, 100));

		data->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		srand((unsigned int)time(NULL));

		//Load Resources ----------------------------------------------------------------------

		//Textures
		data->assets.loadTexture("Player", PLAYER_SPRITES_FILEPATH);
		data->assets.loadTexture("Tiles", BOMB_SPRITES_FILEPATH);
		data->assets.loadTexture("PowerUps", POWER_UP_SPRITES_FILEPATH);

		//Sounds
		data->assets.loadSound("Bomb", BOMB_SOUND_FILEPATH);
		data->assets.loadSound("PowerUp", POWER_UP_SOUND_FILEPATH);

		data->music.openFromFile(MUSIC_FILEPATH);

		//Fonts
		data->assets.loadFont("Font", FONT_FILEPATH);

		//-------------------------------------------------------------------------------------

		data->state = new GameState(data);

		this->run();
	}

	void Game::run() {
		float newTime, frameTime;

		float currentTime = clock.getElapsedTime().asSeconds();

		float accumulator = dt;


		while (data->window.isOpen()) {

			newTime = clock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			currentTime = newTime;

			if (frameTime > 0.15f) {
				frameTime = 0.15f;
			}

			accumulator += frameTime;

			while (accumulator >= dt)
			{
				data->state->handleInput();
				data->state->update();

				data->state->draw();

				accumulator -= dt;
			}

		}

	}


}