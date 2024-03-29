/*==================================================================================
* All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Setings.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lee Zhi Yee (zhiyee.l@digipen.edu)
*
* Brief:
  This source file defines functions for implementing the Settings Menu.
==================================================================================*/
#include "AEEngine.h"
#include "Utilities.hpp"
#include "Settings.hpp"
#include "Audio.hpp"


// --- Mesh ---
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;
AEGfxTexture* volume_bar, * volume_button;

// --- Utilities ---
extern AEVec2 origin;				// center coordinates of screen
extern AEVec2 center_cursor;		// cursor coordinates 
extern AEVec2 world_center_cursor;	// for calculating button offset

// --- Audio ---
extern AEAudio buttonClickedAudio, buttonHoverAudio;
extern AEAudioGroup soundGroup, musicGroup;
static bool SisPressed1, SisPressed2;

// --- Settings variables ---
bool fullscreen = { false };					// game launches in non-fullscreen mode
f32 Sbutton_scaleX{ WINDOWLENGTH_X / 5 };		// width of menu button
f32 Sbutton_scaleY{ WINDOWLENGTH_Y / 12 };		// height of menu button
extern s8 Albam_fontID;							// text font

// --- Audio slider ----
AEVec2 vbutton, vbar;				// vector for volume button & bar coordinates
float buttonscalex, buttonscaley;	// x and y scale of volume button
float barscalex, barscaley;			// x and y scale of volume bar
bool volume_adjusted;				// if TRUE, volume has been adjusted by player
AEVec2 button_offset;				// distance offset of button from bar center
f64 vert_pos_offset{ 150.f };		// y-position of volume bar to above origin.y by 150.f

/*!****************************************************************************************************
\brief
	Loads all settings related texture
*******************************************************************************************************/
void settings_load(void) {
	// texture loading 

	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");

	volume_bar = AEGfxTextureLoad("Assets/volume_bar.png");
	volume_button = AEGfxTextureLoad("Assets/slider_button.png");
}

/*!****************************************************************************************************
\brief
	Initializes all settings related values
*******************************************************************************************************/
void settings_init(void) {
	// initialize volume button coordinates and scale
	vbutton.x = origin.x;
	vbutton.y = origin.y + static_cast<f32>(vert_pos_offset);
	buttonscalex = WINDOWLENGTH_X / 27.f;
	buttonscaley = WINDOWLENGTH_Y / 18.f;

	// initialize volume bar coordinates and scale
	vbar.x = origin.x;
	vbar.y = origin.y + static_cast<f32>(vert_pos_offset);
	barscalex = WINDOWLENGTH_X / 2.f;
	barscaley = WINDOWLENGTH_Y / 80.f;
}

/*!****************************************************************************************************
\brief
	Updates collision detection and drawing variables
*******************************************************************************************************/
void settings_update(void) {

	// get cursor coordinates
	variables_update();

	// if full screen button pressed
	if (AETestPointToRect(&world_center_cursor, &origin, Sbutton_scaleX, Sbutton_scaleY) && AEInputCheckReleased(AEVK_LBUTTON)) {
		fullscreen = !fullscreen;
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);

		AESysToggleFullScreen(fullscreen);
		
	}
	

	// update volume bar position according to origin coordinates
	vbar.x = origin.x;
	vbar.y = origin.y + static_cast<f32>(vert_pos_offset);
}

/*!****************************************************************************************************
\brief
	Draws settings menu based on updated variables 
*******************************************************************************************************/
void settings_draw(void) {
	// get cursor and center origin coordinates
	variables_update();
	 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);

	// ------- Background -------
	AEGfxTextureSet(backgroundTex, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, NULL);

	// ------------------------- Drawing of mesh + Setting texture -------------------------

	// ---- Full screen button ----
	if (AETestPointToRect(&world_center_cursor, &origin, Sbutton_scaleX, Sbutton_scaleY)) {
		AEGfxTextureSet(buttonPressed, 0, 0);
		if (SisPressed1 == FALSE) {
			SisPressed1 = TRUE;
			AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
		}
	}
	else {
		AEGfxTextureSet(buttonNotPressed, 0, 0);
		SisPressed1 = FALSE;
	}
	drawMesh(AEVec2{ Sbutton_scaleX, Sbutton_scaleY }, AEVec2{ origin.x, origin.y}, NULL);

	// ---- Back button ----
	AEVec2 backButton{ origin.x, origin.y - 400.f };

	if (AETestPointToRect(&world_center_cursor, &backButton, Sbutton_scaleX, Sbutton_scaleY)) {
		AEGfxTextureSet(buttonPressed, 0, 0);
		if (SisPressed2 == FALSE) {
			SisPressed2 = TRUE;
			AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
		}
	}
	else {
		AEGfxTextureSet(buttonNotPressed, 0, 0);
		SisPressed2 = FALSE;
	}
	drawMesh(AEVec2{ Sbutton_scaleX, Sbutton_scaleY }, AEVec2{ backButton.x, backButton.y }, NULL);

	// ------ Volume slider -------

	// updates volume button position and corresponding volume level
	if (AETestPointToRect(&center_cursor, &vbutton, barscalex, barscaley * 5) && AEInputCheckReleased(AEVK_LBUTTON)) {

		volume_adjusted = TRUE;
		AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);

		// calculate offset distance
		AEVec2Sub(&button_offset, &world_center_cursor, &origin);

		// set volume according to button offset distance from volume bar center
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
	}

	// draw volume bar
	AEGfxTextureSet(volume_bar, 0, 0);
	drawMesh(AEVec2{ barscalex, barscaley }, AEVec2{ vbar.x, vbar.y }, 0);

	// draw default button position
	if (volume_adjusted == FALSE) {
		AEGfxTextureSet(volume_button, 0, 0);
		drawMesh(AEVec2{ buttonscalex, buttonscaley }, AEVec2{ origin.x, origin.y + (f32)vert_pos_offset }, 0);
	}

	// draw updated volume button position if volume_adjusted == TRUE
	AEGfxTextureSet(volume_button, 0, 0);
	drawMesh(AEVec2{ buttonscalex, buttonscaley }, AEVec2{ origin.x + button_offset.x, origin.y + (f32)vert_pos_offset }, 0);

	// text on buttons
	AEGfxPrint(Albam_fontID, (s8*)"FULL SCREEN", (f32) - 0.12,(f32) -0.03, (f32)0.75F, (f32)1,(f32) 1, (f32)1);
	AEGfxPrint(Albam_fontID, (s8*)"BACK", (f32) - 0.07, (f32) - 0.91, (f32)0.75F, (f32)1, (f32)1, (f32)1);
}

/*!****************************************************************************************************
\brief
	Unloads all settings related texture
*******************************************************************************************************/
void settings_unload(void) {

	// Button texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);
	
	// Volume texture unload
	AEGfxTextureUnload(volume_bar);
	AEGfxTextureUnload(volume_button);
}