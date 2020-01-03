#include "Bomb.h"
#include "Collision.hpp"
#include "Utility.hpp"

#include <iostream>

namespace Game {

	Bomb::Bomb(gameDataRef data, Player& player, sf::Vector2i normalizedPos, int blastRadius) 
		: normalizedPos(normalizedPos),
		  data(data),
		  player(player),
		  bomb(data->assets.getTexture("Tiles"), BOMB_2),
		  bombAnimation(bomb, bombAnimationFrames, 3, 1.0f),
		  bombFlyAnimation(bomb, bombFlyAnimationFrames, 3, 1.0f),
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
		  explosionHorizontalPartAnimation(explosionHorizontalPart, explosionHorizontalPartAnimationFrames, 5, 0.5f),
		  blastRadius(blastRadius),
		  startPosition(sf::Vector2f((normalizedPos * TILESIZE) + sf::Vector2i(5, 5)))
	{	
		bomb.setScale(2.5f, 2.5f);
		bomb.setPosition(sf::Vector2f((normalizedPos * TILESIZE) + sf::Vector2i(5, 5)));

		explosionMiddle.setScale(3.125f, 3.125f);
		explosionUp.setScale(3.125f, 3.125f);
		explosionDown.setScale(3.125f, 3.125f);
		explosionRight.setScale(3.125f, 3.125f);
		explosionLeft.setScale(3.125f, 3.125f);
		explosionVerticalPart.setScale(3.125f, 3.125f);
		explosionHorizontalPart.setScale(3.125f, 3.125f);
	}

	
	void Bomb::update(std::vector<PowerUp*>& powerUps) {
		lifeTime--;
		if (lifeTime <= 0 && !exploded) {
			shouldExplode = true;
		}

		if (exploded) {
			explosionTimer--;
		}

		normalizedPos = sf::Vector2i(bomb.getPosition() / (float)TILESIZE);

		if (moving) {
			moveAnimationPercentage += moveAnimationPercentageStep;
			if (moveAnimationPercentage >= 1.0f) {
				bomb.setPosition(sf::Vector2f((newPosition * TILESIZE) + sf::Vector2i(5, 5)));
				startPosition = bomb.getPosition();
				moving = false;
				fly = false;
				moveAnimationPercentage = 0.0f;
				for (int i = 0; i < powerUps.size(); i++) {
					if (sf::Vector2i(powerUps[i]->getPosition().x / TILESIZE, powerUps[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y)) {
						onTopOfPowerUp = true;
						break;
					}
				}
			} else {
				bomb.setPosition(utility::lerp(moveAnimationPercentage, startPosition, sf::Vector2f((newPosition * TILESIZE) + sf::Vector2i(5, 5))));
			}
		}

	}

	void Bomb::move(sf::Vector2i position) {

		moving = true;
		newPosition = position;

		onTopOfPowerUp = false;

		sf::Vector2f distance = sf::Vector2f((newPosition * TILESIZE) + sf::Vector2i(5, 5)) - bomb.getPosition();

		if (distance.x == 0) {
			moveAnimationPercentageStep = 1.0f / (abs(distance.y) / bombMoveSpeed);
		} else {
			moveAnimationPercentageStep = 1.0f / (abs(distance.x) / bombMoveSpeed);
		}


	}

	void Bomb::explode(int gameField[GAMEFIELD_WIDTH][GAMEFIELD_HEIGHT], std::vector<BrickTile*>& brickTiles, std::vector<PowerUp*>& powerUps, std::vector<Bomb*>& bombs) {

		if (gameField[normalizedPos.x][normalizedPos.y] == 0) {

			player.bombCount--;


			if (moving) {
				bomb.setPosition(sf::Vector2f(normalizedPos * TILESIZE));
			}

			//If bomb lands on powerUp burn powerUp
			for (int i = 0; i < powerUps.size(); i++) {
				if (sf::Vector2i(powerUps[i]->getPosition().x / TILESIZE, powerUps[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y)) {
					powerUps[i]->burn();
					break;
				}
			}
			//If bomb lands on bomb
			for (int i = 0; i < bombs.size(); i++) {
				if (sf::Vector2i(bombs[i]->getPosition().x / TILESIZE, bombs[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y)) {
					bombs[i]->lifeTime = 0;
				}
			}

			bool upHit = false;
			bool DownHit = false;
			bool RightHit = false;
			bool LeftHit = false;

			explosionAnimationFrameList.clear();
			explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedPos.x * TILESIZE, normalizedPos.y * TILESIZE) , &explosionMiddle });

			for (int length = 1; length <= blastRadius; length++) {

				if (!upHit) {
					for (int i = 0; i < bombs.size(); i++) {
						if (sf::Vector2i(bombs[i]->getPosition().x / TILESIZE, bombs[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y - length)) {
							if (!bombs[i]->hitByExplosion) {
								bombs[i]->hitByExplosion = true;
								bombs[i]->lifeTime = explosionDelayAfterHitByExplosion;
							}
							upHit = true;
							break;
						}
					}
				}
				if (!upHit) {
					for (int i = 0; i < powerUps.size(); i++) {
						if (sf::Vector2i(powerUps[i]->getPosition().x / TILESIZE, powerUps[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y - length)) {
							powerUps[i]->burn();
							upHit = true;
							break;
						}
					}
				}
				if (!upHit) {
					if (gameField[normalizedPos.x][normalizedPos.y - length] != 0) {
						upHit = true;
						if (gameField[normalizedPos.x][normalizedPos.y - length] == 1) {
							gameField[normalizedPos.x][normalizedPos.y - length] = 0;
							for (int i = 0; i < brickTiles.size(); i++) {
								if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y - length)) {
									brickTiles[i]->burn();
								}
							}
						}
					} else {
						if (length == blastRadius) {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedPos.x * TILESIZE, (normalizedPos.y - length) * TILESIZE) , &explosionUp } );
						} else {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedPos.x * TILESIZE, (normalizedPos.y - length) * TILESIZE) , &explosionVerticalPart } );
						}
					}
				}

				if (!DownHit) {
					for (int i = 0; i < bombs.size(); i++) {
						if (sf::Vector2i(bombs[i]->getPosition().x / TILESIZE, bombs[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y + length)) {
							if (!bombs[i]->hitByExplosion) {
								bombs[i]->hitByExplosion = true;
								bombs[i]->lifeTime = explosionDelayAfterHitByExplosion;
							}
							DownHit = true;
							break;
						}
					}
				}
				if (!DownHit) {
					for (int i = 0; i < powerUps.size(); i++) {
						if (sf::Vector2i(powerUps[i]->getPosition().x / TILESIZE, powerUps[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y + length)) {
							powerUps[i]->burn();
							DownHit = true;
							break;
						}
					}
				}
				if (!DownHit) {
					if (gameField[normalizedPos.x][normalizedPos.y + length] != 0) {
						DownHit = true;
						if (gameField[normalizedPos.x][normalizedPos.y + length] == 1) {
							gameField[normalizedPos.x][normalizedPos.y + length] = 0;
							for (int i = 0; i < brickTiles.size(); i++) {
								if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x, normalizedPos.y + length)) {
									brickTiles[i]->burn();
								}
							}
						}
					} else {
						if (length == blastRadius) {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedPos.x * TILESIZE, (normalizedPos.y + length) * TILESIZE) , &explosionDown } );
						} else {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f(normalizedPos.x * TILESIZE, (normalizedPos.y + length) * TILESIZE) , &explosionVerticalPart } );
						}
					}
				}

				if (!RightHit) {
					for (int i = 0; i < bombs.size(); i++) {
						if (sf::Vector2i(bombs[i]->getPosition().x / TILESIZE, bombs[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x + length, normalizedPos.y)) {
							if (!bombs[i]->hitByExplosion) {
								bombs[i]->hitByExplosion = true;
								bombs[i]->lifeTime = explosionDelayAfterHitByExplosion;
							}
							RightHit = true;
							break;
						}
					}
				}
				if (!RightHit) {
					for (int i = 0; i < powerUps.size(); i++) {
						if (sf::Vector2i(powerUps[i]->getPosition().x / TILESIZE, powerUps[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x + length, normalizedPos.y)) {
							powerUps[i]->burn();
							RightHit = true;
							break;
						}
					}
				}
				if (!RightHit) { 
					if (gameField[normalizedPos.x + length][normalizedPos.y] != 0) {
						RightHit = true;
						if (gameField[normalizedPos.x + length][normalizedPos.y] == 1) {
							gameField[normalizedPos.x + length][normalizedPos.y] = 0;
							for (int i = 0; i < brickTiles.size(); i++) {
								if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x + length, normalizedPos.y)) {
									brickTiles[i]->burn();
								}
							}
						}
					} else {
						if (length == blastRadius) {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedPos.x + length) * TILESIZE, normalizedPos.y * TILESIZE) , &explosionRight } );
						} else {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedPos.x + length) * TILESIZE, normalizedPos.y * TILESIZE) , &explosionHorizontalPart } );
						}
					}
				}

				if (!LeftHit) {
					for (int i = 0; i < bombs.size(); i++) {
						if (sf::Vector2i(bombs[i]->getPosition().x / TILESIZE, bombs[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x - length, normalizedPos.y)) {
							if (!bombs[i]->hitByExplosion) {
								bombs[i]->hitByExplosion = true;
								bombs[i]->lifeTime = explosionDelayAfterHitByExplosion;
							}
							LeftHit = true;
							break;
						}
					}
				}
				if (!LeftHit) {
					for (int i = 0; i < powerUps.size(); i++) {
						if (sf::Vector2i(powerUps[i]->getPosition().x / TILESIZE, powerUps[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x - length, normalizedPos.y)) {
							powerUps[i]->burn();
							LeftHit = true;
							break;
						}
					}
				}
				if (!LeftHit) {
					if (gameField[normalizedPos.x - length][normalizedPos.y] != 0) {
						LeftHit = true;
						if (gameField[normalizedPos.x - length][normalizedPos.y] == 1) {
							gameField[normalizedPos.x - length][normalizedPos.y] = 0;
							for (int i = 0; i < brickTiles.size(); i++) {
								if (sf::Vector2i(brickTiles[i]->getPosition().x / TILESIZE, brickTiles[i]->getPosition().y / TILESIZE) == sf::Vector2i(normalizedPos.x - length, normalizedPos.y)) {
									brickTiles[i]->burn();
								}
							}
						}
					} else {
						if (length == blastRadius) {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedPos.x - length) * TILESIZE, normalizedPos.y * TILESIZE) , &explosionLeft } );
						} else {
							explosionAnimationFrameList.emplace_back(new ExplosionAnimationFrame { sf::Vector2f((normalizedPos.x - length) * TILESIZE, normalizedPos.y * TILESIZE) , &explosionHorizontalPart } );
						}
					}
				}

			}

		
			exploded = true;
			shouldExplode = false;

		}

	}


	bool Bomb::hits(Player& player) {
		for (int i = 0; i < explosionAnimationFrameList.size(); i++) {
			explosionAnimationFrameList[i]->sprite->setPosition(explosionAnimationFrameList[i]->position);
			if (Collision::checkSpriteCollision(*explosionAnimationFrameList[i]->sprite, 0.6f, player.getSprite(), 0.6f)) {
				return true;
			}
		}
		return false;
	}

	bool Bomb::outOfScreen() {
		if (bomb.getPosition().x > SCREEN_WIDTH || bomb.getPosition().x + bomb.getGlobalBounds().width < 0 || bomb.getPosition().y > SCREEN_HEIGHT || bomb.getPosition().y + bomb.getGlobalBounds().height < 0) {
			player.bombCount--;
			return true;
		} else {
			return false;
		}
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

			if (fly || onTopOfPowerUp) {
				bombFlyAnimation.animation();
			} else {
				bombAnimation.animation();
			}

			data->window.draw(bomb);

		}
	}


}