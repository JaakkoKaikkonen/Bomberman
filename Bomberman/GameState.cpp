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
		  player1(data, sf::Vector2f(50.0f + 5.0f, 50.0f), 1),
		  player2(data, sf::Vector2f(SCREEN_WIDTH - TILESIZE * 2 + 5.0f, 50.0f), 2),
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

				if (!player1.dying && player1.bombCount < player1.bombLimit) {
					if (event.key.code == sf::Keyboard::Space || sf::Joystick::isButtonPressed(0, 0)) {
						sf::Vector2i normalizedbombPos = sf::Vector2i(player1.getPosition() + sf::Vector2f(player1.getSprite().getGlobalBounds().width / 2, player1.getSprite().getGlobalBounds().height / 2 + 5.0f)) / TILESIZE;
						bool overlap = false;
						for (int i = 0; i < bombs.size(); i++) {
							if (bombs[i]->normalizedPos == normalizedbombPos) {
								overlap = true;
								break;
							}
						}
						if (!overlap) {
							bombs.emplace_back(new Bomb(data, player1, normalizedbombPos, player1.blastRadius));
							player1.bombCount++;
						}
					}
				}
				if (!player2.dying && player2.bombCount < player2.bombLimit) {
					if (event.key.code == sf::Keyboard::RControl || sf::Joystick::isButtonPressed(1, 0)) {
						sf::Vector2i normalizedbombPos = sf::Vector2i(player2.getPosition() + sf::Vector2f(player2.getSprite().getGlobalBounds().width / 2, player2.getSprite().getGlobalBounds().height / 2 + 5.0f)) / TILESIZE;
						bool overlap = false;
						for (int i = 0; i < bombs.size(); i++) {
							if (bombs[i]->normalizedPos == normalizedbombPos) {
								overlap = true;
								break;
							}
						}
						if (!overlap) {
							bombs.emplace_back(new Bomb(data, player2, normalizedbombPos, player2.blastRadius));
							player2.bombCount++;
						}
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Joystick::isButtonPressed(0, 2)) {
					if (player1.punchPowerUp) {
						player1.punch();
						this->punchBomb(player1);
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Joystick::isButtonPressed(1, 2)) {
					if (player2.punchPowerUp) {
						player2.punch();
						this->punchBomb(player2);
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Joystick::isButtonPressed(0, 1)) {
					if (player1.kickPowerUp) {
						this->kickBomb(player1);
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Joystick::isButtonPressed(1, 1)) {
					if (player2.kickPowerUp) {
						this->kickBomb(player2);
					}
				}

			}
		}


		float C1positionX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float C1positionY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		
		if (!player1.dying) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || C1positionY < -80) {
				player1.move(Dir::Up);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || C1positionY > 80) {
				player1.move(Dir::Down);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || C1positionX > 80) {
				player1.move(Dir::Right);
			} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || C1positionX < -80){
				player1.move(Dir::Left);
			}
		}

		float C2positionX = sf::Joystick::getAxisPosition(1, sf::Joystick::X);
		float C2positionY = sf::Joystick::getAxisPosition(1, sf::Joystick::Y);

		if (!player2.dying) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || C2positionY < -80) {
				player2.move(Dir::Up);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || C2positionY > 80) {
				player2.move(Dir::Down);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || C2positionX > 80) {
				player2.move(Dir::Right);
			} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || C2positionX < -80){
				player2.move(Dir::Left);
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

	}

	void GameState::update() {

		//Update players
		player1.update();

		if (player1.dead) {
			gameOver = true;
		}

		player2.update();

		if (player2.dead) {
			gameOver = true;
		}

		//Player Wall Collision
		for (int i = 0; i < GAMEFIELD_WIDTH; i++) {
			for (int j = 0; j < GAMEFIELD_HEIGHT; j++) {
				if (gameField[i][j] != 0) {
					blockTile.setPosition(i * TILESIZE, j * TILESIZE);
					if (Collision::bomberManCollision(blockTile, player1.getSprite(), 0.5f, 0.05f, 0.2f, 0.2f)) {
						player1.goToPreviousPosition();
					}
					if (Collision::bomberManCollision(blockTile, player2.getSprite(), 0.5f, 0.05f, 0.2f, 0.2f)) {
						player2.goToPreviousPosition();
					}
				}
			}
		}

		//Bombs
		for (int i = bombs.size() - 1; i >= 0 ; i--) {
			bombs[i]->update();

			if (bombs[i]->shouldExplode) {
				bombs[i]->explode(gameField, brickTiles, powerUps, bombs);
			}

			if (bombs[i]->hits(player1)) {
				player1.kill();
			}

			if (bombs[i]->hits(player2)) {
				player2.kill();
			}

			if (bombs[i]->explosionTimer <= 0 || bombs[i]->outOfScreen()) {
				bombs[i] = bombs[bombs.size() - 1];
				bombs.pop_back();
			}

		}

		//PowerUps
		for (int i = powerUps.size() - 1; i >= 0; i--) {
			if (powerUps[i]->burned() || powerUps[i]->collides(player1) || powerUps[i]->collides(player2)) {
				powerUps[i] = powerUps[powerUps.size() - 1];
				powerUps.pop_back();
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


		if (player2.dead) {
			player2.draw();
			player1.draw();
		} else {
			player1.draw();
			player2.draw();
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
		player1.reset();
		player2.reset();
		player1.setPosition(50.0f + 5.0f, 50.0f);
		player2.setPosition(SCREEN_WIDTH - TILESIZE * 2 + 5.0f, 50.0f);
		bombs.clear();
		powerUps.clear();
		
		this->generateMap();

		gameOver = false;
	}


}