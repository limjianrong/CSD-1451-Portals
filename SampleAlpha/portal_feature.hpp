/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File: portal_feature.hpp
* Course: CSD1451
* Group Name: Apparate
* Primary Author: Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors: -
*
* Brief:
  This source file implements the functions used for the game's portal feature

  The functions include:
  - portal_load
	  creates a square mesh for the portal

  - draw_portal
	  draws a portal and checks if the player has collided with the portal
==================================================================================*/
#pragma once
#include <string>
#include <fstream>
#include "AEEngine.h"
#include "Utilities.hpp"
#include "boss.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "camera.hpp"
#include <iostream>
struct portal {
	
	s32 x{}, y{};
	AEVec2 center{};
	AEMtx33 scale_matrix{}, translation_matrix, final_matrix{};
	bool active{};
	bool draw_outline{};
	AEGfxTexture* picture{};
}; 



void portal_load();
void portal_init();


/*!**************************************************************************************************
\brief
  draws 2 portals once the positions of both portals are valid. location of both portals are determined by right
  click. If the location of a portal is too far from the player, the cursor'x and y will not be assigned to the
  portal's x and y. The player will have to right click again, the first right click determines the coordinate
  of the first portal, and the second right click determines the coordinate of the second portal.

\param[in] playerx
  x coordinate of the player's position

\param[in] playery
  y coordinate of the player's position
*******************************************************************************************************/
void update_portal();
void portal_free();
void portal_unload();

/*!**************************************************************************************************
\brief
  draws a circle around the player to show the player the furthest point that he can place a portal at.
  Not fully implemented yet.

\param[in] playerx
  x coordinate of the player's position

\param[in] playery
  y coordinate of the player's position
*******************************************************************************************************/
void draw_portal_range();


void check_portal_bullet_collision();

void draw_portal();
void check_portal_enemy_collision();
void teleport_player(const AEVec2& portal_center);
void reset_portals();
void update_portal_matrices(portal& portal);
void create_portal(portal& portal);
void check_portal_player_collision();

template<typename T1, typename T2=portal>
void teleport_object(T1& gameObject, T2& portal) {
	gameObject->center.x = static_cast<f32>(portal.x);
	gameObject->center.y = static_cast<f32>(portal.y);
	reset_portals();
}
