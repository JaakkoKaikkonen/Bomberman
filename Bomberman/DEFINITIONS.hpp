#pragma once

#define WINDOW_TITLE "Bomberman"

#define SCREEN_WIDTH 750
#define SCREEN_HEIGHT 650
		
#define TILESIZE 50

#define GAMEFIELD_WIDTH (SCREEN_WIDTH / TILESIZE)
#define GAMEFIELD_HEIGHT (SCREEN_HEIGHT / TILESIZE)

#define FONT_FILEPATH "Resources/Fonts/font.ttf"
#define TOP_ICON_FILEPATH "Resources/Images/bomb_icon.png"
#define BOMB_SPRITES_FILEPATH "Resources/Images/bomberman_tiles.png"
#define PLAYER_SPRITES_FILEPATH "Resources/Images/bombermanSprites.png"
#define POWER_UP_SPRITES_FILEPATH "Resources/Images/items.png"


enum class Dir { Up, Down, Right, Left };

enum class PowerUpName { None, Fire, Bomb, Speed, Punch, Kick };


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
#define DYING_ANIMATION_TIME 1.0f

#define BOMBERMAN_1_WALK_DOWN_1 sf::IntRect(56, 28, 16, 24)
#define BOMBERMAN_1_WALK_DOWN_2 sf::IntRect(72, 28, 16, 24)
#define BOMBERMAN_1_WALK_DOWN_3 sf::IntRect(88, 28, 16, 24)

#define BOMBERMAN_1_WALK_UP_1 sf::IntRect(57, 2, 16, 24)
#define BOMBERMAN_1_WALK_UP_2 sf::IntRect(73, 2, 16, 24)
#define BOMBERMAN_1_WALK_UP_3 sf::IntRect(89, 2, 16, 24)

#define BOMBERMAN_1_WALK_RIGHT_1 sf::IntRect(106, 29, 16, 24)
#define BOMBERMAN_1_WALK_RIGHT_2 sf::IntRect(122, 29, 16, 24)
#define BOMBERMAN_1_WALK_RIGHT_3 sf::IntRect(138, 29, 16, 24)

#define BOMBERMAN_1_WALK_LEFT_1 sf::IntRect(3, 26, 16, 24)
#define BOMBERMAN_1_WALK_LEFT_2 sf::IntRect(19, 26, 16, 24)
#define BOMBERMAN_1_WALK_LEFT_3 sf::IntRect(35, 26, 16, 24)

#define BOMBERMAN_1_PUNCH_DOWN sf::IntRect(51, 85, 16, 24)
#define BOMBERMAN_1_PUNCH_UP sf::IntRect(71, 85, 16, 25)
#define BOMBERMAN_1_PUNCH_RIGHT sf::IntRect(92, 85, 16, 24)
#define BOMBERMAN_1_PUNCH_LEFT sf::IntRect(112, 85, 16, 24)

#define BOMBERMAN_1_DEAD_1 sf::IntRect(30, 57, 16, 24)
#define BOMBERMAN_1_DEAD_2 sf::IntRect(49, 57, 16, 24)
#define BOMBERMAN_1_DEAD_3 sf::IntRect(66, 57, 16, 24)
#define BOMBERMAN_1_DEAD_4 sf::IntRect(83, 57, 16, 24)
#define BOMBERMAN_1_DEAD_5 sf::IntRect(100, 57, 16, 24)
#define BOMBERMAN_1_DEAD_6 sf::IntRect(118, 57, 16, 24)


#define BOMBERMAN_2_WALK_DOWN_1 sf::IntRect(211, 28, 16, 24)
#define BOMBERMAN_2_WALK_DOWN_2 sf::IntRect(227, 28, 16, 24)
#define BOMBERMAN_2_WALK_DOWN_3 sf::IntRect(243, 28, 16, 24)

#define BOMBERMAN_2_WALK_UP_1 sf::IntRect(212, 2, 16, 24)
#define BOMBERMAN_2_WALK_UP_2 sf::IntRect(228, 2, 16, 24)
#define BOMBERMAN_2_WALK_UP_3 sf::IntRect(244, 2, 16, 24)

#define BOMBERMAN_2_WALK_RIGHT_1 sf::IntRect(261, 29, 16, 24)
#define BOMBERMAN_2_WALK_RIGHT_2 sf::IntRect(277, 29, 16, 24)
#define BOMBERMAN_2_WALK_RIGHT_3 sf::IntRect(293, 29, 16, 24)

#define BOMBERMAN_2_WALK_LEFT_1 sf::IntRect(158, 26, 16, 24)
#define BOMBERMAN_2_WALK_LEFT_2 sf::IntRect(174, 26, 16, 24)
#define BOMBERMAN_2_WALK_LEFT_3 sf::IntRect(191, 26, 16, 24)

#define BOMBERMAN_2_PUNCH_DOWN sf::IntRect(206, 85, 16, 24)
#define BOMBERMAN_2_PUNCH_UP sf::IntRect(226, 85, 16, 25)
#define BOMBERMAN_2_PUNCH_RIGHT sf::IntRect(247, 85, 16, 24)
#define BOMBERMAN_2_PUNCH_LEFT sf::IntRect(267, 85, 16, 24)

#define BOMBERMAN_2_DEAD_1 sf::IntRect(185, 57, 16, 24)
#define BOMBERMAN_2_DEAD_2 sf::IntRect(204, 57, 16, 24)
#define BOMBERMAN_2_DEAD_3 sf::IntRect(221, 57, 16, 24)
#define BOMBERMAN_2_DEAD_4 sf::IntRect(238, 57, 16, 24)
#define BOMBERMAN_2_DEAD_5 sf::IntRect(255, 57, 16, 24)
#define BOMBERMAN_2_DEAD_6 sf::IntRect(273, 57, 16, 24)



#define POWER_UP_FIRE sf::IntRect(3, 3, 16, 16)
#define POWER_UP_BOMB sf::IntRect(37, 3, 16, 16)
#define POWER_UP_SPEED sf::IntRect(54, 3, 16, 16)
#define POWER_UP_PUNCH sf::IntRect(37, 20, 16, 16)
#define POWER_UP_KICK sf::IntRect(3, 20, 16, 16)

#define BURN_ANIMATION_TIME 1.0f

#define POWER_UP_BUNR_1 sf::IntRect(1, 119, 16, 16)
#define POWER_UP_BUNR_2 sf::IntRect(18, 119, 16, 16)
#define POWER_UP_BUNR_3 sf::IntRect(35, 119, 16, 16)
#define POWER_UP_BUNR_4 sf::IntRect(52, 119, 16, 16)
#define POWER_UP_BUNR_5 sf::IntRect(69, 119, 16, 16)
#define POWER_UP_BUNR_6 sf::IntRect(86, 119, 16, 16)
#define POWER_UP_BUNR_7 sf::IntRect(103, 119, 16, 16)
