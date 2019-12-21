#include "Player.h"
#include <iostream>

namespace Game {

	Player::Player(gameDataRef data, sf::Vector2f position, int playerNumber)
		: data(data),
		  player(data->assets.getTexture("Player"), BOMBERMAN_1_WALK_DOWN_2),
		  walkingDownAnimation(player, walkingDownAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME),
		  walkingUpAnimation(player, walkingUpAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME),
		  walkingRightAnimation(player, walkingRightAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME),
		  walkingLeftAnimation(player, walkingLeftAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME),
		  dyingAnimation(player, dyingAnimationFrames, 6, DYING_ANIMATION_TIME)
	{
		player.setScale(2.5f, 2.5f);
		player.setPosition(position);

		if (playerNumber == 1) {

			walkingDownAnimationFrames[0] = BOMBERMAN_1_WALK_DOWN_2;
			walkingDownAnimationFrames[1] = BOMBERMAN_1_WALK_DOWN_1;
			walkingDownAnimationFrames[2] = BOMBERMAN_1_WALK_DOWN_2;
			walkingDownAnimationFrames[3] = BOMBERMAN_1_WALK_DOWN_3;

			walkingUpAnimationFrames[0] = BOMBERMAN_1_WALK_UP_2;
			walkingUpAnimationFrames[1] = BOMBERMAN_1_WALK_UP_1;
			walkingUpAnimationFrames[2] = BOMBERMAN_1_WALK_UP_2;
			walkingUpAnimationFrames[3] = BOMBERMAN_1_WALK_UP_3;

			walkingRightAnimationFrames[0] = BOMBERMAN_1_WALK_RIGHT_1;
			walkingRightAnimationFrames[1] = BOMBERMAN_1_WALK_RIGHT_2;
			walkingRightAnimationFrames[2] = BOMBERMAN_1_WALK_RIGHT_1;
			walkingRightAnimationFrames[3] = BOMBERMAN_1_WALK_RIGHT_3;

			walkingLeftAnimationFrames[0] = BOMBERMAN_1_WALK_LEFT_1;
			walkingLeftAnimationFrames[1] = BOMBERMAN_1_WALK_LEFT_2;
			walkingLeftAnimationFrames[2] = BOMBERMAN_1_WALK_LEFT_1;
			walkingLeftAnimationFrames[3] = BOMBERMAN_1_WALK_LEFT_3;

			dyingAnimationFrames[0] = BOMBERMAN_1_DEAD_1;
			dyingAnimationFrames[1] = BOMBERMAN_1_DEAD_2;
			dyingAnimationFrames[2] = BOMBERMAN_1_DEAD_3;
			dyingAnimationFrames[3] = BOMBERMAN_1_DEAD_4;
			dyingAnimationFrames[4] = BOMBERMAN_1_DEAD_5;
			dyingAnimationFrames[5] = BOMBERMAN_1_DEAD_6;

			punchUpFrame = BOMBERMAN_1_PUNCH_UP;
			punchDownFrame = BOMBERMAN_1_PUNCH_DOWN;
			punchRightFrame = BOMBERMAN_1_PUNCH_RIGHT;
			punchLeftFrame = BOMBERMAN_1_PUNCH_LEFT;

		} else if (playerNumber == 2) {

			walkingDownAnimationFrames[0] = BOMBERMAN_2_WALK_DOWN_2;
			walkingDownAnimationFrames[1] = BOMBERMAN_2_WALK_DOWN_1;
			walkingDownAnimationFrames[2] = BOMBERMAN_2_WALK_DOWN_2;
			walkingDownAnimationFrames[3] = BOMBERMAN_2_WALK_DOWN_3;

			walkingUpAnimationFrames[0] = BOMBERMAN_2_WALK_UP_2;
			walkingUpAnimationFrames[1] = BOMBERMAN_2_WALK_UP_1;
			walkingUpAnimationFrames[2] = BOMBERMAN_2_WALK_UP_2;
			walkingUpAnimationFrames[3] = BOMBERMAN_2_WALK_UP_3;

			walkingRightAnimationFrames[0] = BOMBERMAN_2_WALK_RIGHT_1;
			walkingRightAnimationFrames[1] = BOMBERMAN_2_WALK_RIGHT_2;
			walkingRightAnimationFrames[2] = BOMBERMAN_2_WALK_RIGHT_1;
			walkingRightAnimationFrames[3] = BOMBERMAN_2_WALK_RIGHT_3;

			walkingLeftAnimationFrames[0] = BOMBERMAN_2_WALK_LEFT_1;
			walkingLeftAnimationFrames[1] = BOMBERMAN_2_WALK_LEFT_2;
			walkingLeftAnimationFrames[2] = BOMBERMAN_2_WALK_LEFT_1;
			walkingLeftAnimationFrames[3] = BOMBERMAN_2_WALK_LEFT_3;

			dyingAnimationFrames[0] = BOMBERMAN_2_DEAD_1;
			dyingAnimationFrames[1] = BOMBERMAN_2_DEAD_2;
			dyingAnimationFrames[2] = BOMBERMAN_2_DEAD_3;
			dyingAnimationFrames[3] = BOMBERMAN_2_DEAD_4;
			dyingAnimationFrames[4] = BOMBERMAN_2_DEAD_5;
			dyingAnimationFrames[5] = BOMBERMAN_2_DEAD_6;

			punchUpFrame = BOMBERMAN_2_PUNCH_UP;
			punchDownFrame = BOMBERMAN_2_PUNCH_DOWN;
			punchRightFrame = BOMBERMAN_2_PUNCH_RIGHT;
			punchLeftFrame = BOMBERMAN_2_PUNCH_LEFT;

		}
	}


	void Player::move(Dir _dir) {
		previousPosition = player.getPosition();
		moving = true;
		dir = _dir;
		if (Dir::Up == dir) {
			player.move(0, -speed);
		} else if (Dir::Down == dir) {
			player.move(0, speed);
		} else if (Dir::Right == dir) {
			player.move(speed, 0);
		} else if (Dir::Left == dir) {
			player.move(-speed, 0);
		}
	}

	void Player::goToPreviousPosition() {
		if (!dying) {
			player.setPosition(previousPosition);
		}
	}


	void Player::update() {

		if (dying) {
			if (dyingTimer < dyingTime / 6) {
				player.move(0, -8);
			} else if (dyingTimer < dyingTime / 3) {
				player.move(0, 8);
			}
			dyingTimer++;
		}
		if (dyingTimer >= dyingTime) {
			dead = true;
		}

		if (moving) {
			moved = true;
		} else {
			moved = false;
		}
		moving = false;

		punchTimer++;

	}

	void Player::punch() {
		punchTimer = 0;
	}


	void Player::draw() {
		if (!dead) {

			if (dying) {
			
				dyingAnimation.animation();

			} else {

				if (punchTimer <= punchTime) {
					if (Dir::Up == dir) {
						player.setTextureRect(punchUpFrame);
					} else if (Dir::Down == dir) {
						player.setTextureRect(punchDownFrame);
					} else if (Dir::Right == dir) {
						player.setTextureRect(punchRightFrame);
					} else if (Dir::Left == dir) {
						player.setTextureRect(punchLeftFrame);
					}
				} else if (moved) {
					if (Dir::Up == dir) {
						walkingUpAnimation.animate();
					} else if (Dir::Down == dir) {
						walkingDownAnimation.animate();
					} else if (Dir::Right == dir) {
						walkingRightAnimation.animate();
					} else if (Dir::Left == dir) {
						walkingLeftAnimation.animate();
					}
				} else {
					if (Dir::Up == dir) {
						player.setTextureRect(walkingUpAnimationFrames[0]);
					} else if (Dir::Down == dir) {
						player.setTextureRect(walkingDownAnimationFrames[0]);
					} else if (Dir::Right == dir) {
						player.setTextureRect(walkingRightAnimationFrames[0]);
					} else if (Dir::Left == dir) {
						player.setTextureRect(walkingLeftAnimationFrames[0]);
					}
				}
			}
		}

		data->window.draw(player);
	}


	void Player::reset() {
		dead = false;
		dying = false;
		blastRadius = 2;
		speed = 3.0f;
		bombLimit = 1;
		bombCount = 0;
		punchPowerUp = false;
		kickPowerUp = false;
		dir = Dir::Down;
		previousPosition = sf::Vector2f(0.0f, 0.0f);
		moving = false;
		moved = false;
		dyingTimer = 0;
		dyingAnimation.reset();
	}

}