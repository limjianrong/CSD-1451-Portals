#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "Settings.hpp"
#include "GameState_Lose.hpp"
#include "GameState_Mainmenu.hpp"
#include "Player.hpp"

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex; 

// ---External Variables ---
extern AEMtx33 scale, rotate, translate, transform;
extern AEVec2 origin; // screen center coordinates
extern AEVec2 center_cursor; // cursor coordinates
extern s8 Albam_fontID; // text font
extern Player_stats player;

// --- Audio ---
extern AEAudio defeatAudio;
extern AEAudioGroup soundGroup;

void GameStateLoseLoad() {

	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");
	mesh_load();
}

void GameStateLoseInit() {
}

void GameStateLoseUpdate() {
	// get cursor position
	variables_update();


	// if respective buttons clicked, go to their game states
	for (s32 i = 15; i <= 19; i += 4) {
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i + WINDOWLENGTH_Y / 16) {
			if (i == 15) gGameStateNext = GS_Platformer;
			else if (i == 19) gGameStateNext = GS_MainMenu;
		}
	}
}

void GameStateLoseDraw() {
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
	AEGfxTextureSet(backgroundTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// ------- Drawing of mesh + Setting texture -------
	for (int i = 15; i <= 19; i += 4) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 3, WINDOWLENGTH_Y / 8); // button scale
		AEMtx33Trans(&translate, origin.x, origin.y + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i); // x = center, start counting y from bottom of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		if (center_cursor.x >= -WINDOWLENGTH_X / 6 && center_cursor.x <= WINDOWLENGTH_X / 6 &&
			center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i + WINDOWLENGTH_Y / 16)
			AEGfxTextureSet(buttonPressed, 0, 0);
		else AEGfxTextureSet(buttonNotPressed, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// ------ Texts ------
	AEGfxPrint(Albam_fontID, (s8*)"YOU LOSE", -0.65f, 0.4f, 3.0F, 1.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"Restart", -0.17f, -0.05f, 0.95F, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(Albam_fontID, (s8*)"Main Menu", -0.21f, -0.3f, 0.95F, 1.0f, 1.0f, 1.0f);
}

void GameStateLoseFree() {

}
void GameStateLoseUnload() {

	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	AESysFrameEnd();
}