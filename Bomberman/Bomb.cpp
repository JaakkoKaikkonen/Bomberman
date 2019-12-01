#include "Bomb.h"

#include <iostream>

namespace Game {

	Bomb::Bomb(gameDataRef data, sf::Vector2f position) 
		: data(data),
		  bomb(data->assets.getTexture("Tiles"), BOMB_2),
		  explosionMiddle(data->assets.getTexture("Tiles"), EXPLOSION_MIDDLE_1),
		  explosionMiddleAnimation(explosionMiddle, explosionMiddleAnimationFrames, 5, 0.5f),
		  explosionUp(data->assets.getTexture("Tiles"), EXPLOSION_UP_1),
		  explosionUpAnimation(explosionUp, explosionUpAnimationFrames, 5, 0.5f),
		  explosionDown(data->assets.getTexture("Tiles"), EXPLOSION_DOWN_1),
		  explosionDownAnimation(explosionDown, explosionDownAnimationFrames, 5, 0.5f),
		  explosionRight(data->assets.getTexture("Tiles"), EXPLOSION_RIGHT_1),
		  explosionRightAnimation(explosionRight, explosionRightAnimationFrames, 5, 0.5f),
		  explosionLeft(data->assets.getTexture("Tiles"), EXPLOSION_LEFT_1),
		  explosionLeftAnimation(explosionLeft, explosionLeftAnimationFrames, 5, 0.5f),
		  bombAnimation(bomb, bombAnimationFrames, 3, 1.0f)
	{	
		bomb.setScale(2.5f, 2.5f);
		bomb.setPosition(position);

		explosionMiddle.setScale(3.125f, 3.125f);
		explosionUp.setScale(3.125f, 3.125f);
		explosionDown.setScale(3.125f, 3.125f);
		explosionRight.setScale(3.125f, 3.125f);
		explosionLeft.setScale(3.125f, 3.125f);

		explosionMiddle.setPosition(bomb.getPosition());
		explosionUp.setPosition(bomb.getPosition().x, bomb.getPosition().y - TILESIZE);
		explosionDown.setPosition(bomb.getPosition().x, bomb.getPosition().y + TILESIZE);
		explosionRight.setPosition(bomb.getPosition().x + TILESIZE, bomb.getPosition().y);
		explosionLeft.setPosition(bomb.getPosition().x - TILESIZE, bomb.getPosition().y);
	}

	
	void Bomb::update() {
		lifeTime--;
		if (lifeTime <= 0 && !exploded) {
			shouldExplode = true;
		}

		if (exploded) {
			explosionTimer--;
		}
	}

	void Bomb::explode(int gameField[GAMEFIELD_HEIGHT][GAMEFIELD_WIDTH], std::vector<BrickTile*> brickTiles) {
		
		std::cout << "Explode" << std::endl;

		sf::Vector2i normalizedBombPos = sf::Vector2i(bomb.getPosition() / (float)TILESIZE);

		if (gameField[normalizedBombPos.y - 1][normalizedBombPos.x] != 0) {
			explosionUp.setPosition(-100, -100);
			if (gameField[normalizedBombPos.y - 1][normalizedBombPos.x] == 1) {
				gameField[normalizedBombPos.y - 1][normalizedBombPos.x] = 0;
				for (int i = 0; i < brickTiles.size(); i++) {
					if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x, normalizedBombPos.y - 1)) {
						brickTiles[i]->burn();
					}
				}
			}
		}
		if (gameField[normalizedBombPos.y + 1][normalizedBombPos.x] != 0) {
			explosionDown.setPosition(-100, -100);
			if (gameField[normalizedBombPos.y + 1][normalizedBombPos.x] == 1) {
				gameField[normalizedBombPos.y + 1][normalizedBombPos.x] = 0;
				for (int i = 0; i < brickTiles.size(); i++) {
					if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x, normalizedBombPos.y + 1)) {
						brickTiles[i]->burn();
					}
				}
			}
		}
		if (gameField[normalizedBombPos.y][normalizedBombPos.x + 1] != 0) {
			explosionRight.setPosition(-100, -100);
			if (gameField[normalizedBombPos.y][normalizedBombPos.x + 1] == 1) {
				gameField[normalizedBombPos.y][normalizedBombPos.x + 1] = 0;
				for (int i = 0; i < brickTiles.size(); i++) {
					if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x + 1, normalizedBombPos.y)) {
						brickTiles[i]->burn();
					}
				}
			}
		}
		if (gameField[normalizedBombPos.y][normalizedBombPos.x - 1] != 0) {
			explosionLeft.setPosition(-100, -100);
			if (gameField[normalizedBombPos.y][normalizedBombPos.x - 1] == 1) {
				gameField[normalizedBombPos.y][normalizedBombPos.x - 1] = 0;
				for (int i = 0; i < brickTiles.size(); i++) {
					if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x - 1, normalizedBombPos.y)) {
						brickTiles[i]->burn();
					}
				}
			}
		}

		
		exploded = true;
		shouldExplode = false;

	}



	void Bomb::draw() {
		if (exploded) {
			explosionMiddleAnimation.animate();
			data->window.draw(explosionMiddle);
			explosionUpAnimation.animate();
			data->window.draw(explosionUp);
			explosionDownAnimation.animate();
			data->window.draw(explosionDown);
			explosionRightAnimation.animate();
			data->window.draw(explosionRight);
			explosionLeftAnimation.animate();
			data->window.draw(explosionLeft);
		} else {
			bombAnimation.animate();
			data->window.draw(bomb);
		}
	}

}