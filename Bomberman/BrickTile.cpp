#include "BrickTile.h"

namespace Game {

	BrickTile::BrickTile(gameDataRef data, sf::Vector2f position) 
		: data(data),
		  brickTile(data->assets.getTexture("Tiles"), BRICK_TILE),
		  burningTileAnimation(brickTile, burningTileAnimationFrames, 6, 0.75f)
	{
		brickTile.setPosition(position);
		brickTile.setScale(3.125f, 3.125f);
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
			burningTileAnimation.animate();
		}
		
		data->window.draw(brickTile);
	
	}

}