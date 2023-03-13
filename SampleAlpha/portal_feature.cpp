/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File: portal_feature.cpp
* Course: CSD1451
* Group Name: Apparate
* Primary Author: Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors: -
*
* Brief: 
  This source file implements the functions used to initialize, draw the portal and the
  portal's valid selection area.
==================================================================================*/
#include "AEEngine.h"
#include "Utilities.hpp"
#include <cmath> //for square root function
#include "portal_feature.hpp"
#include "weapon_fire.hpp"
#include <iostream> //for std::cout
#include <cstdlib> //for absolute value, abs()
#include "Player.hpp"
#include "Enemy.hpp"
#include <string>

//portal dimensions
float constexpr PORTAL_WIDTH{ 60.0f };
float constexpr PORTAL_HEIGHT{ 60.0f };

//portal objects for teleporting player from one location to another
portal portal_1, portal_2;

//used to check portal collision with enemy1 and enemy2
extern Enemy1_stats enemy1[MAX_ENEMIES_1];
extern Enemy2_stats enemy2[MAX_ENEMIES_2];

//used to check portal collision with bullet
extern Bullet bullet;
extern Bullet bullet_enemy2[MAX_ENEMIES_2];

//used to check portal collision with player
extern Player_stats player;

//used to check if game is paused
extern bool isPaused;

//used to print text to the screen
extern s8 Albam_fontID;

/**************common variables, not specific to either portal***********/
float portal_max_range, portal_cooldown, portal_timer{};

//used to check if portal cooldown is to be decremented or not
bool decrease_cooldown;

//texture and mesh used by both portals
AEGfxTexture* portal_range_picture;
AEGfxVertexList* portal_range_mesh{}; //mesh to draw the portal's valid range

//texture for cards that are drawn when choosing upgrade
AEGfxTexture* temp; // TEMP
/***********************************************************************/


/*!**************************************************************************************************
\brief
  draws a square mesh using 2 triangle meshes and assigns them to portal_1.mesh and portal_2.mesh.
  portal_1's mesh is green in color and portal_2's mesh is red in color.
*******************************************************************************************************/
void portal_load() {
	portal_range_picture = AEGfxTextureLoad("Assets/portal_range.png");
	AE_ASSERT(portal_range_picture); // check if texture is loaded
	temp = AEGfxTextureLoad("Assets/card.png");

	// 1x1 mesh, use standardized mesh, TRS will be done when updating
	portal_1.mesh = portal_2.mesh = create_Square_Mesh();
	portal_range_mesh = create_Square_Mesh();

}

void portal_init() {
	// Initialise
	portal_max_range = 300.0f;
	portal_cooldown = 50.0f;
	decrease_cooldown = false;
}

/*!**************************************************************************************************
\brief
  draws 2 portals once the positions of both portals are valid. location of both portals are determined by right
  click. If the location of a portal is too far from the player, the cursor'x and y will not be assigned to the
  portal's x and y. The player will have to right click again, the first right click determines the coordinate
  of the first portal, and the second right click determines the coordinate of the second portal.


\param[in] PlayerCenter
  Takes in a pointer to the player's center, if cursor is too far from player's center, player has to right click
  a valid location.

\param[in] player.x
  x coordinate of the player's position

\param[in] player.y
  y coordinate of the player's position
*******************************************************************************************************/
void update_portal() {
	//check for player input, 'F' resets portal creation, player must create 2 portals again
	if (AEInputCheckTriggered(AEVK_F)) {
		portal_1.created = false;
		portal_2.created = false;
		portal_1.draw_outline = false;
	}

	//If player right clicked and the portal timer == 0, player can create a portal
	//else, player must wait for portal timer to be decremented to 0 before he can create
	//a portal
	if (AEInputCheckTriggered(VK_RBUTTON) && portal_timer == 0.0f) {
		//if the first portal has not been created, create it, assign cursor's x and y to become
		//the portal's x and y 
		if (portal_1.created == false) {
			portal_1.created = true;
			AEInputGetCursorPosition(&(portal_1.x), &(portal_1.y));

			//cursor's x is based on screen coordinates, must offset it to convert to
			//world coordinates in the game itself
			portal_1.x -= AEGetWindowWidth() / 2;
			
			//offset, similar to cursor's y
			portal_1.y = AEGetWindowHeight() / 2 - portal_1.y;
			if (player.x > 0) {
				portal_1.x += player.x;
			}
			if (player.y > 0) {
				portal_1.y += player.y;
			}
			
			//set a vector to the 1st portal's center
			AEVec2Set(&(portal_1.center), static_cast<f32>(portal_1.x), static_cast<f32>(portal_1.y));

			//if right click is too far from the player, input is invalid, player must create a portal again
			if (sqrt(AEVec2SquareDistance(&player.center, &portal_1.center)) > portal_max_range) {
				portal_1.created = false;
				portal_2.created = false;
				portal_1.draw_outline = false;
			}
			else {
				//update transformation matrix of the first portal
				AEMtx33Scale(&portal_1.scale_matrix, PORTAL_WIDTH, PORTAL_HEIGHT);
				AEMtx33Trans(&portal_1.matrix, static_cast<f32>(portal_1.x), static_cast<f32>(portal_1.y));
				AEMtx33Concat(&portal_1.matrix, &portal_1.matrix, &portal_1.scale_matrix);
				//else, player's right click input is valid, set a token to draw an outline of where the 1st portal will appear
				portal_1.draw_outline = TRUE;
			}
		}

		//if first right click is valid, and there is a second right click, assign the cursor's x and y to portal_2's x and y
		else if (portal_1.created == true) {
			AEInputGetCursorPosition(&(portal_2.x), &(portal_2.y));

			//offset portal_2's x
			portal_2.x -= AEGetWindowWidth() / 2;
			if (player.x > 0) {
				portal_2.x += player.x;
			}

			//offset portal_2.y by windowheight()/2
			portal_2.y = AEGetWindowHeight() / 2 - portal_2.y;
			if (player.y > 0) {
				portal_2.y += player.y;
			}

			//set vector to portal_2's center
			AEVec2Set(&(portal_2.center), static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
			

			portal_2.created = TRUE;
			//if right click is too far from player, reset the creation for both portals
			if (sqrt(AEVec2SquareDistance(&player.center, &portal_2.center)) > portal_max_range) {
				portal_1.created = false;
				portal_2.created = false;
				portal_1.draw_outline = false;
			}		
		}
	}
	
	//if portal_2.created==true, it means that portal_1 and 2 inputs are valid, both portals can now be drawn
	if (portal_2.created == true) {
		//stop drawing the outline for the 1st portal(which does not teleport anything) and draw both portals instead
		portal_1.draw_outline = false;

		//update the transformation matrix for 2nd portal
		AEMtx33Scale(&portal_2.scale_matrix, PORTAL_WIDTH, PORTAL_HEIGHT);
		AEMtx33Trans(&portal_2.matrix, static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
		AEMtx33Concat(&portal_2.matrix, &portal_2.matrix, &portal_2.scale_matrix);


		//check if player is colliding with 1st portal, if collided, teleport player to the position of the 2nd portal
		// stop drawing both portals and set a cooldown before the player can create portals again
		if (AETestRectToRect(&(portal_1.center), 60.0f, 60.0f, &player.center, 50.0f, 50.0f)) {
			player.x = portal_2.center.x;
			player.y = portal_2.center.y;
			portal_1.created = false;
			portal_2.created = false;
			decrease_cooldown = true;
			portal_timer = portal_cooldown;
		}

		//function that checks if enemy/boss bullets are colliding with the portal
		check_bullet_collide_with_portal();
	}

	//cooldown to limit the frequency of portal usage
	if (decrease_cooldown == true && portal_timer > 0.0f && isPaused == false) {
		portal_timer--;
		if (portal_timer == 0.0f) {
			decrease_cooldown = false;
		}
	}
	portal_teleport_enemy();

}//end of update_portal


/*!**************************************************************************************************
\brief
  draws a circle around the player to show the player the furthest range that he can place a portal at.
  If the player clicks a point outside of this circle, the click will be invalid and a portal will not be
  created.

\param[in] player.x
  x coordinate of the player's position

\param[in] player.y
  y coordinate of the player's position
*******************************************************************************************************/
void portal_range() {
	AEMtx33 portal_range_scale_mtx{};
	AEMtx33 portal_range_mtx{};
	//set transformation matrix of circle that shows the portals maximum range
	AEMtx33Scale(&portal_range_scale_mtx, portal_max_range*2, portal_max_range*2);
	AEMtx33Trans(&portal_range_mtx, player.x, player.y);
	AEMtx33Concat(&portal_range_mtx, &portal_range_mtx, &portal_range_scale_mtx);

	//draw the maximum portal range
	AEGfxTextureSet(portal_range_picture, 0.0f, 0.0f);
	AEGfxSetTransform(portal_range_mtx.m);
	AEGfxMeshDraw(portal_range_mesh, AE_GFX_MDM_TRIANGLES);
}

//function to check if enemy/boss bullets are colliding with the portal
void check_bullet_collide_with_portal() {
	//if boss bullet collided with portal, teleport the bullet, destroy the portal after teleporting
	if (AETestRectToRect(&portal_1.center,PORTAL_WIDTH, PORTAL_HEIGHT, &bullet.center,bullet.width, bullet.height)) {
		bullet.isTeleported = true;
		bullet.x = portal_2.x;
		bullet.y = portal_2.y;
		portal_1.created = false;
		portal_2.created = false;
		portal_1.draw_outline = false;
	}
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		//check if bullet_enemy2 collided with portal, teleport the bullet, destroy the portal after teleporting
		if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height)) {
			bullet_enemy2[i].isTeleported = true;
			bullet_enemy2[i].x = portal_2.x;
			bullet_enemy2[i].y = portal_2.y;
			portal_1.created = false;
			portal_2.created = false;
			portal_1.draw_outline = false;
		}
	}
}

//function to draw both portals, draw the outline of 1st portal(but cannot teleport anything), update and draw the portal maximum range
void draw_portal() {
	portal_range();
	//
	if (portal_1.draw_outline == true) {
		//to show that the outline of the 1st portal is different from the actual 1st portal, we can
		//draw the outline to be more transparent than the actual portal
		AEGfxSetTransparency(0.5f);

		//draw the portal outline
		AEGfxSetTransform(portal_1.matrix.m);
		AEGfxTextureSet(temp, 0.0f, 0.0f);
		AEGfxMeshDraw(portal_1.mesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);
	}
	if (portal_2.created == true) {
		//portal_2 has been created, meaning that the portal_1 has also been created
		//draw portal 1
		AEGfxSetTransform(portal_1.matrix.m);
		AEGfxTextureSet(temp, 0.0f, 0.0f);
		AEGfxMeshDraw((portal_1.mesh), AE_GFX_MDM_TRIANGLES);

		//draw portal 2
		AEGfxSetTransform(portal_2.matrix.m);
		AEGfxMeshDraw(portal_2.mesh, AE_GFX_MDM_TRIANGLES);
	}

	//print the portal cooldown to the screen
	//AEGfxPrint(Albam_fontID, "portal timer: ", 0.6f, 0.92f, 1, 0.0f, 0.0f, 1.0f);
	std::string portal_timer_string = std::to_string(static_cast<int>(portal_timer));
	AEGfxPrint(Albam_fontID, &portal_timer_string[0], -0.50f, 0.40f, 1.0f, 0.0f, 0.0f, 0.0f);
}

void portal_teleport_enemy() {
	if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &enemy1->center, enemy1->width, enemy1->height) && portal_2.created == true) {
		enemy1->x = portal_2.x;
		enemy1->y = portal_2.y;
		portal_1.created = false;
		portal_2.created = false;
		portal_1.draw_outline = false;
	}

	if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &enemy2->center, enemy2->width, enemy2->height) && portal_2.created == true) {
		enemy2->x = portal_2.x;
		enemy2->y = portal_2.y;
		portal_1.created = false;
		portal_2.created = false;
		portal_1.draw_outline = false;
	}

}
