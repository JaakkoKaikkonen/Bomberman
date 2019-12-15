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

		for (int i = 0; i < GAMEFIELD_HEIGHT; i++) {
			for (int j = 0; j < GAMEFIELD_WIDTH; j++) {
				if (gameField[i][j] == 1) {
					brickTiles.emplace_back(new BrickTile(data, sf::Vector2f(j * TILESIZE, i * TILESIZE)));
				}
			}
		}

	}


	void GameState::handleInput() {
		sf::Event event;
		while (data->window.pollEvent(event)) {

			if (sf::Event::Closed == event.type) {
				data->window.close();
			}

			if (event.type == sf::Event::KeyPressed) {

				if (!player1.dying && player1.bombCount < player1.bombLimit) {
					if (event.key.code == sf::Keyboard::Space) {
						sf::Vector2i normalizeBombPos = sf::Vector2i((sf::Vector2i(((player1.getPosition() + sf::Vector2f(player1.getSprite().getGlobalBounds().width / 2, player1.getSprite().getGlobalBounds().height / 2 + 5.0f)) / (float)TILESIZE)) * TILESIZE));
						bool overlap = false;
						for (int i = 0; i < bombs.size(); i++) {
							if (bombs[i]->normalizedPos == normalizeBombPos) {
								overlap = true;
								break;
							}
						}
						if (!overlap) {
							bombs.emplace_back(new Bomb(data, player1, normalizeBombPos, player1.blastRadius));
							player1.bombCount++;
						}
					}
				}
				if (!player2.dying && player2.bombCount < player2.bombLimit) {
					if (event.key.code == sf::Keyboard::RControl) {
						sf::Vector2i normalizeBombPos = sf::Vector2i((sf::Vector2i(((player2.getPosition() + sf::Vector2f(player2.getSprite().getGlobalBounds().width / 2, player2.getSprite().getGlobalBounds().height / 2 + 5.0f)) / (float)TILESIZE)) * TILESIZE));
						bool overlap = false;
						for (int i = 0; i < bombs.size(); i++) {
							if (bombs[i]->normalizedPos == normalizeBombPos) {
								overlap = true;
								break;
							}
						}
						if (!overlap) {
							bombs.emplace_back(new Bomb(data, player2, normalizeBombPos, player2.blastRadius));
							player2.bombCount++;
						}
					}
				}

			}
		}
		
		if (!player1.dying) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				player1.move(Dir::Up);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				player1.move(Dir::Down);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				player1.move(Dir::Right);
			} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				player1.move(Dir::Left);
			}
		}

		if (!player2.dying) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				player2.move(Dir::Up);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)  ) {
				player2.move(Dir::Down);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)  ) {
				player2.move(Dir::Right);
			} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
				player2.move(Dir::Left);
			}
		}

		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			data->window.close();
		}

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
		for (int i = 0; i < GAMEFIELD_HEIGHT; i++) {
			for (int j = 0; j < GAMEFIELD_WIDTH; j++) {
				if (gameField[i][j] != 0) {
					blockTile.setPosition(j * TILESIZE, i * TILESIZE);
					if (Collision::bomberManCollision(blockTile, player1.getSprite(), 0.5f, 0.05f, 0.1f, 0.1f)) {
						player1.goToPreviousPosition();
					}
					if (Collision::bomberManCollision(blockTile, player2.getSprite(), 0.5f, 0.05f, 0.1f, 0.1f)) {
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

			if (bombs[i]->explosionTimer <= 0) {
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
			
		for (int i = 0; i < GAMEFIELD_HEIGHT; i++) {
			for (int j = 0; j < GAMEFIELD_WIDTH; j++) {
				if (gameField[i][j] == 0) {
					if (gameField[i - 1][j] != 0) {
						emptyBelowTile.setPosition(j * TILESIZE, i * TILESIZE);
						data->window.draw(emptyBelowTile);
					} else {
						emptyTile.setPosition(j * TILESIZE, i * TILESIZE);
						data->window.draw(emptyTile);
					}
				} else if (gameField[i][j] == 1) {

				} else if (gameField[i][j] == 2) {
					blockTile.setPosition(j * TILESIZE, i * TILESIZE);
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


}