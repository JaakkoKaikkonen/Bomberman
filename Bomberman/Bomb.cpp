#include "Bomb.h"
#include "Collision.hpp"

#include <iostream>

namespace Game {

	Bomb::Bomb(gameDataRef data, sf::Vector2f position) 
		: data(data),
		  bomb(data->assets.getTexture("Tiles"), BOMB_2),
		  bombAnimation(bomb, bombAnimationFrames, 3, 1.0f),
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
		  explosionVerticalPart(data->assets.getTexture("Tiles"), EXPLOSION_VERTICAL_PART_1),
		  explosionVerticalPartAnimation(explosionVerticalPart, explosionVerticalPartAnimationFrames, 5, 0.5f),
		  explosionHorizontalPart(data->assets.getTexture("Tiles"), EXPLOSION_HORIZONTAL_PART_1),
		  explosionHorizontalPartAnimation(explosionHorizontalPart, explosionHorizontalPartAnimationFrames, 5, 0.5f)
	{	
		bomb.setScale(2.5f, 2.5f);
		bomb.setPosition(position);

		explosionMiddle.setScale(3.125f, 3.125f);
		explosionUp.setScale(3.125f, 3.125f);
		explosionDown.setScale(3.125f, 3.125f);
		explosionRight.setScale(3.125f, 3.125f);
		explosionLeft.setScale(3.125f, 3.125f);
		explosionVerticalPart.setScale(3.125f, 3.125f);
		explosionHorizontalPart.setScale(3.125f, 3.125f);
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

	void Bomb::explode(int gameField[GAMEFIELD_HEIGHT][GAMEFIELD_WIDTH], std::vector<BrickTile*> brickTiles, int range) {
		
		std::cout << "Explode" << std::endl;

		sf::Vector2i normalizedBombPos = sf::Vector2i(bomb.getPosition() / (float)TILESIZE);

		bool upHit = false;
		bool DownHit = false;
		bool RightHit = false;
		bool LeftHit = false;

		explosionAnimationFrameList.clear();
		explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedBombPos.x * TILESIZE, normalizedBombPos.y * TILESIZE) , &explosionMiddle });

		for (int length = 1; length <= range; length++) {

			if (!upHit) {
				if (gameField[normalizedBombPos.y - length][normalizedBombPos.x] != 0) {
					upHit = true;
					if (gameField[normalizedBombPos.y - length][normalizedBombPos.x] == 1) {
						gameField[normalizedBombPos.y - length][normalizedBombPos.x] = 0;
						for (int i = 0; i < brickTiles.size(); i++) {
							if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x, normalizedBombPos.y - length)) {
								brickTiles[i]->burn();
							}
						}
					}
				} else {
					if (length == range) {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedBombPos.x * TILESIZE, (normalizedBombPos.y - length) * TILESIZE) , &explosionUp } );
					} else {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedBombPos.x * TILESIZE, (normalizedBombPos.y - length) * TILESIZE) , &explosionVerticalPart } );
					}
				}
			}
			if (!DownHit) {
				if (gameField[normalizedBombPos.y + length][normalizedBombPos.x] != 0) {
					DownHit = true;
					if (gameField[normalizedBombPos.y + length][normalizedBombPos.x] == 1) {
						gameField[normalizedBombPos.y + length][normalizedBombPos.x] = 0;
						for (int i = 0; i < brickTiles.size(); i++) {
							if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x, normalizedBombPos.y + length)) {
								brickTiles[i]->burn();
							}
						}
					}
				} else {
					if (length == range) {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedBombPos.x * TILESIZE, (normalizedBombPos.y + length) * TILESIZE) , &explosionDown } );
					} else {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedBombPos.x * TILESIZE, (normalizedBombPos.y + length) * TILESIZE) , &explosionVerticalPart } );
					}
				}
			}
			if (!RightHit) { 
				if (gameField[normalizedBombPos.y][normalizedBombPos.x + length] != 0) {
					RightHit = true;
					if (gameField[normalizedBombPos.y][normalizedBombPos.x + length] == 1) {
						gameField[normalizedBombPos.y][normalizedBombPos.x + length] = 0;
						for (int i = 0; i < brickTiles.size(); i++) {
							if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x + length, normalizedBombPos.y)) {
								brickTiles[i]->burn();
							}
						}
					}
				} else {
					if (length == range) {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedBombPos.x + length) * TILESIZE, normalizedBombPos.y * TILESIZE) , &explosionRight } );
					} else {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedBombPos.x + length) * TILESIZE, normalizedBombPos.y * TILESIZE) , &explosionHorizontalPart } );
					}
				}
			}
			if (!LeftHit) {
				if (gameField[normalizedBombPos.y][normalizedBombPos.x - length] != 0) {
					LeftHit = true;
					if (gameField[normalizedBombPos.y][normalizedBombPos.x - length] == 1) {
						gameField[normalizedBombPos.y][normalizedBombPos.x - length] = 0;
						for (int i = 0; i < brickTiles.size(); i++) {
							if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedBombPos.x - length, normalizedBombPos.y)) {
								brickTiles[i]->burn();
							}
						}
					}
				} else {
					if (length == range) {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedBombPos.x - length) * TILESIZE, normalizedBombPos.y * TILESIZE) , &explosionLeft } );
					} else {
						explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedBombPos.x - length) * TILESIZE, normalizedBombPos.y * TILESIZE) , &explosionHorizontalPart } );
					}
				}
			}

		}

		
		exploded = true;
		shouldExplode = false;

	}


	bool Bomb::hits(Player player) {
		for (int i = 0; i < explosionAnimationFrameList.size(); i++) {
			explosionAnimationFrameList[i]->sprite->setPosition(explosionAnimationFrameList[i]->position);
			if (Collision::checkSpriteCollision(*explosionAnimationFrameList[i]->sprite, player.getSprite())) {
				return true;
			}
		}
		return false;
	}


	void Bomb::draw() {
		if (exploded) {

			explosionMiddleAnimation.animation();
			explosionUpAnimation.animation();
			explosionDownAnimation.animation();
			explosionRightAnimation.animation();
			explosionLeftAnimation.animation();
			explosionVerticalPartAnimation.animation();
			explosionHorizontalPartAnimation.animation();

			for (int i = 0; i < explosionAnimationFrameList.size(); i++) {
				explosionAnimationFrameList[i]->sprite->setPosition(explosionAnimationFrameList[i]->position);
				data->window.draw(*explosionAnimationFrameList[i]->sprite);
			}

		} else {
			bombAnimation.animation();
			data->window.draw(bomb);
		}
	}


}