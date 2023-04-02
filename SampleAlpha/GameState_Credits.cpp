/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameState_Credits.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Tay Zhun Hang (zhunhang.tay@digipen.edu)
*
* Brief:
  This source file implements the functions used for the credits.
==================================================================================*/
#include "AEEngine.h"
#include "GameState_Credits.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"

#define NUM 4
extern AEGfxVertexList* square_mesh;
extern AEMtx33 scale, rotate, translate, transform;
extern AEVec2 origin;
extern s8 Albam_fontID;
credit_stat credits[NUM];
static f64 timer;

// Load all textures and meshes
void GameStateCreditsLoad(void) {
	credits[0].texture = AEGfxTextureLoad("Assets/Digipen_flipped.png");
	credits[1].texture = AEGfxTextureLoad("Assets/Credits_1.png");
	credits[2].texture = AEGfxTextureLoad("Assets/Credits_2.png");
	credits[3].texture = AEGfxTextureLoad("Assets/Credits_3.png");
	mesh_load();
}

// Initialise variables and set background to black
void GameStateCreditsInit(void) {
	AEGfxSetBackgroundColor(0, 0, 0);
	timer = 0;
	for (int i = 0; i < NUM; i++) {
		credits[i].y = origin.y - i * WINDOWLENGTH_Y;
	}
}

void GameStateCreditsUpdate(void) {
	// start timer
	timer += AEFrameRateControllerGetFrameTime();
	// check if more than 1 sec has passed
	if (timer > 1) {
		for (int i = 0; i < NUM; i++) {
			credits[i].y += 1.f; // moves up the credits
			if (credits[3].y >= (origin.y + WINDOWLENGTH_Y)) { // checks if the last credits picture is gone
				AEGfxPrint(Albam_fontID, (s8*)"Click anywhere to return to main menu", -0.3f, -0.9f, 0.5f, 1.f, 1.f, 1.f);
				if (AEInputCheckTriggered(AEVK_LBUTTON)) gGameStateNext = GS_MainMenu; // return to main menu if leftclick is pressed
			}
		}
	}

}
// draw all the credit images
void GameStateCreditsDraw(void) {
	
	for (int i = 0; i < NUM; i++) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTintColor(1, 1, 1, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Trans(&translate, origin.x, credits[i].y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(credits[i].texture, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
// free function
void GameStateCreditsFree(void) {

}
// unloads all textures and free meshes
void GameStateCreditsUnload(void) {
	for (int i = 0; i < NUM; i++) {
		AEGfxTextureUnload(credits[i].texture);
	}
	AEGfxMeshFree(square_mesh);
	AESysFrameEnd();
}
