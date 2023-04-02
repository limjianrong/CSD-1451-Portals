/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameState_Win.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lee Zhi Yee (zhiyee.l@digipen.edu)
*
* Brief:
  This source file defines the game state functions for the Win screen.
==================================================================================*/
#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "Settings.hpp"
#include "Audio.hpp"
#include "GameState_Win.hpp"


// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;

// --- External variables ---
extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID;					//text font
extern AEVec2 origin;					// screen center coordinates
extern AEVec2 center_cursor;			// cursor coordinates
extern AEVec2 world_center_cursor;		// global cursor coordinates
	
// --- Buttons ---
extern f32 button_scaleX;				// width of button
extern f32 button_scaleY;				// height of button
AEVec2 Wbutton;							// top buttton position
AEVec2 Wbutton2;						// bottom button position
	
// --- Audio ---
extern AEAudio victoryAudio, buttonClickedAudio, buttonHoverAudio;
extern AEAudioGroup soundGroup, musicGroup;
static bool WisPressed1, WisPressed2;
extern AEVec2 button_offset;
extern float barscalex, barscaley;

void GameStateWinLoad() {

	// load textures 
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");

	mesh_load();
}

void GameStateWinInit() {
	// get cursor position
	variables_update();

	Wbutton = { AEGfxGetWinMinX() + WINDOWLENGTH_X / 2.f, AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2.f };
	Wbutton2 = { AEGfxGetWinMinX() + WINDOWLENGTH_X / 2.f, AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2.f - 100.f };

	// volume controlled by settings
	if (button_offset.x < 0.f) {
		AEAudioSetGroupVolume(soundGroup, 0.5f - (AEVec2Length(&button_offset) / barscalex));
		AEAudioSetGroupVolume(musicGroup, 0.5f - (AEVec2Length(&button_offset) / barscalex));
	}
	else if (button_offset.x > 0.f) {
		AEAudioSetGroupVolume(soundGroup, 0.5f + (AEVec2Length(&button_offset) / barscalex));
		AEAudioSetGroupVolume(musicGroup, 0.5f + (AEVec2Length(&button_offset) / barscalex));

	}
	else if (button_offset.x == 0.f) {
		AEAudioSetGroupVolume(soundGroup, 0.5f);
		AEAudioSetGroupVolume(musicGroup, 0.5f);

	}

	// Victory audio
	AEAudioPlay(victoryAudio, soundGroup, 0.25f, 1.f, 0);
}

void GameStateWinUpdate() {
	// get cursor position
	variables_update();

	// button collision, game state switching
	if (AETestPointToRect(&world_center_cursor, &Wbutton, button_scaleX, button_scaleY) && AEInputCheckReleased(AEVK_LBUTTON)) {
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
		gGameStateNext = GS_Platformer;

	}
	if (AETestPointToRect(&world_center_cursor, &Wbutton2, button_scaleX, button_scaleY) && AEInputCheckReleased(AEVK_LBUTTON)) {
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
		gGameStateNext = GS_MainMenu;

	}
}

void GameStateWinDraw() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEGfxTextureSet(backgroundTex, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

	// ------- Drawing of mesh + Setting texture -------

	// ------ top button ------
	if (AETestPointToRect(&world_center_cursor, &Wbutton, button_scaleX, button_scaleY)) {
		AEGfxTextureSet(buttonPressed, 0, 0);

		if (WisPressed1 == FALSE) {
			WisPressed1 = TRUE;
			AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
		}
	}
	else {
		AEGfxTextureSet(buttonNotPressed, 0, 0);
		WisPressed1 = FALSE;
	}
	drawMesh(AEVec2{ button_scaleX, button_scaleY },Wbutton, PI);

	// ------ bottom button ------
	if (AETestPointToRect(&world_center_cursor, &Wbutton2, button_scaleX, button_scaleY)) {
		AEGfxTextureSet(buttonPressed, 0, 0);

		if (WisPressed2 == FALSE) {
			WisPressed2 = TRUE;
			AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
		}
	}
	else {
		AEGfxTextureSet(buttonNotPressed, 0, 0);
		WisPressed2 = FALSE;
	}
	drawMesh(AEVec2{ button_scaleX, button_scaleY }, Wbutton2, PI);


	// ------ Texts ------
	AEGfxPrint(Albam_fontID, (s8*)"YOU WIN!", -0.426f, 0.4f, 3.0F, 1.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.115f, -0.03f, 0.95F, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.13f, -0.25f, 0.95F, 1.0f, 1.0f, 1.0f);
}

void GameStateWinFree() {

}
void GameStateWinUnload() {

	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	AESysFrameEnd();
}