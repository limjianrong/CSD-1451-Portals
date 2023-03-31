/******************************************************************************//*!
\file		GameState_Platformer.cpp
\author 	Digipen, Lin ZhaoZhi
\par    	email: z.lin@digipen.edu
\date   	30 January, 2023
\brief		Source file for the platformer game state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 *//******************************************************************************/
#include "AEEngine.h"

// ----- Game States -----
#include "GameStateManager.hpp"
#include "GameState_Mainmenu.hpp"
#include "GameStateList.hpp"

// ----- Player related -----
#include "Player.hpp"
#include "weapon_fire.hpp"
#include "portal_feature.hpp"
#include "Upgrades.hpp"

// ----- Enemies related -----
#include "Enemy.hpp"
#include "Enemy3.hpp"
#include "boss.hpp"

// ----- Camera ------
#include "camera.hpp"

// ----- Others -----
#include "Utilities.hpp"
#include "draw_level.hpp"
#include "Settings.hpp"
#include "Audio.hpp"


//#include <iostream>

// --- Mesh ---
extern AEGfxVertexList* square_mesh;													// Created square mesh
extern AEMtx33 scale, rotate, translate, transform;										// TRS
extern s8 Albam_fontID;																	// FontID
static AEGfxTexture* buttonNotPressed, * buttonPressed;									// Button textures
AEGfxTexture* background_layer1Tex, * background_layer2Tex, * background_layer3Tex;		// Background texture

// ----- Game objects -----
extern Player_stats player;		// player stats

// ----- Game states -----
bool isPaused;
bool isSettings{ FALSE };

// ----- Cursor positions -----
extern AEVec2 cursor;					// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;			// Origin is CENTER of window
extern AEVec2 world_center_cursor;		// Origin is CENTER of window
// ----- Window origin -----
extern AEVec2 origin;					// Center of screen, no matter where the camera moves

// ----- Camera -----
extern Camera camera;

// --- Audio ---
extern AEAudio buttonClickedAudio, buttonHoverAudio;
extern AEAudioGroup soundGroup;
static bool isPressed1, isPressed2, isPressed3, isPressed4;

/*!**************************************************************************************************
\brief
  In charge of loading platformer game
*******************************************************************************************************/
void GameStatePlatformerLoad(void) {

	// --- Loads fontID into memory ---
	//Albam_fontID = AEGfxCreateFont("Assets/Albam.ttf", 50);

	// --- Loads textures ---
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	background_layer1Tex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Backgrounds/set2_background.png");
	background_layer2Tex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Backgrounds/set2_tiles.png");
	background_layer3Tex = AEGfxTextureLoad("Assets/abstract-platformer/PNG/Backgrounds/set2_hills.png");

	// --- Loads mesh ---
	mesh_load();
	audio_load();

	// --- Loads different functions ---
	draw_level_load();			// Level
	enemies_load();				// Enemy1 & Enemy2
	enemy3_load();				// Enemy3
	boss_load();				// Boss
	portal_load();				// Portal
	upgrades_load();			// Upgrades
	player_load();				// Player
	GameStateSettingsLoad();
}
/*!**************************************************************************************************
\brief
  Initialise all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerInit(void) {

	audio_init();
	player_init();				// Player
	draw_level_init();			// Level
	enemies_init();				// Enemy1 & Enemy2
	enemy3_init();				// Enemy3
	boss_init();				// Boss
	portal_init();				// Portal
	upgrades_init();			// Upgrades
	camera_init();				//camera, must be initialized after portal_init as some values from
								//portal_init are used
	GameStateSettingsInit();
}
/*!**************************************************************************************************
\brief
  Updates all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerUpdate(void) {

	variables_update();  // Updating all global variables commonly used is utmost priority

	if (isPaused && (isSettings == FALSE)) {

		// --------- Collision ---------
		for (s32 i = 9; i <= 15; i += 2) {
			if (AEInputCheckReleased(AEVK_LBUTTON) &&
				center_cursor.x >= -WINDOWLENGTH_X / 10 && center_cursor.x <= WINDOWLENGTH_X / 10 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i + WINDOWLENGTH_Y / 24) {
				if (i == 9) {
					isPaused = FALSE;				// Resume button

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == 11) {
					gGameStateNext = GS_RESTART;	// Restart button

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == 13) {
					//GameStateSettingsUpdate();	// Settings button
					//isPaused = FALSE;				// Resume game
					isSettings = TRUE;

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == 15) {
					gGameStateNext = GS_MainMenu;	// Main menu button
					isPaused = FALSE;				// Resume game

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
			}
		}

	
	}

	else {
		

		draw_level_update();		// Level
		enemies_update();			// Enemy1 & Enemy2
		enemy3_update(&player);		// Enemy3
		boss_update();				// Boss
		upgrade_update();			// Upgrade
		player_update();			// Player
		update_portal();			// Upgrade
		camera_update();
	}
}
/*!**************************************************************************************************
\brief
  Draws all meshes being used for platformer game and render any texture for assets
*******************************************************************************************************/
void GameStatePlatformerDraw(void) {

	// Pre requisite setting
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEGfxTextureSet(background_layer1Tex, 0, 0);						// Layer 1
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);
	AEGfxTextureSet(background_layer2Tex, 0, 0);						// Layer 2
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	AEGfxTextureSet(background_layer3Tex, 0, 0);						// Layer 3
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// ------- Draw Functions -------
	draw_level_draw();		// Level
	enemies_draw();			// Enemy1 & Enemy2
	draw_enemy3();			// Enemy3
	boss_draw();			// Boss
	draw_portal();			// Portal
	player_draw();			// Player
	upgrade_draw();			// Upgrade


	// -------------- Pause menu --------------
	if (AEInputCheckReleased(AEVK_P)) {
		if (isPaused == FALSE) isPaused = TRUE;
		else if (isPaused == TRUE) isPaused = FALSE;
	}
	if (isPaused) {
		// --------- Make whole screen translucent ---------
		AEGfxSetTransparency(0.55f);
		AEGfxTextureSet(nullptr, 0, 0);
		drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

		// --------- Buttons ---------
		AEGfxSetTransparency(1.0f);
		for (int i = 9; i <= 15; i+=2) {

			if (center_cursor.x >= -WINDOWLENGTH_X / 10 && center_cursor.x <= WINDOWLENGTH_X / 10 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i + WINDOWLENGTH_Y / 24) {
				
				AEGfxTextureSet(buttonPressed, 0, 0);

				//if (isPressed == FALSE) {
				//	isPressed = TRUE;
				//	std::cout << "THERE IS SOUND HERE" << std::endl;
				//	// Hovering over button sfx
				//	//AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				//}
			}
			else {
				//isPressed = FALSE;
				AEGfxTextureSet(buttonNotPressed, 0, 0);
			}
			drawMesh(AEVec2{ WINDOWLENGTH_X / 5, WINDOWLENGTH_Y / 12 }, AEVec2 { origin.x, origin.y + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i }, PI);

			if (center_cursor.x >= -WINDOWLENGTH_X / 10 && center_cursor.x <= WINDOWLENGTH_X / 10 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 + WINDOWLENGTH_Y / 24) {

				if (isPressed1 == FALSE) {
					isPressed1 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed1 = FALSE;
			}

			if (center_cursor.x >= -WINDOWLENGTH_X / 10 && center_cursor.x <= WINDOWLENGTH_X / 10 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 + WINDOWLENGTH_Y / 24) {

				if (isPressed2 == FALSE) {
					isPressed2 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed2 = FALSE;
			}

			if (center_cursor.x >= -WINDOWLENGTH_X / 10 && center_cursor.x <= WINDOWLENGTH_X / 10 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 + WINDOWLENGTH_Y / 24) {

				if (isPressed3 == FALSE) {
					isPressed3 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed3 = FALSE;
			}

			if (center_cursor.x >= -WINDOWLENGTH_X / 10 && center_cursor.x <= WINDOWLENGTH_X / 10 &&
				center_cursor.y >= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 - WINDOWLENGTH_Y / 24 &&
				center_cursor.y <= WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 + WINDOWLENGTH_Y / 24) {

				if (isPressed4 == FALSE) {
					isPressed4 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed4 = FALSE;
			}


		
		}

		// --------- Texts ---------
		AEGfxPrint(Albam_fontID, (s8*)"PAUSED", -0.27f, 0.55f, 2.0f, 1, 1, 0);
		AEGfxPrint(Albam_fontID, (s8*)"RESUME", -0.11f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.12f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"SETTINGS", -0.13f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.155f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);

		if (isSettings == TRUE) {
			GameStateSettingsDraw();
			GameStateSettingsUpdate();
			if (AEInputCheckReleased(AEVK_LBUTTON) &&
				center_cursor.x >= -WINDOWLENGTH_X / 6.f && center_cursor.x <= WINDOWLENGTH_X / 6.f &&
				center_cursor.y >= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 - WINDOWLENGTH_Y / 16.f &&
				center_cursor.y <= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 + WINDOWLENGTH_Y / 16.f) {
				isSettings = FALSE;
			}
		}
	}
	AEGfxSetCamPosition(camera.x, camera.y);

}

/*!**************************************************************************************************
\brief
  Free all objects
*******************************************************************************************************/
void GameStatePlatformerFree(void) {

}
/*!**************************************************************************************************
\brief
  Unloads
*******************************************************************************************************/
void GameStatePlatformerUnload(void) {

	draw_level_unload();		// Level
	enemies_unload();			// Enemy1 & Enemy2
	enemy3_unload();			// Enemy3
	boss_unload();				// Boss
	portal_unload();			// Portal
	upgrades_unload();			// Upgrades
	player_unload();			// Player
	GameStateSettingsUnload();


	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(background_layer1Tex);
	AEGfxTextureUnload(background_layer2Tex);
	AEGfxTextureUnload(background_layer3Tex);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	// Informing the system about the loop's end
	AESysFrameEnd();
}