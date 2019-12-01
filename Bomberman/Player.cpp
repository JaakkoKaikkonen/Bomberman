#include "Player.h"
#include <iostream>

namespace Game {

	Player::Player(gameDataRef data)
		: data(data),
		  player(data->assets.getTexture("Player"), BOMBERMAN_1_WALK_DOWN_2),
		  walkingDownAnimation(player, walkingDownAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME),
		  walkingUpAnimation(player, walkingUpAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME),
		  walkingRightAnimation(player, walkingRightAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME),
		  walkingLeftAnimation(player, walkingLeftAnimationFrames, 4, BOMBERMAN_WALK_ANIMATION_TIME)
	{
		player.setScale(2.5f, 2.5f);
		player.setPosition(50, 50);
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
		player.setPosition(previousPosition);
	}

	void Player::update() {

		if (moving) {
			moved = true;
		} else {
			moved = false;
		}
		moving = false;

	}

	void Player::draw() {
		if (moved) {
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
				player.setTextureRect(BOMBERMAN_1_WALK_UP_2);
			} else if (Dir::Down == dir) {
				player.setTextureRect(BOMBERMAN_1_WALK_DOWN_2);
			} else if (Dir::Right == dir) {
				player.setTextureRect(BOMBERMAN_1_WALK_RIGHT_1);
			} else if (Dir::Left == dir) {
				player.setTextureRect(BOMBERMAN_1_WALK_LEFT_1);
			}
			
		}
		data->window.draw(player);
	}

}