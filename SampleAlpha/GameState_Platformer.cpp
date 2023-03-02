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

// ----- Enemies related -----
#include "Enemy.hpp"
#include "Enemy3.hpp"
#include "boss.hpp"

// ----- Others -----
#include "Utilities.hpp"
#include "draw_level.hpp"
#include "Upgrades.hpp"


//#include <iostream>
extern AEGfxVertexList* pMesh; // Mesh
extern AEMtx33 scale, rotate, translate, transform; // TRS
extern s8 Albam_fontID; // FontID
extern AEGfxTexture* buttonNotPressed, * buttonPressed; // Button texture
extern Player_stats player; // player stats
AEGfxTexture* background2Tex; // Background texture
f32 originX, originY; // origin (0,0) is in middle of screen, no matter where the camera moves
bool isPaused;
/*!**************************************************************************************************
\brief
  In charge of loading platformer game
*******************************************************************************************************/
void GameStatePlatformerLoad(void) {
	background2Tex = AEGfxTextureLoad("Assets/background/Backgrounds/backgroundColorFall.png");

	initialize_player();
	initialize_portal();
	draw_level_init();
	enemy_init();
	enemy3_init();
	initialize_boss();
	upgrades_init();
}
/*!**************************************************************************************************
\brief
  Initialise all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerInit(void) {

}
/*!**************************************************************************************************
\brief
  Updates all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerUpdate(void) {
	
	// --------- Gets origin (0,0) of constantly moving screen ---------
	originX = AEGfxGetWinMinX() + WINDOWLENGTH_X / 2;
	originY = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2;

	if (isPaused) {

		// --------- Collision ---------
		// Resume button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			get_cursor_center_position().x >= -WINDOWLENGTH_X / 8 && get_cursor_center_position().x <= WINDOWLENGTH_X / 8 &&
			get_cursor_center_position().y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 - WINDOWLENGTH_Y / 24 &&
			get_cursor_center_position().y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 + WINDOWLENGTH_Y / 24) {
			isPaused = FALSE;
		}
		// Restart button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			get_cursor_center_position().x >= -WINDOWLENGTH_X / 8 && get_cursor_center_position().x <= WINDOWLENGTH_X / 8 &&
			get_cursor_center_position().y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 - WINDOWLENGTH_Y / 24 &&
			get_cursor_center_position().y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 + WINDOWLENGTH_Y / 24) {
			gGameStateNext = GS_RESTART;
			//isPaused = FALSE;
		}
		// Settings button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			get_cursor_center_position().x >= -WINDOWLENGTH_X / 8 && get_cursor_center_position().x <= WINDOWLENGTH_X / 8 &&
			get_cursor_center_position().y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 - WINDOWLENGTH_Y / 24 &&
			get_cursor_center_position().y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 + WINDOWLENGTH_Y / 24) {
			gGameStateNext = GS_Settings;
			isPaused = FALSE;
		}
		// Main menu button
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			get_cursor_center_position().x >= -WINDOWLENGTH_X / 8 && get_cursor_center_position().x <= WINDOWLENGTH_X / 8 &&
			get_cursor_center_position().y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 - WINDOWLENGTH_Y / 24 &&
			get_cursor_center_position().y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 + WINDOWLENGTH_Y / 24) {
			gGameStateNext = GS_MainMenu;
			isPaused = FALSE;
		}
	}

	else {
		update_player();
		update_level();
		enemy3_update(&player);
		update_enemy();
		update_boss();
		move_update();
		update_upgrade_cards();
		
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
	AEMtx33Scale(&scale, WINDOWLENGTH_X *10, WINDOWLENGTH_Y * 10);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(background2Tex, 0, 0);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	draw_boss();
	draw_player();
	draw_enemy();
	draw_level();
	draw_enemy3();
	draw_upgrade_cards();


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
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// --------- Window in middle of screen ---------
		AEGfxSetTransparency(1.0f);
		AEMtx33Scale(&scale, WINDOWLENGTH_X/3, WINDOWLENGTH_Y/2);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, originX, originY -WINDOWLENGTH_Y/10);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		
		// --------- Buttons ---------
		for (int i = 9; i <= 15; i+=2) {
			AEMtx33Scale(&scale, WINDOWLENGTH_X / 4, WINDOWLENGTH_Y / 12);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, originX, originY + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			if (get_cursor_center_position().x >= -WINDOWLENGTH_X / 8 && get_cursor_center_position().x <= WINDOWLENGTH_X / 8 &&
				get_cursor_center_position().y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i - WINDOWLENGTH_Y / 24 &&
				get_cursor_center_position().y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i + WINDOWLENGTH_Y / 24)
				AEGfxTextureSet(buttonPressed, 0, 0);
			else AEGfxTextureSet(buttonNotPressed, 0, 0);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}

		// --------- Texts ---------
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
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

	//unload_player();
	// 
	// Informing the system about the loop's end
	AESysFrameEnd();
}