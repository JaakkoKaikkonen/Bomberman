#include "BrickTile.h"
#include <iostream>

namespace Game {

	BrickTile::BrickTile(gameDataRef data, sf::Vector2f position) 
		: data(data),
		  brickTile(data->assets.getTexture("Tiles"), BRICK_TILE),
		  burningTileAnimation(brickTile, burningTileAnimationFrames, 6, 0.75f)
	{
		brickTile.setPosition(position);
		brickTile.setScale(3.125f, 3.125f);

		float randNum = (float)rand() / RAND_MAX;

		if (randNum > 0.9f) {
			powerUp = PowerUpName::Fire;
		} else if (randNum > 0.8f) {
			powerUp = PowerUpName::Bomb;
		} else if (randNum > 0.7f) {
			powerUp = PowerUpName::Speed;
		} else if (randNum > 0.6f) {
			powerUp = PowerUpName::Throw;
		}

	}


	void BrickTile::burn() {
		burning = true;
		burnTimer.restart();
	}

	bool BrickTile::burned() {
		return (burning && burnTimer.getElapsedTime().asSeconds() >= burnTime);
	}

	void BrickTile::draw() {
		if (burning) {
			burningTileAnimation.animation();
		}
		
		data->window.draw(brickTile);
	
	}

}