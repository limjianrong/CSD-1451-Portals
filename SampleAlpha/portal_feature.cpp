/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:				portal_feature.cpp
* Course:			CSD1451
* Group Name:		Apparate
* Primary Author:	Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors: -
*
* Brief: 
  This source file implements the functions used to initialize, take player input and
  update portal coordinates as well as check for collision between game objects, and finally
  drawing the portals.
==================================================================================*/

#include "portal_feature.hpp"

//portal dimensions
float PORTAL_WIDTH{};
float PORTAL_HEIGHT{};

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
extern Camera camera;
//used to check if game is paused
extern bool isPaused;

// shared square mesh
extern AEGfxVertexList* square_mesh;
//used to print text to the screen
extern s8 Albam_fontID;

// ----- Audio -----
extern AEAudio portalAudio;
extern AEAudioGroup soundGroup;


/*************  Portal Cooldown and Max Range ***********/
float portal_max_range, portal_cooldown,portal_time_elapsed{};

//file stream to initialize portal data members
std::ifstream portal_ifs{};

//texture used by both portals
AEGfxTexture* portal_range_picture;
AEGfxTexture* portal_range_on_cooldown_picture;

//texture for cards that are drawn when choosing upgrade
AEGfxTexture* card_pic; // card picture for player upgrades
/***********************************************************************/


/*!**************************************************************************************************
\brief
  loads the assets needed for the portal as well as the cards used for player upgrades.
  opens a text file file and initializes the values for portal dimensions, max range, cooldown by reading
  from a text file
*******************************************************************************************************/
void portal_load() {
	//load picture for portal
	portal_1.picture = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile023.png");
	if (!portal_1.picture) {
		std::cout << "\nFailed to load platformPack_tile023.png.txt";
	}

	//load picture for portal max range
	portal_range_picture = AEGfxTextureLoad("Assets/portal_range.png");
	if (!portal_range_picture) {
		std::cout << "portal_range.png not loaded";
	}

	//load picture to be shown when portal is on cooldown
	portal_range_on_cooldown_picture = AEGfxTextureLoad("Assets/portal_range_on_cooldown.png");
	if (!portal_range_on_cooldown_picture) {
		std::cout << "portal_range_on_cooldown.png not loaded";
	}

	//load picture for cards used for player upgrades
	card_pic = AEGfxTextureLoad("Assets/card.png");
	if (!card_pic) {
		std::cout << "\nFailed to load card.png";
	}

	//read values from text file
	portal_ifs.open("Assets/textFiles/portal_stats.txt");
	if (!portal_ifs) {
		std::cout << "\nFailed to load portal_stats.txt";
	}

	std::string str{};
	portal_ifs >> str >> PORTAL_WIDTH;
	portal_ifs >> str >> PORTAL_HEIGHT;
	portal_ifs >> str >> portal_max_range;
	portal_ifs >> str >> portal_cooldown;
	portal_ifs.close();
} //end of portal_load


/*!**************************************************************************************************
\brief
	initialize active flags of portal objects to be false
*******************************************************************************************************/
void portal_init() {

	portal_1.active = false;
	portal_2.active = false;
	portal_1.draw_outline = false;
} //end of portal_init

/*!**************************************************************************************************
\brief
  update_portal checks for player input and creates a portal if the player input is valid(within a certain distance
  from the player). If there is invalid input when creating a portal, all portals created at that point will be
  resetted.
*******************************************************************************************************/
void update_portal() {

	//check for player input, key 'F' resets portal creation, player must create 2 portals again
	if (AEInputCheckTriggered(AEVK_F)) {
		reset_portals();
	}

	//portal cooldown
	portal_time_elapsed += static_cast<float>(AEFrameRateControllerGetFrameTime());

	//player right click to create a portal to teleport from 
	if (AEInputCheckTriggered(VK_RBUTTON) && portal_time_elapsed >= portal_cooldown) {

		//if the first portal is not active, set it to be active, assign cursor's x and y to become
		//the portal 1's x and y position
		if (!portal_1.active) {
			create_portal(portal_1);
			//if portal_1 is too far from the player, input is invalid, player must create a portal again
			if ((AEVec2Distance(&player.center, &portal_1.center)) > portal_max_range) {
				reset_portals();
			}
			else {
				//update transformation matrix of the portal 1
				update_portal_matrices(portal_1);
				// player's right click input is valid, draw an outline to show the player where portal 1 will appear
				portal_1.draw_outline = TRUE;
			}
		}

		//if right click and portal 1 is already active, set portal 2 to be active, assign cursor's x and y to become
		//the portal 2's x and y position
		else if (portal_1.active) {

			create_portal(portal_2);
			//if right click is too far from player, reset the creation for both portals
			if ((AEVec2Distance(&player.center, &portal_2.center)) > portal_max_range) {
				reset_portals();
			}

			//portal 2 input is valid, update transformation matrices of portal 2
			else {
				update_portal_matrices(portal_2);
			}
		}
	}
	
	//if portal 2 is active, it means that portal 1 and 2 inputs are valid
	//check for collision between portal and game objects
	if (portal_2.active) {

		//stop drawing the outline for the 1st portal and draw both portals instead
		portal_1.draw_outline = false;

		//check if player is colliding with portal 1 or portal 2
		check_portal_player_collision();
		//function that checks if enemy/boss bullets are colliding with the portal and teleport bullets if there is collision
		check_portal_bullet_collision();

		//function that checks if enemies are colliding with the portal
		check_portal_enemy_collision();
	} 
}//end of update_portal

//function to draw both portals, draw the outline of portal 1, update and draw the portal maximum range
void draw_portal() {
	draw_portal_range();
	
	if (portal_1.draw_outline) {
		//to show that the outline of the 1st portal is different from the actual 1st portal, we can
		//draw the outline to be more transparent than the actual portal
		AEGfxSetTransparency(0.5f);

		//draw the portal outline
		AEGfxSetTransform(portal_1.final_matrix.m);
		AEGfxTextureSet(portal_1.picture, 0.0f, 0.0f);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);
	}
	if (portal_2.active) {
		//portal_2 has been created, meaning that the portal_1 has also been created
		//draw portal 1
		AEGfxSetTransform(portal_1.final_matrix.m);
		AEGfxTextureSet(portal_1.picture, 0.0f, 0.0f);
		AEGfxMeshDraw((square_mesh), AE_GFX_MDM_TRIANGLES);

		//draw portal 2
		AEGfxSetTransform(portal_2.final_matrix.m);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
} //end of draw_portal

//empty by design
void portal_free() {} //end of portal_free


//unloads the loaded textures
void portal_unload() {
	// Texture unload
	AEGfxTextureUnload(portal_range_picture);
	AEGfxTextureUnload(portal_range_on_cooldown_picture);
	AEGfxTextureUnload(card_pic);
	AEGfxTextureUnload(portal_1.picture);
} //end of portal_unload

//function to create a portal, function is called when there is a right click input from player
void create_portal(portal& portal) {

	//set the portal active flag to be true
	portal.active = true;

	//get cursor position and assign it to the location where the portal will be created
	AEInputGetCursorPosition(&(portal.x), &(portal.y));

	//cursor's x and y is based on screen coordinates, must offset it to convert to
	//world coordinates in the game itself
	portal.x -= AEGetWindowWidth() / 2;
	portal.x += static_cast<s32>((AEGfxGetWinMinX() + AEGfxGetWinMaxX()) / 2);

	portal.y = AEGetWindowHeight() / 2 - portal.y;
	portal.y += static_cast<s32>((AEGfxGetWinMinY() + AEGfxGetWinMaxY()) / 2);

	//set a vector to the portal's center
	AEVec2Set(&(portal.center), static_cast<f32>(portal.x), static_cast<f32>(portal.y));
}

//function to update the transformation matrices of portals that have been created, function is called
//when there is a valid right click from player and a portal's active flag is set to true
void update_portal_matrices(portal& portal) {
	AEMtx33Scale(&portal.scale_matrix, PORTAL_WIDTH, PORTAL_HEIGHT);
	AEMtx33Trans(&portal.translation_matrix, static_cast<f32>(portal.x), static_cast<f32>(portal.y));
	AEMtx33Concat(&portal.final_matrix, &portal.translation_matrix, &portal.scale_matrix);
}

/*!**************************************************************************************************
\brief
	checks if player has collided with portal
*******************************************************************************************************/
void check_portal_player_collision() {
	//if player collide with portal 1
	if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &player.center, player.dimensions.x, player.dimensions.y)) {
		teleport_player(portal_2.center);

		// Portal teleportation sound
		AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
	}
	//if player collide with portal 2
	else if (AETestRectToRect(&portal_2.center, PORTAL_WIDTH, PORTAL_HEIGHT, &player.center, player.dimensions.x, player.dimensions.y)) {
		teleport_player(portal_1.center);

		// Portal teleportation sound
		AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
	}
}

/*!**************************************************************************************************
\brief
	teleports the player and sets the camera to follow the player
*******************************************************************************************************/
void teleport_player(const AEVec2& portal) {

	teleport_object(player, portal);

	//reset the time elapsed since previous teleport
	portal_time_elapsed = 0;

	//set camera to follow player if the player teleports

	camera.x = player.center.x;
	camera.y = player.center.y;
	
	//lowest value of camera x and y is 0
	camera.x = AEClamp(camera.x, 0, static_cast<f32>(INT_MAX));
	camera.y = AEClamp(camera.y, 0, static_cast<f32>(INT_MAX));
}

//function to check if enemy/boss bullets are colliding with the portal
void check_portal_bullet_collision() {
	//if boss bullet collided with portal 1, teleport the bullet, portal will be resetted after teleporting a bullet
	if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &bullet.center, bullet.width, bullet.height)) {
		bullet.isTeleported = true;
		teleport_object(bullet, portal_2);

		// Portal teleportation sound
		AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
	}
	//if boss bullet collide with portal 2
	else if (AETestRectToRect(&portal_2.center, PORTAL_WIDTH, PORTAL_HEIGHT, &bullet.center, bullet.width, bullet.height)) {
		bullet.isTeleported = true;
		teleport_object(bullet, portal_1);

		// Portal teleportation sound
		AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
	}

	//iterate through array of bullet_enemy2
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		//check if bullet_enemy2 collided with portal 1, teleport the bullet, portal will be resetted after teleporting a bullet
		if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height)) {
			bullet_enemy2[i].isTeleported = true;
			//bullet_enemy2[i].x = static_cast<f32>(portal_2.x);
			//bullet_enemy2[i].y = static_cast<f32>(portal_2.y);
			//reset_portals();
			teleport_object(bullet_enemy2[i], portal_2);

			// Portal teleportation sound
			AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
		}
		//check if bullet collided with portal 2
		else if (AETestRectToRect(&portal_2.center, PORTAL_WIDTH, PORTAL_HEIGHT, &bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height)) {
			bullet_enemy2[i].isTeleported = true;
			teleport_object(bullet_enemy2[i], portal_1);

			// Portal teleportation sound
			AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
		}
	}
}

//check if enemy has collided with portal
void check_portal_enemy_collision() {
	//portal teleport enemy1, iterate through array of enemy1
	for (s32 i = 0; i < MAX_ENEMIES_1; ++i) {
		//enemy1 collide with portal 1
		if (AETestRectToRect(&portal_1.center, PORTAL_WIDTH, PORTAL_HEIGHT, &enemy1[i].center, enemy1[i].dimensions.x, enemy1[i].dimensions.y)) {
			teleport_object(enemy1[i], portal_2);

			// Portal teleportation sound
			AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
		}

		//enemy1 collide with portal 2
		else if (AETestRectToRect(&portal_2.center, PORTAL_WIDTH, PORTAL_HEIGHT, &enemy1[i].center, enemy1[i].dimensions.x, enemy1[i].dimensions.y)) {
			teleport_object(enemy1[i], portal_1);

			// Portal teleportation sound
			AEAudioPlay(portalAudio, soundGroup, 1.f, 1.f, 0);
		}
	}
}

//resets the portals, function is called whenever a game object is teleported
void reset_portals() {

	//set active flags and draw_outline flag of portal_1 to be false
	portal_1.active = false;
	portal_2.active = false;
	portal_1.draw_outline = false;
}


/*!**************************************************************************************************
\brief
  draws a circle around the player to show the player the furthest range that he can place a portal at.
  If the player clicks a point outside of this circle, the click will be invalid and all active portals
  will be resetted to become not active
*******************************************************************************************************/
void draw_portal_range() {
	AEMtx33 portal_range_scale_mtx{};
	AEMtx33 portal_range_mtx{};
	//set transformation matrix to show the portal maximum range
	AEMtx33Scale(&portal_range_scale_mtx, portal_max_range*2, portal_max_range*2);
	AEMtx33Trans(&portal_range_mtx, player.center.x, player.center.y);
	AEMtx33Concat(&portal_range_mtx, &portal_range_mtx, &portal_range_scale_mtx);

	//draw the maximum portal range, if the portal teleportation is on cooldown,
	//use a different texture
	if (portal_time_elapsed >= portal_cooldown) {
		AEGfxTextureSet(portal_range_picture, 0.0f, 0.0f);
	}
	else {
		AEGfxTextureSet(portal_range_on_cooldown_picture, 0.0f, 0.0f);
	}

	//set transform and draw the maximum range
	AEGfxSetTransform(portal_range_mtx.m);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}
