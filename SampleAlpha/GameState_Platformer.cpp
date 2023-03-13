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

// ----- Others -----
#include "Utilities.hpp"
#include "draw_level.hpp"

//#include <iostream>

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
extern AEMtx33 scale, rotate, translate, transform; // TRS
extern s8 Albam_fontID; // FontID
extern AEGfxTexture* buttonNotPressed, * buttonPressed; // Button texture
AEGfxTexture* background2Tex; // Background texture

// ----- Game objects -----
extern Player_stats player; // player stats
// ----- Game states -----
bool isPaused;

// ----- Cursor positions -----
extern AEVec2 cursor;					// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;			// Origin is CENTER of window
extern AEVec2 world_center_cursor;		// Origin is CENTER of window
// ----- Window origin -----
extern f32 originX, originY;			// Center of screen, no matter where the camera moves

/*!**************************************************************************************************
\brief
  In charge of loading platformer game
*******************************************************************************************************/
void GameStatePlatformerLoad(void) {
	background2Tex = AEGfxTextureLoad("Assets/backgroundColorFall.png");
	
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
	draw_level_init();			// Level
	enemies_init();				// Enemy1 & Enemy2
	enemy3_init();				// Enemy3
	boss_init();				// Boss
	portal_init();				// Portal
	upgrades_init();			// Upgrades
	player_init();				// Player
}
/*!**************************************************************************************************
\brief
  Updates all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerUpdate(void) {

	variables_update();  // Updating all global variables commonly used is utmost priority

	if (isPaused) {

		// --------- Collision ---------
		// Resume button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 8 && center_cursor.x <= WINDOWLENGTH_X / 8 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 - WINDOWLENGTH_Y / 24 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 + WINDOWLENGTH_Y / 24) {
			isPaused = FALSE;
		}
		// Restart button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 8 && center_cursor.x <= WINDOWLENGTH_X / 8 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 - WINDOWLENGTH_Y / 24 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 + WINDOWLENGTH_Y / 24) {
			gGameStateNext = GS_RESTART;
		}
		// Settings button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 8 && center_cursor.x <= WINDOWLENGTH_X / 8 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 - WINDOWLENGTH_Y / 24 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 + WINDOWLENGTH_Y / 24) {
			gGameStateNext = GS_Settings;
			isPaused = FALSE;
		}
		// Main menu button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 8 && center_cursor.x <= WINDOWLENGTH_X / 8 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 - WINDOWLENGTH_Y / 24 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 + WINDOWLENGTH_Y / 24) {
			gGameStateNext = GS_MainMenu;
			isPaused = FALSE;
		}
	}

	else {
		update_level();				// Level
		enemies_update();			// Enemy1 & Enemy2
		enemy3_update(&player);		// Enemy3
		update_boss();				// Boss
		upgrade_update();			// Upgrade
		player_update();
		update_portal();
		
	}

	// lose condition, changes to lose state
	if (!isPaused) {
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
	AEMtx33Scale(&scale, WINDOWLENGTH_X + 20, WINDOWLENGTH_Y + 20);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	//AEGfxTextureSet(nullptr, 0, 0);
	AEGfxTextureSet(background2Tex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	draw_level();		// Level
	enemies_draw();		// Enemy1 & Enemy2
	draw_enemy3();		// Enemy3
	draw_boss();		// Boss
	draw_portal();		// Portal
	player_draw();		// Player
	upgrade_draw();		// Upgrade


	// -------------- Pause menu --------------
	if (AEInputCheckReleased(AEVK_P)) {
		if (isPaused == FALSE) isPaused = TRUE;
		else if (isPaused == TRUE) isPaused = FALSE;
	}
	if (isPaused) {
		// --------- Make whole screen translucent ---------
		AEGfxSetTransparency(0.55f);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, originX, originY);
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
			AEMtx33Scale(&scale, WINDOWLENGTH_X / 4, WINDOWLENGTH_Y / 12);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, originX, originY + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			if (center_cursor.x >= -WINDOWLENGTH_X / 8 && center_cursor.x <= WINDOWLENGTH_X / 8 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i + WINDOWLENGTH_Y / 24)
				AEGfxTextureSet(buttonPressed, 0, 0);
			else AEGfxTextureSet(buttonNotPressed, 0, 0);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}

		// --------- Texts ---------
		AEGfxPrint(Albam_fontID, (s8*)"PAUSED", -0.35f, 0.55f, 2.0f, 1, 1, 0);
		AEGfxPrint(Albam_fontID, (s8*)"RESUME", -0.17f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.90f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.17f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.90f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"SETTINGS", -0.18f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.90f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.21f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.90f, 1, 1, 1);
	}

}
/*!**************************************************************************************************
\brief
  Free all objects
*******************************************************************************************************/
void GameStatePlatformerFree(void) {
	//AEGfxMeshFree(pMesh);
	//AEGfxTextureUnload(pTex);
}
/*!**************************************************************************************************
\brief
  Unloads
*******************************************************************************************************/
void GameStatePlatformerUnload(void) {

	//player_unload();
	// 
	// Informing the system about the loop's end
	AESysFrameEnd();
}