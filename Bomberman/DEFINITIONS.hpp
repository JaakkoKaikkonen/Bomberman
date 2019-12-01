#pragma once

#define WINDOW_TITLE "Bomberman"

#define SCREEN_WIDTH 750
#define SCREEN_HEIGHT 650
		
#define TILESIZE 50

#define GAMEFIELD_WIDTH (SCREEN_WIDTH / TILESIZE)
#define GAMEFIELD_HEIGHT (SCREEN_HEIGHT / TILESIZE)

#define TOP_ICON_FILEPATH "Resources/Images/bomb_icon.png"

#define BOMB_SPRITES_FILEPATH "Resources/Images/bomberman_tiles.png"

#define PLAYER_SPRITES_FILEPATH "Resources/Images/bombermanSprites.png"


enum class Dir { Up, Down, Right, Left };

#define BLOCK_TILE sf::IntRect(18, 0, 16, 16)
#define BRICK_TILE sf::IntRect(35, 0, 16, 16)
#define EMPTY_TILE sf::IntRect(52, 0, 16, 16)
#define EMPTY_BELOW_TILE sf::IntRect(69, 0, 16, 16)

#define BOMB_1 sf::IntRect(103, 102, 16, 16)
#define BOMB_2 sf::IntRect(120, 102, 16, 16)
#define BOMB_3 sf::IntRect(137, 102, 16, 16)

#define EXPLOSION_MIDDLE_1 sf::IntRect(69, 68, 16, 16)
#define EXPLOSION_MIDDLE_2 sf::IntRect(120, 51, 16, 16)
#define EXPLOSION_MIDDLE_3 sf::IntRect(103, 51, 16, 16)
#define EXPLOSION_MIDDLE_4 sf::IntRect(86, 51, 16, 16)
#define EXPLOSION_MIDDLE_5 sf::IntRect(69, 51, 16, 16)

#define EXPLOSION_UP_1 sf::IntRect(69, 34, 16, 16)
#define EXPLOSION_UP_2 sf::IntRect(86, 34, 16, 16)
#define EXPLOSION_UP_3 sf::IntRect(103, 34, 16, 16)
#define EXPLOSION_UP_4 sf::IntRect(120, 34, 16, 16)
#define EXPLOSION_UP_5 sf::IntRect(120, 17, 16, 16)

#define EXPLOSION_DOWN_1 sf::IntRect(52, 34, 16, 16)
#define EXPLOSION_DOWN_2 sf::IntRect(69, 85, 16, 16)
#define EXPLOSION_DOWN_3 sf::IntRect(120, 85, 16, 16)
#define EXPLOSION_DOWN_4 sf::IntRect(103, 85, 16, 16)
#define EXPLOSION_DOWN_5 sf::IntRect(86, 85, 16, 16)

#define EXPLOSION_RIGHT_1 sf::IntRect(52, 51, 16, 16)
#define EXPLOSION_RIGHT_2 sf::IntRect(52, 68, 16, 16)
#define EXPLOSION_RIGHT_3 sf::IntRect(18, 85, 16, 16)
#define EXPLOSION_RIGHT_4 sf::IntRect(18, 68, 16, 16)
#define EXPLOSION_RIGHT_5 sf::IntRect(18, 51, 16, 16)

#define EXPLOSION_LEFT_1 sf::IntRect(1, 17, 16, 16)
#define EXPLOSION_LEFT_2 sf::IntRect(18, 17, 16, 16)
#define EXPLOSION_LEFT_3 sf::IntRect(35, 17, 16, 16)
#define EXPLOSION_LEFT_4 sf::IntRect(52, 17, 16, 16)
#define EXPLOSION_LEFT_5 sf::IntRect(69, 17, 16, 16)

#define EXPLOSION_VERTICAL_PART_1 sf::IntRect(52, 85, 16, 16)
#define EXPLOSION_VERTICAL_PART_2 sf::IntRect(35, 85, 16, 16)
#define EXPLOSION_VERTICAL_PART_3 sf::IntRect(120, 68, 16, 16)
#define EXPLOSION_VERTICAL_PART_4 sf::IntRect(103, 68, 16, 16)
#define EXPLOSION_VERTICAL_PART_5 sf::IntRect(86, 68, 16, 16)

#define EXPLOSION_HORIZONTAL_PART_1 sf::IntRect(35, 68, 16, 16)
#define EXPLOSION_HORIZONTAL_PART_2 sf::IntRect(35, 51, 16, 16)
#define EXPLOSION_HORIZONTAL_PART_3 sf::IntRect(1, 85, 16, 16)
#define EXPLOSION_HORIZONTAL_PART_4 sf::IntRect(1, 68, 16, 16)
#define EXPLOSION_HORIZONTAL_PART_5 sf::IntRect(1, 51, 16, 16)


#define BURNING_BRICK_TILE_1 sf::IntRect(1, 102, 16, 16)
#define BURNING_BRICK_TILE_2 sf::IntRect(18, 102, 16, 16)
#define BURNING_BRICK_TILE_3 sf::IntRect(35, 102, 16, 16)
#define BURNING_BRICK_TILE_4 sf::IntRect(52, 102, 16, 16)
#define BURNING_BRICK_TILE_5 sf::IntRect(69, 102, 16, 16)
#define BURNING_BRICK_TILE_6 sf::IntRect(86, 102, 16, 16)


#define BOMBERMAN_WALK_ANIMATION_TIME 1.0f

#define BOMBERMAN_1_WALK_DOWN_1 sf::IntRect(56, 28, 15, 24)
#define BOMBERMAN_1_WALK_DOWN_2 sf::IntRect(72, 28, 15, 24)
#define BOMBERMAN_1_WALK_DOWN_3 sf::IntRect(88, 28, 15, 24)

#define BOMBERMAN_1_WALK_UP_1 sf::IntRect(57, 2, 15, 24)
#define BOMBERMAN_1_WALK_UP_2 sf::IntRect(73, 2, 15, 24)
#define BOMBERMAN_1_WALK_UP_3 sf::IntRect(89, 2, 15, 24)

#define BOMBERMAN_1_WALK_RIGHT_1 sf::IntRect(106, 29, 16, 24)
#define BOMBERMAN_1_WALK_RIGHT_2 sf::IntRect(122, 29, 16, 24)
#define BOMBERMAN_1_WALK_RIGHT_3 sf::IntRect(138, 29, 16, 24)

#define BOMBERMAN_1_WALK_LEFT_1 sf::IntRect(3, 26, 16, 24)
#define BOMBERMAN_1_WALK_LEFT_2 sf::IntRect(19, 26, 16, 24)
#define BOMBERMAN_1_WALK_LEFT_3 sf::IntRect(35, 26, 16, 24)