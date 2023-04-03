/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameState_Lose.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lee Zhi Yee (zhiyee.l@digipen.edu)
*
* Brief:
  This source file defines the functions for game state Lose screen.
==================================================================================*/
#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "GameState_Lose.hpp"
#include "Audio.hpp"
#include <iostream>

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex; 

// ---External Variables ---
extern AEVec2 origin;					// screen center coordinates
extern AEVec2 center_cursor;			// cursor coordinates
extern AEVec2 world_center_cursor;		// global cursor coordinates
extern s8 Albam_fontID;					// text font
//extern Player_stats player;

// --- Buttons ---
extern f32 button_scaleX;				// width of button
extern f32 button_scaleY;				// height of button
AEVec2 Lbutton;							// position of top button
AEVec2 Lbutton2;						// position of bottom button

// --- Audio ---
extern AEAudio defeatAudio, buttonClickedAudio, buttonHoverAudio;
extern AEAudioGroup soundGroup, musicGroup;
static bool LisPressed1, LisPressed2;
extern AEVec2 button_offset;
extern float barscalex, barscaley;

/*!****************************************************************************************************
\brief
	Loads textures
*******************************************************************************************************/
void GameStateLoseLoad() {

	// load textures
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");
	mesh_load();
}

/*!****************************************************************************************************
\brief
	Initializes variables and plays victory audio
*******************************************************************************************************/
void GameStateLoseInit() {
	// get cursor position
	variables_update();

	Lbutton = {AEGfxGetWinMinX() + WINDOWLENGTH_X/2.f, AEGfxGetWinMinY() + WINDOWLENGTH_Y/2.f};
	Lbutton2 = { AEGfxGetWinMinX() + WINDOWLENGTH_X / 2.f, AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2.f - 100.f};

	// audio volume controlled by settings
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

	// Defeated audio
	AEAudioPlay(defeatAudio, soundGroup, 0.25f, 1.f, 0);

}

/*!****************************************************************************************************
\brief
	Collision detection
*******************************************************************************************************/
void GameStateLoseUpdate() {
	// get cursor position
	variables_update();

	// button collision, game statw switching
	if (AETestPointToRect(&world_center_cursor, &Lbutton, button_scaleX, button_scaleY) && AEInputCheckReleased(AEVK_LBUTTON)) {
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
		gGameStateNext = GS_Platformer;

	}
	if (AETestPointToRect(&world_center_cursor, &Lbutton2, button_scaleX, button_scaleY) && AEInputCheckReleased(AEVK_LBUTTON)) {
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
		gGameStateNext = GS_MainMenu;

	}
}

/*!****************************************************************************************************
\brief
	Draws game state
*******************************************************************************************************/
void GameStateLoseDraw() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEGfxTextureSet(backgroundTex, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, NULL);

	// ------- Drawing of mesh + Setting texture -------

	// ------ top button -----
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
	drawMesh(AEVec2{ button_scaleX, button_scaleY }, Lbutton, NULL);

	// ----- bottom button ------
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
	drawMesh(AEVec2{ button_scaleX, button_scaleY }, Lbutton2, NULL);


	// ------ Texts ------
	AEGfxPrint(Albam_fontID, (s8*)"YOU LOSE", -0.43f, 0.4f, 3.0F, 1.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.115f, -0.03f, 0.95F, 1.0f, 1.0f, 1.0f);
	AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.13f, -0.25f, 0.95F, 1.0f, 1.0f, 1.0f);
}

/*!****************************************************************************************************
\brief
	Frees any data if required
*******************************************************************************************************/
void GameStateLoseFree() {

}

/*!****************************************************************************************************
\brief
	Unloads textures
*******************************************************************************************************/
void GameStateLoseUnload() {

	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	AESysFrameEnd();
}