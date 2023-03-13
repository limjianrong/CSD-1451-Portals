#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "GameState_Settings.hpp"
#include "GameState_Win.hpp"


// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;


extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID;
extern f32 originX, originY;
extern AEVec2 center_cursor;

void GameStateWinLoad() {

	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");

}

void GameStateWinInit() {

}

void GameStateWinUpdate() {
	variables_update();

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

void GameStateWinDraw() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Trans(&translate, originX, originY);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(backgroundTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// ------- Drawing of mesh + Setting texture -------
	for (int i = 15; i <= 19; i += 4) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 3, WINDOWLENGTH_Y / 8); // scaling it up
		AEMtx33Trans(&translate, originX, originY + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 30 * i); // x=0, start counting y from bottom edge of screen
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
	AEGfxPrint(Albam_fontID, (s8*)"YOU WIN!", -0.55, 0.4, 3.0F, 1, 1, 0);
	AEGfxPrint(Albam_fontID, (s8*)"Restart", -0.25, -0.05, 0.95F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"MainMenu", -0.19, -0.3, 0.95F, 1, 1, 1);
}

void GameStateWinFree() {

}
void GameStateWinUnload() {

	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	AESysFrameEnd();
}