/******************************************************************************/
/*!
\file		GameState_Platformer.cpp
\author 	Digipen, Lin ZhaoZhi
\par    	email: z.lin@digipen.edu
\date   	30 January, 2023
\brief		Source file for the platformer game state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "AEEngine.h"
#include "Player.hpp"
#include "Utilities.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "draw_level.hpp"
#include "Enemy.hpp"
#include "Main_menu.hpp"

/*!**************************************************************************************************
\brief
  In charge of loading platformer game
*******************************************************************************************************/
void GameStatePlatformerLoad(void) {
	initialize_player(200);
	initialize_portal();
	initialize_endpoint();
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

}
/*!**************************************************************************************************
\brief
  Draws all meshes being used for platformer game and render any texture for assets
*******************************************************************************************************/
void GameStatePlatformerDraw(void) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set the tint to white, so that the sprite can display the full range of colors (default is black).
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);


	draw_player(200);

	draw_enemy();

	draw_level();
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
	// Informing the system about the loop's end
	AESysFrameEnd();
}