/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Tutorial.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lee Zhi Yee (zhiyee.l@digipen.edu)
*
* Brief:
  This source file declares functions for implementing the Tutorial Menu, defines 
  enumerations for top and bottom row assetss, and defines their respective count.
==================================================================================*/
#pragma once

const int asset_count{ 4 };
const int asset2_count{ 4 };

void tutorial_load(void);
void tutorial_draw(void);
void tutorial_unload(void);

enum asset {
	_player,
	portals,
	range,
	enemy1
};

enum asset2 {
	enemy2,
	enemy3,
	enemy3_warning,
	_boss
};


