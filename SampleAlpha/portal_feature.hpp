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
  - initialize_portal
	  creates a square mesh for the portal

  - draw_portal
	  draws a portal and checks if the player has collided with the portal
==================================================================================*/
#pragma once

/*!**************************************************************************************************
\brief
  draws a square mesh using 2 triangle meshes and assigns them to portal_1.mesh and portal_2.mesh.
  portal_1's mesh is green in color and portal_2's mesh is red in color.
*******************************************************************************************************/
void initialize_portal(void);

/*!**************************************************************************************************
\brief
  draws 2 portals once the positions of both portals are valid. location of both portals are determined by right
  click. If the location of a portal is too far from the player, the cursor'x and y will not be assigned to the
  portal's x and y. The player will have to right click again, the first right click determines the coordinate
  of the first portal, and the second right click determines the coordinate of the second portal.


\param[in] PlayerCenter
  Takes in a pointer to the player's center, if cursor is too far from player's center, player has to right click
  a valid location.

\param[in] playerx
  x coordinate of the player's position

\param[in] playery
  y coordinate of the player's position
*******************************************************************************************************/
void draw_portal(AEVec2* PlayerCenter, f32 &playerx, f32 &playery);


/*!**************************************************************************************************
\brief
  draws a circle around the player to show the player the furthest point that he can place a portal at.
  Not fully implemented yet.

\param[in] playerx
  x coordinate of the player's position

\param[in] playery
  y coordinate of the player's position
*******************************************************************************************************/
void draw_portal_range(f32 playerx, f32 playery);