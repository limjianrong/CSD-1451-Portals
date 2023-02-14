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
#include <cmath> //for square root function
#include "portal_feature.hpp"
#include "weapon_fire.hpp"
#include <iostream> //for std::cout
#include <cstdlib> //for absolute value, abs()

#define PORTAL_WIDTH 60.0f
#define PORTAL_HEIGHT 60.0f
#define TRUE 1
#define FALSE 0

int drawportal{}, token{}; //drawportal and token are used in draw_portal
int draw_portal_outline{};
portal portal_1, portal_2;
extern f32 bullet_x;
extern f32 bullet_y;
extern f32* pointer_to_bullet_x{ &bullet_x };
extern f32* pointer_to_bullet_y{ &bullet_y };

AEGfxTexture* greencircle;
AEGfxVertexList* greencirclemesh{}; //mesh to draw the portal's valid range
//f32* pointer_to_bullet_x{ &bullet_x };
//f32* pointer_to_bullet_y{ &bullet_y };
/*!**************************************************************************************************
\brief
  draws a square mesh using 2 triangle meshes and assigns them to portal_1.mesh and portal_2.mesh.
  portal_1's mesh is green in color and portal_2's mesh is red in color.
*******************************************************************************************************/
void initialize_portal() {
	greencircle = AEGfxTextureLoad("Assets/greencircle1.png");
	//draw mesh for portal_1 (green color)
	//std::cout << "initialized portal center is" << portal_1->center.x;
	//if (!portal_1->mesh) {
	//	std::cout << "mesh is  nullptr";
	//}
	AEGfxMeshStart();
	AEGfxTriAdd(
		30.0f, -30.0f, 0xFF00FF00, 0.0f, 0.0f,
		30.0f, 30.0f, 0xFF00FF00, 0.0f, 0.0f,
		-30.0f, 30.0f, 0xFF00FF00, 0.0f, 0.0f);
	AEGfxTriAdd(-30.0f, 30.0f, 0xFF00FF00, 0.0f, 0.0f,
		-30.0f, -30.0f, 0xFF00FF00, 0.0f, 0.0f,
		30.0f, -30.0f, 0xFF00FF00, 0.0f, 0.0f);
	(portal_1.mesh) = { AEGfxMeshEnd() };


	//draw mesh for portal_2 (red color)
	AEGfxMeshStart();
	AEGfxTriAdd(
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	(portal_2.mesh)  = { AEGfxMeshEnd() };

	//draw mesh to show the player the furthest point he can place a portal at. 
	AEGfxMeshStart();
	AEGfxTriAdd(
		150.0f, -150.0f, 0xFFFF0000, 0.0f, 0.0f,
		150.0f, 150.0f, 0xFFFF0000, 0.0f, 0.0f,
		-150.0f, 150.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(-150.0f, 150.0f, 0xFFFF0000, 0.0f, 0.0f,
		-150.0f, -150.0f, 0xFFFF0000, 0.0f, 0.0f,
		150.0f, -150.0f, 0xFFFF0000, 0.0f, 0.0f);
	greencirclemesh = AEGfxMeshEnd();

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

\param[in] playerx
  x coordinate of the player's position

\param[in] playery
  y coordinate of the player's position
*******************************************************************************************************/
void draw_portal(f32& playerx, f32& playery) {

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);


	AEVec2 PlayerCenter{};
	AEVec2Set(&PlayerCenter, playerx, playery);
	
	if (AEInputCheckTriggered(VK_RBUTTON)) { // first right click, assign cursor x and y to portal_1 x and y.
		if (drawportal == 0) {
			drawportal = 1;
			AEInputGetCursorPosition(&(portal_1.x), &(portal_1.y));

			std::cout << "\nportal 1 before minus" << portal_1.x;
			//offset portal_1's x by half of window width

			portal_1.x -= AEGetWindowWidth() / 2;
			
			//std::cout << "\nportal 1 after minus" << portal_1.x;
			//std::cout << "\nplayercenter x is" << PlayerCenter.x;
			//offset portal_1's y by half of the window height
			portal_1.y = AEGetWindowHeight() / 2 - portal_1.y;
			if (playerx > 0) {
				portal_1.x += playerx;
				//std::cout << "\nportal 1 x after += portal_width" << portal_1.x;
			}

			//set vector to portal_1's center
			AEVec2Set(&(portal_1.center), static_cast<f32>(portal_1.x), static_cast<f32>(portal_1.y));

			//if right click is too far from the player, input is invalid, player must select again
			if (sqrt(AEVec2SquareDistance(&PlayerCenter, &portal_1.center)) > 300) {
				std::cout << "\nportal 1 selection is out of range, select again";
				drawportal = 0;
				token = 0;
				draw_portal_outline = FALSE;
			}
			else {
				draw_portal_outline = TRUE;
			}
		}

		//if first right click is valid, and there is a second right click, assign the cursor's x and y to portal_2's x and y
		else if (drawportal == 1) {
			AEInputGetCursorPosition(&(portal_2.x), &(portal_2.y));

			//offset portal_2's x by half of window width
			portal_2.x -= AEGetWindowWidth() / 2;
			if (playerx > 0) {
				portal_2.x += playerx;
				//std::cout << "\nportal 2 x after += portal_width" << portal_2.x;
			}

			//offset portal_2.y by windowheight()/2
			portal_2.y = AEGetWindowHeight() / 2 - portal_2.y;
			
			//set vector to portal_2's center
			AEVec2Set(&(portal_2.center), static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
			
			
			token = 1;
			//if there is right click but cursor is too far from player, input for portal_2's x and y is invalid, player will START OVER and select portal_1's x and y again
			//if there is right click and cursor is within range, and portal_2 is already being drawn, player can change the position of portal_2 without 
			//resetting portal_1's location
			if (sqrt(AEVec2SquareDistance(&PlayerCenter, &portal_2.center)) > 500) {
				std::cout<<"\nportal 2 selection is out of range";
				drawportal = 0;
				token = 0;
				draw_portal_outline = FALSE;
			}		
		}
	}
	if (draw_portal_outline == TRUE) {
		AEGfxSetTransparency(0.5f);
		AEMtx33Trans(&portal_1.matrix, static_cast<f32>(portal_1.x), static_cast<f32>(portal_1.y));
		AEGfxSetTransform(portal_1.matrix.m);
		AEGfxMeshDraw(portal_1.mesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);
	}
	//if token==1, it means that portal_1 and 2 inputs are valid, both portals can now be drawn
	if (token == 1) {
		draw_portal_outline = FALSE;
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//if player has passed the center of the screen

		//AEMtx33Trans(&portal_1.matrix, static_cast<f32>(portal_1.x), static_cast<f32>(portal_1.y));
		AEMtx33Trans(&portal_2.matrix, static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
		
		AEGfxSetTransform(portal_1.matrix.m);

		AEGfxMeshDraw((portal_1.mesh), AE_GFX_MDM_TRIANGLES);

		
		//AEMtx33Trans(&portal_2.matrix, static_cast<f32>(portal_2.x), static_cast<f32>(portal_2.y));
		AEGfxSetTransform(portal_2.matrix.m);
		AEGfxMeshDraw(portal_2.mesh, AE_GFX_MDM_TRIANGLES);

		//check if player is colliding with portal_1, if collided, stop drawing both portals and let player
		//select where to draw the portals again
		if (AETestRectToRect(&(portal_1.center), 60.0f, 60.0f, &PlayerCenter, 50.0f, 50.0f)) {
			playerx = portal_2.center.x;
			playery = portal_2.center.y;
			drawportal = 0;
			token = 0;
		}

		check_bullet_collide_with_portal(bullet_x, bullet_y);
	}

	//std::cout << "\nportal.center.x is" << portal_1->x;

}//end of draw_portal


/*!**************************************************************************************************
\brief
  draws a circle around the player to show the player the furthest range that he can place a portal at.
  If the player clicks a point outside of this circle, the click will be invalid and a portal will not be
  created. Not fully implemented yet.

\param[in] playerx
  x coordinate of the player's position

\param[in] playery
  y coordinate of the player's position
*******************************************************************************************************/
void draw_portal_range(f32 playerx, f32 playery) {
	AEMtx33 portal_range_mtx{};
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(greencircle, 0, 0);
	AEMtx33Trans(&portal_range_mtx, playerx, playery);
	AEGfxSetTransform(portal_range_mtx.m);

	AEGfxMeshDraw(greencirclemesh, AE_GFX_MDM_LINES_STRIP);
}

int check_bullet_collide_with_portal(f32 bullet_x,f32 bullet_y) {
	AEVec2 bullet_center{};
	f32 bullet_width{ 20.0f }, bullet_height{ 20.0f }; //needs to be standardized in weapon_fire.cpp

	AEVec2Set(&bullet_center, bullet_x, bullet_y);
	//std::cout << "\nprotal1x is" << portal_1.x;
	if (AETestRectToRect(&portal_1.center,PORTAL_WIDTH, PORTAL_HEIGHT, &bullet_center,bullet_width, bullet_height)) {
		std::cout << "\nBULLET COLLIDED WITH PORTAL";
		*pointer_to_bullet_x= portal_2.x;
		*pointer_to_bullet_y = portal_2.y;
	}

	return 0;
}

