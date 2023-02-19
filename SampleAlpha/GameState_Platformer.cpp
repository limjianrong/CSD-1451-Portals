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
#include "GameStateManager.hpp"
#include "GameStateList.hpp"

#include "Player.hpp"
#include "Utilities.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "draw_level.hpp"
#include "Enemy.hpp"

#include <iostream>
extern AEGfxVertexList* pMesh;
extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID;
bool isPaused;
AEVec2 center_cursor_paused; // cursor coords, origin is middle of screen
/*!**************************************************************************************************
\brief
  In charge of loading platformer game
*******************************************************************************************************/
void GameStatePlatformerLoad(void) {
	initialize_player();
	initialize_portal();
	draw_level_init();
	enemy_init();

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
	
	if (isPaused) {
		
		center_cursor_paused = get_cursor_center_position();
		// --------- Collision ---------
		// Restart button
		if (center_cursor_paused.x >= -100 && center_cursor_paused.x <= 100 &&
			center_cursor_paused.y >= 25 && center_cursor_paused.y <= 125 && (AEInputCheckCurr(AEVK_LBUTTON))) {
			//AEGfxSetBackgroundColor(255, 255, 255);
			gGameStateNext = GS_RESTART;
		}
		// Settings button
		if (center_cursor_paused.x >= -100 && center_cursor_paused.x <= 100 &&
			center_cursor_paused.y >= -50 && center_cursor_paused.y <= 50 && (AEInputCheckCurr(AEVK_LBUTTON))) {
			AEGfxSetBackgroundColor(0, 125, 255);
			//gGameStateNext = GS_Settings;
		}
		// Main menu button
		if (center_cursor_paused.x >= -100 && center_cursor_paused.x <= 100 &&
			center_cursor_paused.y >= -115 && center_cursor_paused.y <= -50 && (AEInputCheckCurr(AEVK_LBUTTON))) {
			gGameStateNext = GS_MainMenu;
			isPaused = FALSE;
		}
	}
	
	//if (AEInputCheckTriggered(AEVK_LBUTTON)) {
	//	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	//	gGameStateNext = GS_MainMenu;
	//}
	else {
		update_player();
		update_level();
	}


}
/*!**************************************************************************************************
\brief
  Draws all meshes being used for platformer game and render any texture for assets
*******************************************************************************************************/
void GameStatePlatformerDraw(void) {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);

	draw_player();
	draw_enemy();
	draw_level();

	// -------------- Pause menu --------------
	if (AEInputCheckTriggered(AEVK_P)) {
		if (isPaused == FALSE) isPaused = TRUE;
		else if (isPaused == TRUE) isPaused = FALSE;
	}
	if (isPaused) {
		// --------- Make whole screen translucent ---------
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//AEGfxSetTransparency(0.55f);
		AEGfxSetTintColor(0.f, 0.f, 0.f, 0.55f);
		AEMtx33Scale(&scale, WINDOWXLENGTH, WINDOWYLENGTH);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMinX() + WINDOWXLENGTH / 2, AEGfxGetWinMinY() + WINDOWYLENGTH / 2);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// --------- Window in middle of screen ---------
		AEGfxSetTintColor(255, 255, 255, 0.90f);
		AEMtx33Scale(&scale, 300, 250);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMinX() + WINDOWXLENGTH / 2, AEGfxGetWinMinY() + WINDOWYLENGTH / 2);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// --------- Buttons ---------
		// Restart button
		AEGfxSetTintColor(0, 255, 255, 1.0f);
		AEMtx33Scale(&scale, 200, 50);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMinX() + WINDOWXLENGTH / 2, AEGfxGetWinMinY() + WINDOWYLENGTH / 2 + 75);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		// Settings button
		AEMtx33Scale(&scale, 200, 50);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMinX() + WINDOWXLENGTH / 2, AEGfxGetWinMinY() + WINDOWYLENGTH / 2);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		// Main menu button
		AEMtx33Scale(&scale, 200, 65);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMinX() + WINDOWXLENGTH / 2, AEGfxGetWinMinY() + WINDOWYLENGTH / 2 - 85);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// --------- Texts ---------
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(Albam_fontID, (s8*)"PAUSED", -0.35f, 0.55f, 2.0f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.15f, 0.2f, 0.7f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(Albam_fontID, (s8*)"SETTINGS", -0.17f, -0.05f, 0.7f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(Albam_fontID, (s8*)"BACK TO", -0.12f, -0.26f, 0.55f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.15f, -0.36f, 0.55f, 0.0f, 0.0f, 0.0f);
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