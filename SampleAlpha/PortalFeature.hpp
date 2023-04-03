/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:				PortalFeature.hpp
* Course:			CSD1451
* Group Name:		Apparate
* Primary Author:	Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors: -
*
* Brief:
  This source file implements the functions declarations used for the game's portal feature,
  such as creating a portal, checking for collision between portal and game objects,
  drawing of portal
==================================================================================*/
#pragma once
#include <string>
#include <fstream> //file IO
#include "AEEngine.h"
#include "Boss.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Enemy3.hpp" 
#include "Camera.hpp"
#include <iostream> //for std::cout if files are not opened
#include "limits.h" //for INT_MAX
struct portal {
	
	//position of portal
	s32 x{}, y{};

	//center of portal
	AEVec2 center{};

	//transformation matrices
	AEMtx33 scale_matrix{}, translation_matrix, final_matrix{};

	//active or in-active flag
	bool active{};

	//flag to draw outline of portal
	bool draw_outline{};

	//texture for portal
	AEGfxTexture* picture{};
}; 

/*!**************************************************************************************************
\brief
  loads the assets needed for the portal as well as the cards used for player upgrades.
  opens a text file file and initializes the values for portal dimensions, max range, cooldown by reading
  from a text file
*******************************************************************************************************/
void portal_load();


//sets active flags of portal objects to be false
void portal_init();


/*!**************************************************************************************************
\brief
  update_portal checks for player input and creates a portal if the player input is valid(within a certain distance
  from the player). If there is invalid input when creating a portal, all portals created at that point will be
  resetted.
*******************************************************************************************************/
void update_portal();

//function to draw both portals, draw the outline of portal 1, update and draw the portal maximum range
void draw_portal();

//free portal objects
void portal_free();

//unloads the loaded textures
void portal_unload();

//function to create a portal, function is called when there is a right click input from player
void create_portal(portal& portal);

//function to update the transformation matrices of portals that have been created, function is called
//when there is a valid right click from player and a portal's active flag is set to true
void update_portal_matrices(portal& portal);

//checks if player has collided with portal
void check_portal_player_collision();

//teleports the player and sets the camera to follow the player
void teleport_player(const AEVec2& portal_center);

//function to check if enemy/boss bullets are colliding with the portal
void check_portal_bullet_collision();

//check if enemy has collided with portal
void check_portal_enemy_collision();

//resets the portals, function is called whenever a game object is teleported
void reset_portals();



 //draws a circle around the player to show the player the furthest point that he can place a portal at.
void draw_portal_range();

//function template used for teleporting objects
template <typename T1, typename T2 = portal>
void teleport_object(T1& gameObject, T2& portal) {
	//set gameObject position to be the same position as the portal passed as input parameter
	gameObject.center.x = static_cast<f32>(portal.x);
	gameObject.center.y = static_cast<f32>(portal.y);
	reset_portals();
}
