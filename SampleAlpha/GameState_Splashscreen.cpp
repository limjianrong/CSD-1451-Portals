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
#include "GameState_Splashscreen.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"

AEGfxTexture* logo;
extern AEGfxVertexList* square_mesh;
extern AEMtx33 scale, rotate, translate, transform;
extern AEVec2 origin;
static f32 transparency;
static f64 timer;

// load texture and mesh
void GameStateSplashscreenLoad(void) {
	mesh_load();
	logo = AEGfxTextureLoad("Assets/Digipen.png");

}
// initialise timer and transparancy variables and set background colour to black
void GameStateSplashscreenInit(void) {
	timer = 0;
	transparency = 1.0f;
	AEGfxSetBackgroundColor(0, 0, 0);
}
// update function
void GameStateSplashscreenUpdate(void) {

	timer += AEFrameRateControllerGetFrameTime(); //starts timer
	//checks if 2 seconds or more have passed
	if (timer >= 2) {
		transparency -= TRANSPARENCY_DECREMENT;//start decrementing the transparency
		if (transparency < 0.f) gGameStateNext = GS_MainMenu; //check if transparancy is less than 0, if so, change gamestate to main menu
	}

}
// draw function to draw the splash screen
void GameStateSplashscreenDraw(void) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(transparency);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxTextureSet(logo, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, NULL);

}
// free function
void GameStateSplashscreenFree(void) {

}
// unload function that unload all textures and free meshes
void GameStateSplashscreenUnload(void) {
	AEGfxTextureUnload(logo);
	AEGfxMeshFree(square_mesh);
	AESysFrameEnd();
}
