#include "GameState.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>
#include "Utility.hpp"
#include "Collision.hpp"


namespace Game {

	GameState::GameState(gameDataRef data)
		: data(data),
		  emptyTile(data->assets.getTexture("Tiles"), EMPTY_TILE),
		  emptyBelowTile(data->assets.getTexture("Tiles"), EMPTY_BELOW_TILE),
		  blockTile(data->assets.getTexture("Tiles"), BLOCK_TILE), 
		  gameOverText("GAME OVER", data->assets.getFont("Font"), 32)
	{	
		gameOverText.setOrigin(gameOverText.getGlobalBounds().width / 2, gameOverText.getGlobalBounds().height / 2);
		gameOverText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.26f);

		emptyTile.setScale(3.125f, 3.125f);
		emptyBelowTile.setScale(3.125f, 3.125f);
		blockTile.setScale(3.125f, 3.125f);

		this->generateMap();

		//Set player startPositions
		playerStartPositions[0] = sf::Vector2f(50.0f + 5.0f, 50.0f);
		playerStartPositions[1] = sf::Vector2f(SCREEN_WIDTH - TILESIZE * 2 + 5.0f, 50.0f);
		playerStartPositions[2] = sf::Vector2f(50.0f + 5.0f, SCREEN_HEIGHT - TILESIZE * 2.5);
		playerStartPositions[3] = sf::Vector2f(SCREEN_WIDTH - TILESIZE * 2 + 5.0f, SCREEN_HEIGHT - TILESIZE * 2.5);

		//Spawn 2 players
		players.emplace_back(new Player(data, playerStartPositions[0], 1));
		players.emplace_back(new Player(data, playerStartPositions[1], 2));

		//Start music
		data->music.setLoop(true);
		data->music.play();
	}


	void GameState::handleInput() {
		sf::Event event;
		while (data->window.pollEvent(event)) {

			if (sf::Event::Closed == event.type) {
				data->window.close();
			}

			/*if (event.type == sf::Event::EventType::JoystickButtonPressed) {
                std::cout << "Joystick ID : " << event.joystickButton.joystickId << std::endl;
                std::cout << "Joystick Button : " << event.joystickButton.button << std::endl;
			}*/

			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::EventType::JoystickButtonPressed) {

				for (int i = 0; i < players.size(); i++) {
					if (!players[i]->dying && players[i]->bombCount < players[i]->bombLimit) {
						if ((i == 0 && event.key.code == sf::Keyboard::Space) ||
							(i == 1 && event.key.code == sf::Keyboard::RControl) ||
							sf::Joystick::isButtonPressed(i, (unsigned int)ControllerButton::A)) {

							sf::Vector2i normalizedbombPos = sf::Vector2i(players[i]->getPosition() + sf::Vector2f(players[i]->getSprite().getGlobalBounds().width / 2, players[i]->getSprite().getGlobalBounds().height / 2 + 5.0f)) / TILESIZE;
							bool overlap = false;
							for (int j = 0; j < bombs.size(); j++) {
								if (bombs[j]->normalizedPos == normalizedbombPos) {
									overlap = true;
									break;
								}
							}
							if (!overlap) {
								bombs.emplace_back(new Bomb(data, *players[i], normalizedbombPos, players[i]->blastRadius));
								players[i]->bombCount++;
							}

						}
					}

					if ((i == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) || 
						(i == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) || 
						sf::Joystick::isButtonPressed(i, (unsigned int)ControllerButton::X)) {

						if (players[i]->punchPowerUp) {
							players[i]->punch();
							this->punchBomb(*players[i]);
						}

						if (players[i]->kickPowerUp) {
							players[i]->punch();
							this->kickBomb(*players[i]);
						}

					}

				}

			}
		}


		for (int i = 0; i < players.size(); i++) {

			float analogStickPositionX = 0;
			float analogStickPositionY = 0;

			if (playersConnected[i]) {
				analogStickPositionX = sf::Joystick::getAxisPosition(i, sf::Joystick::X);
				analogStickPositionY = sf::Joystick::getAxisPosition(i, sf::Joystick::Y);
			}
		
			if (!players[i]->dying) {
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) && i == 0) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && i == 1) || analogStickPositionY < -80) {
					players[i]->move(Dir::Up);
				} else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) && i == 0) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && i == 1) || analogStickPositionY > 80) {
					players[i]->move(Dir::Down);
				} else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && i == 0) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && i == 1) || analogStickPositionX > 80) {
					players[i]->move(Dir::Right);
				} else if((sf::Keyboard::isKeyPressed(sf::Keyboard::A) && i == 0) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && i == 1) || analogStickPositionX < -80){
					players[i]->move(Dir::Left);
				}
			}

		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) || sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButton::Menu)) {
			this->reset();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			data->window.close();
		}



		for (int i = 0; i < 4; i++) {
			if (sf::Joystick::isConnected(i) && !playersConnected[i]) {
				playersConnected[i] = true;
				if (i >= 2) {
					players.emplace_back(new Player(data, playerStartPositions[i], i + 1));
				}
			}
		}


	}

	void GameState::update() {

		//Update players
		int playersDeadCounter = 0;
		for (int i = 0; i < players.size(); i++) {

			players[i]->update();

			
			if (players[i]->dead) {
				playersDeadCounter++;
			}

			//Player Wall Collision
			for (int j = 0; j < GAMEFIELD_WIDTH; j++) {
				for (int k = 0; k < GAMEFIELD_HEIGHT; k++) {
					if (gameField[j][k] != 0) {
						blockTile.setPosition(j * TILESIZE, k * TILESIZE);
						if (Collision::bomberManCollision(blockTile, players[i]->getSprite(), 0.5f, 0.05f, 0.2f, 0.2f)) {
							players[i]->goToPreviousPosition();
						}
					}
				}
			}

		}

		if (playersDeadCounter >= players.size() - 1 && players.size() > 1) {
			gameOver = true;
		}


		//Bombs
		for (int i = bombs.size() - 1; i >= 0 ; i--) {
			bombs[i]->update(powerUps);

			if (bombs[i]->shouldExplode) {
				bombs[i]->explode(gameField, brickTiles, powerUps, bombs);
				data->assets.getSound("Bomb").play();
			}

			for (int j = 0; j < players.size(); j++) {
				if (bombs[i]->hits(*players[j])) {
					players[j]->kill();
				}
			}

			if (bombs[i]->explosionTimer <= 0 || bombs[i]->outOfScreen()) {
				bombs[i] = bombs[bombs.size() - 1];
				bombs.pop_back();
			}

		}

		//PowerUps
		for (int i = powerUps.size() - 1; i >= 0; i--) {

			if (powerUps[i]->burned()) {
				powerUps[i] = powerUps[powerUps.size() - 1];
				powerUps.pop_back();
				continue;
			}

			for (int j = 0; j < players.size(); j++) {
				if (powerUps[i]->collides(*players[j])) {
					powerUps[i] = powerUps[powerUps.size() - 1];
					powerUps.pop_back();
					data->assets.getSound("PowerUp").play();
					break;
				}
			}

		}

		//BrickTiles
		for (int i = brickTiles.size() - 1; i >= 0 ; i--) {
			if (brickTiles[i]->burned()) {
				if (brickTiles[i]->powerUp != PowerUpName::None) {
					powerUps.emplace_back(new PowerUp(data, brickTiles[i]->powerUp, brickTiles[i]->getPosition()));
					for (int i = 0; i < bombs.size(); i++) {
						if (Collision::checkSpriteCollision(powerUps[powerUps.size() - 1]->getSprite(), bombs[i]->getSprite())) {
							bombs[i]->onTopOfPowerUp = true;
						}
					}
				}
				brickTiles[i] = brickTiles[brickTiles.size() - 1];
				brickTiles.pop_back();
			}
		}

	}

	void GameState::draw() {

		data->window.clear();
			
		for (int i = 0; i < GAMEFIELD_WIDTH; i++) {
			for (int j = 0; j < GAMEFIELD_HEIGHT; j++) {
				if (gameField[i][j] == 0) {
					if (gameField[i][j - 1] != 0) {
						emptyBelowTile.setPosition(i * TILESIZE, j * TILESIZE);
						data->window.draw(emptyBelowTile);
					} else {
						emptyTile.setPosition(i * TILESIZE, j * TILESIZE);
						data->window.draw(emptyTile);
					}
				} else if (gameField[i][j] == 2) {
					blockTile.setPosition(i * TILESIZE, j * TILESIZE);
					data->window.draw(blockTile);
				}
			}
		}

		for (int i = 0; i < powerUps.size(); i++) {
			powerUps[i]->draw();
		}

		for (int i = 0; i < brickTiles.size(); i++) {
			brickTiles[i]->draw();
		}

		for (int i = 0; i < bombs.size(); i++) {
			bombs[i]->draw();
		}

		//Draw dead player under alive players
		for (int i = 0; i < players.size(); i++) {
			if (players[i]->dead) {
				players[i]->draw();
			}
		}
		for (int i = 0; i < players.size(); i++) {
			if (!players[i]->dead) {
				players[i]->draw();
			}
		}


		if (gameOver) {
			data->window.draw(gameOverText);
		}

		data->window.display();
	
	}


	void GameState::punchBomb(Player player) {
		
		sf::Vector2i normalizedPlayerPos = sf::Vector2i((player.getPosition().x + player.getSprite().getGlobalBounds().width / 2) / (float)TILESIZE, (player.getPosition().y + player.getSprite().getGlobalBounds().height / 2) / (float)TILESIZE);

		if (Dir::Up == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x, normalizedPlayerPos.y - 1);
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y - step] != 0) {
						while (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y - step] != 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x, bombs[i]->normalizedPos.y - step));
						bombs[i]->fly = true;
					} 
					break;
				}
			}

		} else if (Dir::Down == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x, normalizedPlayerPos.y + 1);
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y + step] != 0) {
						while (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y + step] != 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x, bombs[i]->normalizedPos.y + step));
						bombs[i]->fly = true;
					}
					break;
				}
			}

		} else if (Dir::Right == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x + 1, normalizedPlayerPos.y);
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x + step][bombs[i]->normalizedPos.y] != 0) {
						while (gameField[bombs[i]->normalizedPos.x + step][bombs[i]->normalizedPos.y] != 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x + step, bombs[i]->normalizedPos.y));
						bombs[i]->fly = true;
					}
					break;
				}
			}

		} else if (Dir::Left == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x - 1, normalizedPlayerPos.y );
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x  - step][bombs[i]->normalizedPos.y] != 0) {
						while (gameField[bombs[i]->normalizedPos.x - step][bombs[i]->normalizedPos.y] != 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x - step, bombs[i]->normalizedPos.y));
						bombs[i]->fly = true;
					}
					break;
				}
			}

		}

	}

	void GameState::kickBomb(Player player) {
		
		sf::Vector2i normalizedPlayerPos = sf::Vector2i((player.getPosition().x + player.getSprite().getGlobalBounds().width / 2) / (float)TILESIZE, (player.getPosition().y + player.getSprite().getGlobalBounds().height / 2) / (float)TILESIZE);

		if (Dir::Up == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x, normalizedPlayerPos.y - 1);
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y - step] == 0) {
						while (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y - (step + 1)] == 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x, bombs[i]->normalizedPos.y - step));
					} 
					break;
				}
			}

		} else if (Dir::Down == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x, normalizedPlayerPos.y + 1);
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y + step] == 0) {
						while (gameField[bombs[i]->normalizedPos.x][bombs[i]->normalizedPos.y + (step + 1)] == 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x, bombs[i]->normalizedPos.y + step));
					}
					break;
				}
			}

		} else if (Dir::Right == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x + 1, normalizedPlayerPos.y);
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x + step][bombs[i]->normalizedPos.y] == 0) {
						while (gameField[bombs[i]->normalizedPos.x + (step + 1)][bombs[i]->normalizedPos.y] == 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x + step, bombs[i]->normalizedPos.y));
					}
					break;
				}
			}

		} else if (Dir::Left == player.dir) {

			sf::Vector2i inFrontOfPlayerPos = sf::Vector2i(normalizedPlayerPos.x - 1, normalizedPlayerPos.y );
			
			for (int i = 0; i < bombs.size(); i++) {
				if (normalizedPlayerPos == bombs[i]->normalizedPos || inFrontOfPlayerPos == bombs[i]->normalizedPos) {
					int step = 1;
					if (gameField[bombs[i]->normalizedPos.x  - step][bombs[i]->normalizedPos.y] == 0) {
						while (gameField[bombs[i]->normalizedPos.x - (step + 1)][bombs[i]->normalizedPos.y] == 0) {
							step++;
						}
						bombs[i]->move(sf::Vector2i(bombs[i]->normalizedPos.x - step, bombs[i]->normalizedPos.y));
					}
					break;
				}
			}

		}

	}


	void GameState::generateMap() {

		for (int i = 0; i < GAMEFIELD_WIDTH; i++) {
			for (int j = 0; j < GAMEFIELD_HEIGHT; j++) {
				if (i == 0 || i == GAMEFIELD_WIDTH - 1 || j == 0 || j == GAMEFIELD_HEIGHT - 1 || (i % 2 == 0 && j % 2 == 0)) {
					gameField[i][j] = 2;
				} else {
					gameField[i][j] = 0;
				}
			}
		}

		for (int i = 0; i < GAMEFIELD_WIDTH; i++) {
			for (int j = 0; j < GAMEFIELD_HEIGHT; j++) {
				if (gameField[i][j] != 2) {
					float change = (float)rand() / RAND_MAX;
					if (change >= 0.16f) {
						gameField[i][j] = 1;
					}
				}
			}
		}

		gameField[1][1] = 0;
		gameField[2][1] = 0;
		gameField[1][2] = 0;
		gameField[GAMEFIELD_WIDTH - 2][GAMEFIELD_HEIGHT - 2] = 0;
		gameField[GAMEFIELD_WIDTH - 2][GAMEFIELD_HEIGHT - 3] = 0;
		gameField[GAMEFIELD_WIDTH - 3][GAMEFIELD_HEIGHT - 2] = 0;
		gameField[1][GAMEFIELD_HEIGHT - 2] = 0;
		gameField[2][GAMEFIELD_HEIGHT - 2] = 0;
		gameField[1][GAMEFIELD_HEIGHT - 3] = 0;
		gameField[GAMEFIELD_WIDTH - 2][1] = 0;
		gameField[GAMEFIELD_WIDTH - 2][2] = 0;
		gameField[GAMEFIELD_WIDTH - 3][1] = 0;

		brickTiles.clear();
		for (int i = 0; i < GAMEFIELD_WIDTH; i++) {
			for (int j = 0; j < GAMEFIELD_HEIGHT; j++) {
				if (gameField[i][j] == 1) {
					brickTiles.emplace_back(new BrickTile(data, sf::Vector2f(i * TILESIZE, j * TILESIZE)));
				}
			}
		}

	}


	void GameState::reset() {
		for (int i = 0; i < players.size(); i++) {
			players[i]->reset();
			players[i]->setPosition(playerStartPositions[i]);
		}

		bombs.clear();
		powerUps.clear();
		
		this->generateMap();

		gameOver = false;
	}


}