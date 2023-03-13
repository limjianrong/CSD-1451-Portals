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
//#include "Upgrades.hpp"
#define TRUE 1
#define FALSE 0

float constexpr PORTAL_WIDTH{ 60.0f };
float constexpr PORTAL_HEIGHT{ 60.0f };

portal portal_1, portal_2;
extern Bullet bullet;
extern Bullet bullet_enemy2[MAX_ENEMIES_2];
extern Player_stats player;
extern bool isPaused;
//portal range
float portal_range{ 300.0f };
float portal_cooldown{ 50.0f };
float portal_timer{};
bool decrease_cooldown = false;
AEGfxTexture* portal_range_picture;
AEGfxVertexList* portal_range_mesh{}; //mesh to draw the portal's valid range

AEGfxTexture* temp; // TEMP
/*!**************************************************************************************************
\brief
  draws a square mesh using 2 triangle meshes and assigns them to portal_1.mesh and portal_2.mesh.
  portal_1's mesh is green in color and portal_2's mesh is red in color.
*******************************************************************************************************/
void portal_load() {
	portal_range_picture = AEGfxTextureLoad("Assets/portal_range.png");
	AE_ASSERT(portal_range_picture); // Similar to your checking
	temp = AEGfxTextureLoad("Assets/card.png");

	portal_1.mesh = portal_2.mesh = create_Square_Mesh();
	portal_range_mesh = create_Square_Mesh();
	//int Parts = 12;
	//for (float i = 0; i < Parts; ++i)
	//{
	//	AEGfxTriAdd(
	//		0.0f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
	//		cosf(i * 2 * PI / Parts) * 0.5f, sinf(i * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
	//		cosf((i + 1) * 2 * PI / Parts) * 0.5f, sinf((i + 1) * 2 * PI / Parts) * 0.5f, 0xFFFFFF00, 0.0f, 0.0f);
	//}

	//portal_range_mesh = AEGfxMeshEnd();

}

void portal_init() {
	// Initialise
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
	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		std::cout << "\nportal_cooldown is" << portal_timer;
	}
	
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	if (AEInputCheckTriggered(AEVK_F)) {
		portal_1.created = FALSE;
		portal_2.created = FALSE;
		portal_1.draw_outline = FALSE;
	}
	if (AEInputCheckTriggered(VK_RBUTTON) && portal_timer == 0.0f) { // first right click, assign cursor x and y to portal_1 x and y.
		if (portal_1.created == FALSE) {
			portal_1.created = TRUE;
			AEInputGetCursorPosition(&(portal_1.x), &(portal_1.y));
			//std::cout << "\nraw mouse y_pos " << portal_1.y;
			//std::cout << "\nraw value x_pos is " << portal_1.x;
			//offset portal_1's x by half of window width

			portal_1.x -= AEGetWindowWidth() / 2;
			
			//offset portal_1's y by half of the window height
			portal_1.y = AEGetWindowHeight() / 2 - portal_1.y;
			if (player.x > 0) {
				portal_1.x += player.x;
			}
			if (player.y > 0) {
				portal_1.y += player.y;
			}
			//std::cout << "\nafter offsetting portal_1 y_pos " << portal_1.y;
			//std::cout << "\nafter offsetting portal_1 x_pos " << portal_1.x;
			//set vector to portal_1's center
			AEVec2Set(&(portal_1.center), static_cast<f32>(portal_1.x), static_cast<f32>(portal_1.y));

			//if right click is too far from the player, input is invalid, player must select again
			if (sqrt(AEVec2SquareDistance(&player.center, &portal_1.center)) > portal_range) {
				std::cout << "\nportal 1 selection is out of range, select again";
				portal_1.created = FALSE;
				portal_2.created = FALSE;
				portal_1.draw_outline = FALSE;
			}
			else {
				portal_1.draw_outline = TRUE;
			}
		}

		//if first right click is valid, and there is a second right click, assign the cursor's x and y to portal_2's x and y
		else if (portal_1.created == 1) {
			AEInputGetCursorPosition(&(portal_2.x), &(portal_2.y));

			//offset portal_2's x by half of window width
			portal_2.x -= AEGetWindowWidth() / 2;
			if (player.x > 0) {
				portal_2.x += player.x;
				//std::cout << "\nportal 2 x after += portal_width" << portal_2.x;
			}

			portal_2.y = AEGetWindowHeight() / 2 - portal_2.y;
			if (player.y > 0) {
				portal_2.y += player.y;
			}

			//offset portal_2.y by windowheight()/2

			
			//set vector to portal_2's center
			AEVec2Set(&(portal_2.center), static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
			
			
			portal_2.created = TRUE;
			//if there is right click but cursor is too far from player, input for portal_2's x and y is invalid, player will START OVER and select portal_1's x and y again
			//if there is right click and cursor is within range, and portal_2 is already being drawn, player can change the position of portal_2 without 
			//resetting portal_1's location
			if (sqrt(AEVec2SquareDistance(&player.center, &portal_2.center)) > portal_range) {
				std::cout<<"\nportal 2 selection is out of range";
				portal_1.created = FALSE;
				portal_2.created = FALSE;
				portal_1.draw_outline = FALSE;
			}		
		}
	}
	
	//if portal_2.created==TRUE, it means that portal_1 and 2 inputs are valid, both portals can now be drawn
	if (portal_2.created == TRUE) {
		portal_1.draw_outline = FALSE;
		//AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		//AEMtx33Trans(&portal_2.matrix, static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
		AEMtx33Scale(&portal_2.scale_matrix, PORTAL_WIDTH, PORTAL_HEIGHT);
		AEMtx33Trans(&portal_2.matrix, static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
		AEMtx33Concat(&portal_2.matrix, &portal_2.matrix, &portal_2.scale_matrix);


		//check if player is colliding with portal_1, if collided, stop drawing both portals and let player
		//select where to draw the portals again
		if (AETestRectToRect(&(portal_1.center), 60.0f, 60.0f, &player.center, 50.0f, 50.0f)) {
			player.x = portal_2.center.x;
			player.y = portal_2.center.y;
			
			portal_1.created = FALSE;
			portal_2.created = FALSE;
			decrease_cooldown = true;
			portal_timer = portal_cooldown;
		}

		check_bullet_collide_with_portal();
	}
	if (decrease_cooldown == true && portal_timer > 0.0f && isPaused == false) {
		portal_timer--;
		if (portal_timer == 0.0f) {
			decrease_cooldown = false;
		}
	}

}//end of draw_portal


/*!**************************************************************************************************
\brief
  draws a circle around the player to show the player the furthest range that he can place a portal at.
  If the player clicks a point outside of this circle, the click will be invalid and a portal will not be
  created. Not fully implemented yet.

\param[in] player.x
  x coordinate of the player's position

\param[in] player.y
  y coordinate of the player's position
*******************************************************************************************************/
void draw_portal_range() {
	AEMtx33 portal_range_scale_mtx{};
	AEMtx33 portal_range_mtx{};
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(portal_range_picture, 0.0f, 0.0f);
	AEMtx33Scale(&portal_range_scale_mtx, portal_range*2, portal_range*2);
	AEMtx33Trans(&portal_range_mtx, player.x, player.y);
	AEMtx33Concat(&portal_range_mtx, &portal_range_mtx, &portal_range_scale_mtx);
	AEGfxSetTransform(portal_range_mtx.m);
	AEGfxMeshDraw(portal_range_mesh, AE_GFX_MDM_TRIANGLES);
}

void check_bullet_collide_with_portal() {

	
	//check if boss bullet collided with portal
	if (AETestRectToRect(&portal_1.center,PORTAL_WIDTH, PORTAL_HEIGHT, &bullet.center,bullet.width, bullet.height)) {
		bullet.isTeleported = TRUE;
		bullet.x = portal_2.x;
		bullet.y = portal_2.y;
		portal_1.created = FALSE;
		portal_2.created = FALSE;
		portal_1.draw_outline = FALSE;
	}
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		//check if bullet_enemy2 collided with portal 
		//AEVec2Set(&bullet_enemy2.center, bullet_enemy2.x, bullet_enemy2.y);
		if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height)) {
			bullet_enemy2[i].isTeleported = TRUE;
			bullet_enemy2[i].x = portal_2.x;
			bullet_enemy2[i].y = portal_2.y;
			portal_1.created = FALSE;
			portal_2.created = FALSE;
			portal_1.draw_outline = FALSE;
		}
	}
}

void draw_portal() {
	draw_portal_range();
	if (portal_1.draw_outline == TRUE) {
		AEGfxSetTransparency(0.5f);
		AEMtx33Scale(&portal_1.scale_matrix, PORTAL_WIDTH, PORTAL_HEIGHT);
		AEMtx33Trans(&portal_1.matrix, static_cast<f32>(portal_1.x), static_cast<f32>(portal_1.y));
		AEMtx33Concat(&portal_1.matrix, &portal_1.matrix, &portal_1.scale_matrix);
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

}

