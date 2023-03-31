/******************************************************************************//*!
\file		GameState_Platformer.cpp
\author 	Digipen, Lin ZhaoZhi
\par    	email: z.lin@digipen.edu
\date   	30 January, 2023
\brief		Source file for the platformer game state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#include "AEEngine.h"

// ----- Game States -----
#include "GameStateManager.hpp"
#include "GameState_Mainmenu.hpp"
#include "GameStateList.hpp"

// ----- Player related -----
#include "Player.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "Upgrades.hpp"

// ----- Enemies related -----
#include "Enemy.hpp"
#include "Enemy3.hpp"
#include "boss.hpp"

// ----- Camera ------
#include "camera.hpp"

// ----- Others -----
#include "Utilities.hpp"
#include "draw_level.hpp"


//#include <iostream>

// --- Mesh ---
extern AEGfxVertexList* square_mesh;													// Created square mesh
extern AEMtx33 scale, rotate, translate, transform;										// TRS
extern s8 Albam_fontID;																	// FontID
static AEGfxTexture* buttonNotPressed, * buttonPressed;									// Button textures
AEGfxTexture* background_layer1Tex, * background_layer2Tex, * background_layer3Tex;		// Background texture

// ----- Game objects -----
extern Player_stats player;		// player stats

// ----- Game states -----
bool isPaused;

// ----- Cursor positions -----
extern AEVec2 cursor;					// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;			// Origin is CENTER of window
extern AEVec2 world_center_cursor;		// Origin is CENTER of window
// ----- Window origin -----
extern AEVec2 origin;					// Center of screen, no matter where the camera moves

// ----- Camera -----
extern Camera camera;

// ----- Audio -----
//AEAudioGroup soundGroup;

/*!**************************************************************************************************
\brief
  In charge of loading platformer game
*******************************************************************************************************/
void GameStatePlatformerLoad(void) {

	// --- Loads fontID into memory ---
	//Albam_fontID = AEGfxCreateFont("Assets/Albam.ttf", 50);

	// --- Loads textures ---
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	background_layer1Tex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Backgrounds/set2_background.png");
	background_layer2Tex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Backgrounds/set2_tiles.png");
	background_layer3Tex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Backgrounds/set2_hills.png");

	// --- Loads mesh ---
	mesh_load();

	// --- Loads different functions ---
	draw_level_load();			// Level
	enemies_load();				// Enemy1 & Enemy2
	enemy3_load();				// Enemy3
	boss_load();				// Boss
	portal_load();				// Portal
	upgrades_load();			// Upgrades
	player_load();				// Player
	
}
/*!**************************************************************************************************
\brief
  Initialise all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerInit(void) {

	player_init();				// Player
	draw_level_init();			// Level
	enemies_init();				// Enemy1 & Enemy2
	enemy3_init();				// Enemy3
	boss_init();				// Boss
	portal_init();				// Portal
	upgrades_init();			// Upgrades
	camera_init();				//camera, must be initialized after portal_init as some values from
							//portal_init are used
	
	
}
/*!**************************************************************************************************
\brief
  Updates all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerUpdate(void) {

	variables_update();  // Updating all global variables commonly used is utmost priority
	AEAudioUpdate();	 // Updates AEAudio module
	//soundGroup = AEAudioCreateGroup();

	if (isPaused) {

		// --------- Collision ---------
		for (s32 i = 9; i <= 15; i += 2) {
			if (AEInputCheckReleased(AEVK_LBUTTON) &&
				center_cursor.x >= -WINDOWLENGTH_X / 8 && center_cursor.x <= WINDOWLENGTH_X / 8 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i + WINDOWLENGTH_Y / 24) {
				if (i == 9) {
					isPaused = FALSE;				// Resume button
				}
				else if (i == 11) {
					gGameStateNext = GS_RESTART;	// Restart button
				}
				else if (i == 13) {
					gGameStateNext = GS_Settings;	// Settings button
					isPaused = FALSE;				// Resume game
				}
				else if (i == 15) {
					gGameStateNext = GS_MainMenu;	// Main menu button
					isPaused = FALSE;				// Resume game
				}
			}
		}
	}

	else {
		

		draw_level_update();		// Level
		enemies_update();			// Enemy1 & Enemy2
		enemy3_update(&player);		// Enemy3
		boss_update();				// Boss
		upgrade_update();			// Upgrade
		player_update();			// Player
		update_portal();			// Upgrade
		camera_update();
		// Lose condition, changes to lose state
		if (player.Lives == 0) gGameStateNext = GS_Lose;
	}
}
/*!**************************************************************************************************
\brief
  Draws all meshes being used for platformer game and render any texture for assets
*******************************************************************************************************/
void GameStatePlatformerDraw(void) {

	// Pre requisite setting
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Trans(&translate, origin.x, origin.y);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(background_layer1Tex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(background_layer2Tex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(background_layer3Tex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	draw_level_draw();		// Level
	enemies_draw();			// Enemy1 & Enemy2
	draw_enemy3();			// Enemy3
	boss_draw();			// Boss
	draw_portal();			// Portal
	player_draw();			// Player
	upgrade_draw();			// Upgrade


	// -------------- Pause menu --------------
	if (AEInputCheckReleased(AEVK_P)) {
		if (isPaused == FALSE) isPaused = TRUE;
		else if (isPaused == TRUE) isPaused = FALSE;
	}
	if (isPaused) {
		// --------- Make whole screen translucent ---------
		AEGfxSetTransparency(0.55f);
		AEMtx33Scale(&scale, WINDOWLENGTH_X+200, WINDOWLENGTH_Y+200);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, origin.x, origin.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxTextureSet(nullptr, 0, 0);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		// --------- Window in middle of screen ---------
		/*AEGfxSetTransparency(1.0f);
		AEMtx33Scale(&scale, WINDOWLENGTH_X/3, WINDOWLENGTH_Y/2);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, originX, originY -WINDOWLENGTH_Y/10);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/
		
		// --------- Buttons ---------
		AEGfxSetTransparency(1.0f);
		for (int i = 9; i <= 15; i+=2) {
			AEMtx33Scale(&scale, WINDOWLENGTH_X / 5, WINDOWLENGTH_Y / 12);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, origin.x, origin.y + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			if (center_cursor.x >= -WINDOWLENGTH_X / 10 && center_cursor.x <= WINDOWLENGTH_X / 10 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i + WINDOWLENGTH_Y / 24)
				AEGfxTextureSet(buttonPressed, 0, 0);
			else AEGfxTextureSet(buttonNotPressed, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}

		// --------- Texts ---------
		AEGfxPrint(Albam_fontID, (s8*)"PAUSED", -0.27f, 0.55f, 2.0f, 1, 1, 0);
		AEGfxPrint(Albam_fontID, (s8*)"RESUME", -0.11f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.12f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"SETTINGS", -0.13f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.155f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
	}
	AEGfxSetCamPosition(camera.x, camera.y);

}
/*!**************************************************************************************************
\brief
  Free all objects
*******************************************************************************************************/
void GameStatePlatformerFree(void) {

}
/*!**************************************************************************************************
\brief
  Unloads
*******************************************************************************************************/
void GameStatePlatformerUnload(void) {

	draw_level_unload();		// Level
	enemies_unload();			// Enemy1 & Enemy2
	enemy3_unload();			// Enemy3
	boss_unload();				// Boss
	portal_unload();			// Portal
	upgrades_unload();			// Upgrades
	player_unload();			// Player


	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(background_layer1Tex);
	AEGfxTextureUnload(background_layer2Tex);
	AEGfxTextureUnload(background_layer3Tex);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	// Informing the system about the loop's end
	AESysFrameEnd();
}