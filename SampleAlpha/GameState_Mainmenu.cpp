/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					GameState_MainMenu.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lee Zhi Yee (zhiyee.l@digipen.edu)
*
* Brief:
  This source file implements the game state functions for the Main Menu.
==================================================================================*/
#include "AEEngine.h"
#include "GameState_Mainmenu.hpp"
#include "Settings.hpp"
#include "GameStateManager.hpp"
#include "GameStateList.hpp"
#include "Utilities.hpp"
#include "Audio.hpp"
#include <iostream>

// ---- Graphics -----
extern s8 Albam_fontID;
extern AEMtx33 scale, rotate, translate, transform;
static AEGfxTexture* buttonNotPressed, * buttonPressed, *backgroundTex, * quitWindow;
float quit_scaleX{ WINDOWLENGTH_X / 2.f };
float quit_scaleY{ WINDOWLENGTH_Y / 1.2f };

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh

// ----- Cursor positions -----
extern AEVec2 cursor;				// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;		// Origin is CENTER of window
extern AEVec2 world_center_cursor;	// Origin is CENTER of window

// ---- Drawing Variables ----
float menu_button_scaleX{ WINDOWLENGTH_X / 5 };		// Width of menu button
float menu_button_scaleY{ WINDOWLENGTH_Y / 12 };	// Height of menu button
float button_leftEdge{ -WINDOWLENGTH_X / 10 };		// X coordinate of left edge of menu button
float button_rightEdge{ WINDOWLENGTH_X / 10 };		// X coordinate of right edge of menu button
float button_startY{ WINDOWLENGTH_Y / 2 };			// Buttons drawn top down from this value above origin.y
float button_Yunit{ WINDOWLENGTH_Y / 29 };			// Buttons' y-coordinates are separated by multiples of this unit
int first_multiple{ 15 };							// First multiple of button_Yunit
int multiple_increment{ 3 };						// Increment of multiples of button_Yunit
int button_count{ 5 };								// 5 menu buttons

// ---- Quit Confirmation ----
bool isQuitting;					// if TRUE, player has clicked on "Quit Game" button

// --- Audio ---
extern AEAudio buttonClickedAudio, buttonHoverAudio, gameBGM;
extern AEAudioGroup soundGroup, musicGroup;
static bool mm_isPressed1, mm_isPressed2, mm_isPressed3, mm_isPressed4, mm_isPressed5;


// Maybe removed soon if bug fixed (WIP: Restart game & Set camera to default when BACK TO MAIN MENU)
extern AEVec2 origin; // origin (0,0) is in middle of screen, no matter where the camera moves

// ---- Settings Menu ---- 
bool is_Settings{ FALSE }; // if TRUE, screen is currently showing settings menu

void GameStateMainmenuLoad(void) {

	// Texture load
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest_resized.png");
	quitWindow = AEGfxTextureLoad("Assets/card.png");

	
	// Settings texture load
	settings_load();
	// Audio load
	audio_load();

	mesh_load();
	
}

void GameStateMainmenuInit(void) {

	// Initialize settings menu variables
	settings_init();
	// Initialize audio
	audio_init();

	// --- Game BGM ---
	AEAudioPlay(gameBGM, musicGroup, 0.25f, 1.f, -1);

}

void GameStateMainmenuUpdate(void) {

	variables_update();			// Updating all global variables commonly used is utmost priority

	if (is_Settings == FALSE)	// Settings menu buttons & Main Menu buttons located
								// in same locations, so collision should carry out main menu functions when is_Settings disabled
								// and settings functions when is_Settings enabled
	{
		for (int i = first_multiple; i <= first_multiple + (multiple_increment * (button_count - 1)); i += multiple_increment) {
			if ((AEInputCheckReleased(AEVK_LBUTTON) &&
				is_Settings == FALSE &&
				(center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - button_Yunit * i - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - button_Yunit * i + menu_button_scaleY / 2))
				&& isQuitting == FALSE) {

				if (i == first_multiple) {
					gGameStateNext = GS_Platformer;

					// Stop all audio
					AEAudioStopGroup(soundGroup);
					AEAudioStopGroup(musicGroup);
					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == first_multiple + multiple_increment) {
					gGameStateNext = GS_Tutorial;

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == first_multiple + multiple_increment * 2) {
					is_Settings = TRUE;
					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == first_multiple + multiple_increment * 3) {
					gGameStateNext = GS_Credits;
					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == first_multiple + multiple_increment * 4) {
					isQuitting = TRUE;

					// Stop all audio
					AEAudioStopGroup(soundGroup);
					AEAudioStopGroup(musicGroup);
					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}

			}
		}

		if (isQuitting == TRUE) {
			for (int i = first_multiple+multiple_increment*2; i <= first_multiple + (multiple_increment * (button_count - 2)); i += multiple_increment) {
				if (AEInputCheckReleased(AEVK_LBUTTON) &&
					is_Settings == FALSE &&
					(center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
						center_cursor.y >= button_startY - button_Yunit * i - menu_button_scaleY / 2 &&
						center_cursor.y <= button_startY - button_Yunit * i + menu_button_scaleY / 2)) {

					
					if (i == first_multiple + multiple_increment * 2) {
						isQuitting = FALSE;

						AEAudioPlay(gameBGM, musicGroup, 0.25f, 1.f, -1);
						// Audio once button is pressed
						AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
					}
					else if (i == first_multiple + multiple_increment * 3) {
						gGameStateNext = GS_QUIT;
						// Audio once button is pressed
						AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
					}
				}
			}
		}
	
	}


}

void GameStateMainmenuDraw(void) {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEGfxTextureSet(backgroundTex, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

	// ------- Drawing of mesh + Setting texture -------
	for (int i = first_multiple; i <= first_multiple + (multiple_increment*(button_count-1)) ; i+=multiple_increment) {
	
		if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
			center_cursor.y >= button_startY - button_Yunit * i - menu_button_scaleY / 2 &&
			center_cursor.y <= button_startY - button_Yunit * i + menu_button_scaleY / 2)
			&& is_Settings == FALSE
			&& isQuitting == FALSE)
			AEGfxTextureSet(buttonPressed, 0, 0);

		else AEGfxTextureSet(buttonNotPressed, 0, 0);
	
		drawMesh(AEVec2{ menu_button_scaleX, menu_button_scaleY }, AEVec2{origin.x, origin.y + button_startY - button_Yunit * i}, PI);

		// ----- Audio -----

	// 1st button from top
		if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
			center_cursor.y >= button_startY - (button_Yunit * first_multiple) - menu_button_scaleY / 2 &&
			center_cursor.y <= button_startY - (button_Yunit * first_multiple) + menu_button_scaleY / 2)
			&& is_Settings == FALSE
			&& isQuitting == FALSE)
		{
			if (mm_isPressed1 == FALSE) {
				mm_isPressed1 = TRUE;
				// Hovering over button sfx
				AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
			}
		}
		else {
			mm_isPressed1 = FALSE;
		}
		

		// 2nd button from top
		if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
			center_cursor.y >= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 1))) - menu_button_scaleY / 2 &&
			center_cursor.y <= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 1))) + menu_button_scaleY / 2)
			&& is_Settings == FALSE
			&& isQuitting == FALSE)
		{
			if (mm_isPressed2 == FALSE) {
				mm_isPressed2 = TRUE;
				// Hovering over button sfx
				AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
			}
		}
		else {
			mm_isPressed2 = FALSE;
		}
		

		// 3rd button from top
		if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
			center_cursor.y >= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 2))) - menu_button_scaleY / 2 &&
			center_cursor.y <= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 2))) + menu_button_scaleY / 2)
			&& is_Settings == FALSE)
		{
			if (mm_isPressed3 == FALSE) {
				mm_isPressed3 = TRUE;
				// Hovering over button sfx
				AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
			}
		}
		else {
			mm_isPressed3 = FALSE;
		}
		

		// 4th button from top
		if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
			center_cursor.y >= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 3))) - menu_button_scaleY / 2 &&
			center_cursor.y <= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 3))) + menu_button_scaleY / 2)
			&& is_Settings == FALSE)
		{
			if (mm_isPressed4 == FALSE) {
				mm_isPressed4 = TRUE;
				// Hovering over button sfx
				AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
			}
		}
		else {
			mm_isPressed4 = FALSE;
		}
		

		// 5th button from top
		if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
			center_cursor.y >= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 4))) - menu_button_scaleY / 2 &&
			center_cursor.y <= button_startY - (button_Yunit * (first_multiple + (multiple_increment * 4))) + menu_button_scaleY / 2)
			&& is_Settings == FALSE
			&& isQuitting == FALSE)
		{
			if (mm_isPressed5 == FALSE) {
				mm_isPressed5 = TRUE;
				// Hovering over button sfx
				AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
			}
		}
		else {
			mm_isPressed5 = FALSE;
		}
		
	}

	

	// ------ Texts ------
	// redoooooooooooooooooooooooooooooooooooo
	float first_text = -0.0583f;
	AEGfxPrint(Albam_fontID, (s8*)"PORTALS", -0.4, 0.15, 3.0F, 1, 1, 0);
	AEGfxPrint(Albam_fontID, (s8*)"START GAME", -0.125, first_text , 0.75f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"TUTORIAL", -0.097, first_text - 0.21f, 0.75f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"SETTINGS", -0.097, first_text - 0.41f, 0.75f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"CREDITS", -0.085, first_text - 0.62f, 0.75f, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"QUIT GAME", -0.11, first_text - 0.82f, 0.75f, 1, 1, 1);

	if (is_Settings == TRUE) {
		settings_draw();	// Draws menu to screen
		settings_update();	// Updates collision within menu

		// if "Back" button pressed, exit settings menu
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			center_cursor.x >= -WINDOWLENGTH_X / 6.f && center_cursor.x <= WINDOWLENGTH_X / 6.f &&
			center_cursor.y >= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 - WINDOWLENGTH_Y / 16.f &&
			center_cursor.y <= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 + WINDOWLENGTH_Y / 16.f) {
			is_Settings = FALSE;
		}
	}

	if (isQuitting == TRUE) {
		// --------- Make whole screen translucent ---------
		AEGfxSetTransparency(0.55f);
		AEGfxTextureSet(nullptr, 0, 0);
		drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

		AEGfxSetTransparency(1.f);

		AEGfxTextureSet(quitWindow, 0, 0);
		drawMesh(AEVec2{ quit_scaleX, quit_scaleY }, origin, PI);

		for (int i = first_multiple + multiple_increment * 2; i <= first_multiple + (multiple_increment * (button_count - 2)); i += multiple_increment) {

			if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - button_Yunit * i - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - button_Yunit * i + menu_button_scaleY / 2)
				&& is_Settings == FALSE)
				AEGfxTextureSet(buttonPressed, 0, 0);

			else AEGfxTextureSet(buttonNotPressed, 0, 0);

			drawMesh(AEVec2{ menu_button_scaleX, menu_button_scaleY }, AEVec2{ origin.x, origin.y + button_startY - button_Yunit * i }, PI);

			AEGfxPrint(Albam_fontID, (s8*)"Are you sure", -0.12, first_text + 0.65f, 0.75f, 0, 0, 0);
			AEGfxPrint(Albam_fontID, (s8*)"you want to quit", -0.18, first_text + 0.5f, 0.75f, 0, 0, 0);
			AEGfxPrint(Albam_fontID, (s8*)"the game?", -0.1, first_text + 0.35f , 0.75f, 0, 0, 0);

			AEGfxPrint(Albam_fontID, (s8*)"BACK", -0.055, first_text - 0.41f, 0.75f, 1, 1, 1);
			AEGfxPrint(Albam_fontID, (s8*)"QUIT GAME", -0.11, first_text - 0.62f, 0.75f, 1, 1, 1);
		}
	}
}
void GameStateMainmenuFree() {
	
}

void GameStateMainmenuUnload(void) {
	// Settings Texture unload
	settings_unload(); 
	
	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);
	AEGfxTextureUnload(quitWindow);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	AESysFrameEnd();
}