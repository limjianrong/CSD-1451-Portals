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


struct portal {
	
	s32 x{}, y{};
	AEGfxVertexList* mesh{};
	AEVec2 center{ 2000, 2000 };
	AEMtx33 matrix{};
	AEMtx33 scale_matrix{};
	bool created{};
	bool draw_outline{};
}; 




/*!**************************************************************************************************
\brief
  draws a square mesh using 2 triangle meshes and assigns them to portal_1.mesh and portal_2.mesh.
  portal_1's mesh is green in color and portal_2's mesh is red in color.
*******************************************************************************************************/
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


void check_bullet_collide_with_portal();

void draw_portal();