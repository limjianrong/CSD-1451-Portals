#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "Settings.hpp"
#include "GameState_Lose.hpp"
#include "GameState_Mainmenu.hpp"
#include "Player.hpp"
#include "Tutorial.hpp"
#include "Audio.hpp"
#include <iostream>

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex; 

// ---External Variables ---
extern AEMtx33 scale, rotate, translate, transform;
extern AEVec2 origin; // screen center coordinates
extern AEVec2 center_cursor; // cursor coordinates
extern AEVec2 world_center_cursor;
extern s8 Albam_fontID; // text font
extern Player_stats player;

// --- Buttons ---
extern f32 button_scaleX;		// width of button
extern f32 button_scaleY;		// height of button
AEVec2 Lbutton;
AEVec2 Lbutton2;

// --- Audio ---
extern AEAudio defeatAudio, buttonClickedAudio, buttonHoverAudio;
extern AEAudioGroup soundGroup;
static bool LisPressed1, LisPressed2;

void GameStateLoseLoad() {

	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");
	mesh_load();
}

void GameStateLoseInit() {
	variables_update();

	Lbutton = {AEGfxGetWinMinX() + WINDOWLENGTH_X/2.f, AEGfxGetWinMinY() + WINDOWLENGTH_Y/2.f};
	Lbutton2 = { AEGfxGetWinMinX() + WINDOWLENGTH_X / 2.f, AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2.f - 100.f};

	// Defeated audio
	AEAudioPlay(defeatAudio, soundGroup, 0.25f, 1.f, 0);

}

void GameStateLoseUpdate() {
	// get cursor position
	variables_update();

	if (AETestPointToRect(&world_center_cursor, &Lbutton, button_scaleX, button_scaleY) && AEInputCheckReleased(AEVK_LBUTTON)) {
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
		gGameStateNext = GS_Platformer;

	}
	if (AETestPointToRect(&world_center_cursor, &Lbutton2, button_scaleX, button_scaleY) && AEInputCheckReleased(AEVK_LBUTTON)) {
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
		gGameStateNext = GS_MainMenu;

	}
}

void GameStateLoseDraw() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEGfxTextureSet(backgroundTex, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

	// ------- Drawing of mesh + Setting texture -------
	if (AETestPointToRect(&world_center_cursor, &Lbutton, button_scaleX, button_scaleY)) {
		AEGfxTextureSet(buttonPressed, 0, 0);

		if (LisPressed1 == FALSE) {
			LisPressed1 = TRUE;
			AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
		}
	}
	else {
		AEGfxTextureSet(buttonNotPressed, 0, 0);
		LisPressed1 = FALSE;
	}
	drawMesh(AEVec2{ button_scaleX, button_scaleY }, Lbutton, PI);

	if (AETestPointToRect(&world_center_cursor, &Lbutton2, button_scaleX, button_scaleY)) {
		AEGfxTextureSet(buttonPressed, 0, 0);

		if (LisPressed2 == FALSE) {
			LisPressed2 = TRUE;
			AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
		}
	}
	else {
		AEGfxTextureSet(buttonNotPressed, 0, 0);
		LisPressed2 = FALSE;
	}
	drawMesh(AEVec2{ button_scaleX, button_scaleY }, Lbutton2, PI);


	// ------ Texts ------
	AEGfxPrint(Albam_fontID, (s8*)"YOU LOSE", -0.43f, 0.4f, 3.0F, 1.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.115f, -0.03f, 0.95F, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.13f, -0.25f, 0.95F, 1.0f, 1.0f, 1.0f);
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