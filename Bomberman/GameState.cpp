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
		  player(data),
		  gameOverText("GAME OVER", data->assets.getFont("Font"), 32),
		  player2(data)
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

		//Networking////////////////////////////////////////////////////////////////////////////
		if (socket.bind(port) != sf::Socket::Done) {
			// error...
			std::cout << "udp socket bind error" << std::endl;
		}


#ifdef SERVER

		// UDP socket:
		char flag[100] = "connect";
		if (socket.send(flag, 100, ip, port) != sf::Socket::Done)
		{
			std::cout << "udp socket send error" << std::endl;
		}
		std::cout << "send connect" << std::endl;

#endif
#ifdef CLIENT

		char flag[100] = "";
		std::size_t received;
		if (socket.receive(flag, 100, received, ip, port) != sf::Socket::Done)
		{
			std::cout << "udp socket receive error" << std::endl;
		}
		std::cout << "Received " << received << " bytes from " << ip << " on port " << port << std::endl;
		std::cout << flag << std::endl;
		if (flag == "connect") {
			std::cout << "connected" << std::endl;
		}

#endif

	}


	void GameState::handleInput() {
		sf::Event event;
		while (data->window.pollEvent(event)) {

			if (sf::Event::Closed == event.type) {
				data->window.close();
			}

			if (!player.dying) {
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::RControl) {
						bombs.emplace_back(new Bomb(data, sf::Vector2f((sf::Vector2i(((player.getPosition() + sf::Vector2f(player.getSprite().getGlobalBounds().width / 2, player.getSprite().getGlobalBounds().height / 2 + 5.0f)) / (float)TILESIZE)) * TILESIZE)) + sf::Vector2f(5.0f, 5.0f)));
					}
				}
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			data->window.close();
		}
		
		if (!player.dying) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				player.move(Dir::Up);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				player.move(Dir::Down);
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				player.move(Dir::Right);
			} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
				player.move(Dir::Left);
			}
		}

	}

	void GameState::update() {

		//Networking////////////////////////////////////////////////////////////////////////////

		// on sending side
		float posXOut = player.getPosition().x;
		float posYOut = player.getPosition().y;
		sf::Packet packetOut;
		packetOut << posXOut << posYOut;
		if (socket.send(packetOut, ip, port) != sf::Socket::Done)
		{
			std::cout << "udp socket send error" << std::endl;
		}


		sf::Packet packetIn;
		if (socket.receive(packetIn, ip, port) != sf::Socket::Done)
		{
			std::cout << "udp socket receive error" << std::endl;
		}
		std::cout << ip << " on port " << port << std::endl;

		// on receiving side
		float posXIn;
		float posYIn;

		packetIn >> posXIn >> posYIn;

		std::cout << posXIn << " " << posYIn << std::endl;

		player2.setPosition(posXIn, posYIn);



		//Update player
		player.update();

		if (player.dead) {
			gameOver = true;
		}

		//Player Wall Collision
		for (int i = 0; i < GAMEFIELD_HEIGHT; i++) {
			for (int j = 0; j < GAMEFIELD_WIDTH; j++) {
				if (gameField[i][j] != 0) {
					blockTile.setPosition(j * TILESIZE, i * TILESIZE);
					if (Collision::bomberManWallCollision(blockTile, player.getSprite(), 0.5f, 0.05f, 0.1f, 0.1f)) {
						player.goToPreviousPosition();
					}
				}
			}
		}

		//Bombs
		for (int i = bombs.size() - 1; i >= 0 ; i--) {
			bombs[i]->update();

			if (bombs[i]->shouldExplode) {
				bombs[i]->explode(gameField, brickTiles, 5);
			}

			if (bombs[i]->hits(player)) {
				player.kill();
			}

			if (bombs[i]->explosionTimer <= 0) {
				bombs[i] = bombs[bombs.size() - 1];
				bombs.pop_back();
			}

		}

		//BrickTiles
		for (int i = brickTiles.size() - 1; i >= 0 ; i--) {
			if (brickTiles[i]->burned()) {
				brickTiles[i] = brickTiles[brickTiles.size() - 1];
				brickTiles.pop_back();
			}
		}

		//std::cout << gameOver << std::endl;

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

		for (int i = 0; i < brickTiles.size(); i++) {
			brickTiles[i]->draw();
		}

		for (int i = 0; i < bombs.size(); i++) {
			bombs[i]->draw();
		}

		player.draw();

		player2.draw();

		if (gameOver) {
			data->window.draw(gameOverText);
		}

		data->window.display();
	
	}


}