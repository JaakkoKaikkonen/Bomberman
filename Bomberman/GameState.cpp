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
		  player1StartPosition(50.0f + 5.0f, 50.0f),
		  player2StartPosition(SCREEN_WIDTH - TILESIZE * 2 + 5.0f, 50.0f),
		  player3StartPosition(50.0f + 5.0f, SCREEN_HEIGHT - TILESIZE * 2.5),
		  player4StartPosition(SCREEN_WIDTH - TILESIZE * 2 + 5.0f, SCREEN_HEIGHT - TILESIZE * 2.5),
		  gameOverText("GAME OVER", data->assets.getFont("Font"), 32)
	{	
		gameOverText.setOrigin(gameOverText.getGlobalBounds().width / 2, gameOverText.getGlobalBounds().height / 2);
		gameOverText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.26f);

		emptyTile.setScale(3.125f, 3.125f);
		emptyBelowTile.setScale(3.125f, 3.125f);
		blockTile.setScale(3.125f, 3.125f);

		this->generateMap();

	}


	void GameState::handleInput() {
		sf::Event event;
		while (data->window.pollEvent(event)) {

			if (sf::Event::Closed == event.type) {
				data->window.close();
			}


			if (event.type == sf::Event::EventType::JoystickButtonPressed) {
                std::cout << "Joystick ID : " << event.joystickButton.joystickId << std::endl;
                std::cout << "Joystick Button : " << event.joystickButton.button << std::endl;
			}


			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::EventType::JoystickButtonPressed) {

				for (int i = 0; i < players.size(); i++) {
					if (!players[i]->dying && players[i]->bombCount < players[i]->bombLimit) {
						if ((i == 0 && event.key.code == sf::Keyboard::Space) ||
							(i == 1 && event.key.code == sf::Keyboard::RControl) ||
							sf::Joystick::isButtonPressed(i, 0)) {

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
						sf::Joystick::isButtonPressed(i, 2)) {

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

			float analogStickPositionX = sf::Joystick::getAxisPosition(i, sf::Joystick::X);
			float analogStickPositionY = sf::Joystick::getAxisPosition(i, sf::Joystick::Y);
		
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


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) || sf::Joystick::isButtonPressed(0, 7)) {
			this->reset();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			data->window.close();
		}


		/*std::cout << "Joystick 0 connected: " << sf::Joystick::isConnected(0) << std::endl;
		std::cout << "Joystick 1 connected: " << sf::Joystick::isConnected(1) << std::endl;
		std::cout << sf::Joystick::getButtonCount(0) << std::endl;*/

		if (sf::Joystick::isConnected(0) && !player1Connected) {
			player1Connected = true;
			players.emplace_back(new Player(data, player1StartPosition, 1));
		}
		if (sf::Joystick::isConnected(1) && !player2Connected) {
			player2Connected = true;
			players.emplace_back(new Player(data, player2StartPosition, 2));
		}
		if (sf::Joystick::isConnected(2) && !player3Connected) {
			player3Connected = true;
			players.emplace_back(new Player(data, player3StartPosition, 3));
		}
		if (sf::Joystick::isConnected(3) && !player4Connected) {
			player4Connected = true;
			players.emplace_back(new Player(data, player4StartPosition, 4));
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
			bombs[i]->update();

			if (bombs[i]->shouldExplode) {
				bombs[i]->explode(gameField, brickTiles, powerUps, bombs);
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

			for (int j = 0; j < players.size(); j++) {
				if (powerUps[i]->collides(*players[j]) || powerUps[i]->burned()) {
					powerUps[i] = powerUps[powerUps.size() - 1];
					powerUps.pop_back();
					break;
				}
			}

		}

		//BrickTiles
		for (int i = brickTiles.size() - 1; i >= 0 ; i--) {
			if (brickTiles[i]->burned()) {
				if (brickTiles[i]->powerUp != PowerUpName::None) {
					powerUps.emplace_back(new PowerUp(data, brickTiles[i]->powerUp, brickTiles[i]->getPosition()));
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
				} else if (gameField[i][j] == 1) {

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
			if (i == 0) {
				players[i]->setPosition(player1StartPosition);
			} else if (i == 1) {
				players[i]->setPosition(player2StartPosition);
			} else if (i == 2) {
				players[i]->setPosition(player3StartPosition);
			} else if (i == 3) {
				players[i]->setPosition(player4StartPosition);
			}
		}

		bombs.clear();
		powerUps.clear();
		
		this->generateMap();

		gameOver = false;
	}


}