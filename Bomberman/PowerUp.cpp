#include "PowerUp.h"
#include "Collision.hpp"

namespace Game {

	PowerUp::PowerUp(gameDataRef data, PowerUpName powerUp, sf::Vector2f position) 
		: data(data),
		  powerUpSprite(data->assets.getTexture("PowerUps")),
		  powerUp(powerUp),
		  burnAnimation(powerUpSprite, burnAnimationFrames, 7, BURN_ANIMATION_TIME)
	{
		if (PowerUpName::Fire == powerUp) {
			powerUpSprite.setTextureRect(POWER_UP_FIRE);
		} else if (PowerUpName::Bomb == powerUp) {
			powerUpSprite.setTextureRect(POWER_UP_BOMB);
		} else if (PowerUpName::Speed == powerUp) {
			powerUpSprite.setTextureRect(POWER_UP_SPEED);
		} else if (PowerUpName::Throw == powerUp) {
			powerUpSprite.setTextureRect(POWER_UP_THROW);
		}

		powerUpSprite.setPosition(position);
		powerUpSprite.setScale(3.0f, 3.0f);
	}


	bool PowerUp::collides(Player& player) {
		if (!burning) {
			if (Collision::bomberManCollision(powerUpSprite, player.getSprite(), 0.5f, 0.05f, 0.1f, 0.1f)) {
			
				if (PowerUpName::Fire == powerUp){
					player.blastRadius += 1;
				} else if(PowerUpName::Bomb == powerUp) {
					player.bombLimit += 1;
				} else if(PowerUpName::Speed == powerUp) {
					player.speed += 0.5f;
				} else if(PowerUpName::Throw == powerUp) {
				
				} 

				return true;
			}
		}

		return false;
	}

	void PowerUp::burn() {
		powerUpSprite.setTexture(data->assets.getTexture("Tiles"));
		burnTimer.restart();
		burning = true;
	}

	bool PowerUp::burned() {
		return (burning && burnTimer.getElapsedTime().asSeconds() >= burnTime);
	}

	void PowerUp::draw() {
		if (burning) {
			burnAnimation.animation();
		}
		data->window.draw(powerUpSprite);
	}

}
