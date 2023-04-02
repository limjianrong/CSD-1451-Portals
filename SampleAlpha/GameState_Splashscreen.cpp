/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameState_Splashscreen.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Tay Zhun Hang (zhunhang.tay@digipen.edu)
*
* Brief:
  This source file implements the functions used for the splash screen.
==================================================================================*/
#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"
#include <iostream>

AEGfxTexture* logo;
extern AEGfxVertexList* square_mesh;
extern AEMtx33 scale, rotate, translate, transform;
extern AEVec2 origin;
static f32 transparancy;
static f64 timer;

void GameStateSplashscreenLoad(void) {
	mesh_load();
	logo = AEGfxTextureLoad("Assets/Digipen_flipped.png");

}
void GameStateSplashscreenInit(void) {
	timer = 0;
	transparancy = 1.0f;
	AEGfxSetBackgroundColor(0, 0, 0);
}

void GameStateSplashscreenUpdate(void) {

	timer += AEFrameRateControllerGetFrameTime();

	if (timer >= 2) {
		transparancy -= 0.3f;
		if (transparancy < 0.f) gGameStateNext = GS_MainMenu;
	}

}

void GameStateSplashscreenDraw(void) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(transparancy);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxTextureSet(logo, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

}
void GameStateSplashscreenFree(void) {

}
void GameStateSplashscreenUnload(void) {
	AEGfxTextureUnload(logo);
	AEGfxMeshFree(square_mesh);
	AESysFrameEnd();
}
