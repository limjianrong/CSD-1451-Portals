/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:							GameState_Platformer.cpp
* Course:						CSD1451
* Group Name:					Apparate
* Primary Author (30%):			Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors (70%):		Lee Zhi Yee, zhiyee.l@digipen.edu
*
* Brief:
  Source file for the platformer game state
==================================================================================*/

#include "AEEngine.h"

// ----- Game States -----
#include "GameStateManager.hpp"
#include "GameState_Mainmenu.hpp"
#include "GameStateList.hpp"

// ----- Player related -----
#include "Player.hpp"
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
#include "Tutorial.hpp"

// ---- Drawing Variables ----
extern float menu_button_scaleX;				// Width of menu button
extern float menu_button_scaleY;				// Height of menu button
extern float button_leftEdge;					// X coordinate of left edge of menu button
extern float button_rightEdge;					// X coordinate of right edge of menu button
extern float button_startY;						// Buttons drawn top down from this value above origin.y
float p_button_Yunit{ WINDOWLENGTH_Y / 20 };	// Buttons' y-coordinates are separated by multiples of this unit
int p_first_multiple{ 9 };						// First multiple of button_Yunit
int p_multiple_increment{ 2 };					// Increment of multiples of button_Yunit
extern int button_count;						// 5 menu buttons

extern f32 buttonX;
extern f32 buttonY;
extern f32 button_scaleX;
extern f32 button_scaleY;


// --- Mesh ---
extern AEGfxVertexList* square_mesh;													// Created square mesh
extern AEMtx33 scale, rotate, translate, transform;										// TRS
extern s8 Albam_fontID;																	// FontID
static AEGfxTexture* buttonNotPressed, * buttonPressed;									// Button textures
AEGfxTexture* background_layer1Tex, * background_layer2Tex, * background_layer3Tex;		// Background texture

// ----- Game objects -----
extern Player_stats player;				// player stats

// ----- Game states -----
bool isPaused;
bool isSettings{ FALSE };
bool isTutorial{ FALSE };

// ----- Cursor positions -----
extern AEVec2 cursor;					// Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;			// Origin is CENTER of window
extern AEVec2 world_center_cursor;		// Origin is CENTER of window
// ----- Window origin -----
extern AEVec2 origin;					// Center of screen, no matter where the camera moves

// ----- Camera -----
extern Camera camera;

// --- Audio ---
extern AEAudio buttonClickedAudio, buttonHoverAudio, gameBGM;
extern AEAudioGroup soundGroup, musicGroup;
static bool isPressed1, isPressed2, isPressed3, isPressed4, isPressed5;
extern AEVec2 button_offset;
extern float barscalex, barscaley;
//static bool TisPressed;
//static bool SisPressed1, SisPressed2;

// --- Quit Confirmation ---
bool p_isQuitting;
extern float quit_scaleX;
extern float quit_scaleY;
AEGfxTexture* p_quitWindow;

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
	p_quitWindow = AEGfxTextureLoad("Assets/card.png");

	// --- Loads mesh ---
	mesh_load();
	//audio_load();

	// --- Loads different functions ---
	draw_level_load();			// Level
	enemies_load();				// Enemy1 & Enemy2
	enemy3_load();				// Enemy3
	boss_load();				// Boss
	portal_load();				// Portal
	upgrades_load();			// Upgrades
	player_load();				// Player
	settings_load();			// Settings Menu
	camera_load();
	tutorial_load();			// Tutorial Menu
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
	settings_init();			// Settings Menu
	tutorial_init();			// Tutorial Menu

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

	// --- Game BGM ---
	AEAudioPlay(gameBGM, musicGroup, 0.25f, 1.f, -1);

}
/*!**************************************************************************************************
\brief
  Updates all objects being used for platformer game
*******************************************************************************************************/
void GameStatePlatformerUpdate(void) {

	variables_update();  // Updating all global variables commonly used is utmost priority

	if (isPaused && (isSettings == FALSE) && (isTutorial == FALSE)) {

		// --------- Collision ---------
		for (s32 i = p_first_multiple; i <= p_first_multiple + p_multiple_increment*(button_count-1); i += p_multiple_increment) {
			if (AEInputCheckReleased(AEVK_LBUTTON) &&
				center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - p_button_Yunit * i - menu_button_scaleY/2 &&
				center_cursor.y <= button_startY - p_button_Yunit * i + menu_button_scaleY/2
				&& p_isQuitting == FALSE) {
				if (i == p_first_multiple) {
					isPaused = FALSE;				// Resume button

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}
				else if (i == p_first_multiple + (p_multiple_increment*1)) {
					gGameStateNext = GS_RESTART;	// Restart button

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
					// Stop all audio
					AEAudioStopGroup(soundGroup);
					AEAudioStopGroup(musicGroup);
				}
				else if (i == p_first_multiple + (p_multiple_increment * 2)) {
					//GameStateSettingsUpdate();	// Settings button
					//isPaused = FALSE;				// Resume game
					isSettings = TRUE;

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);

				}
				else if (i == p_first_multiple + (p_multiple_increment * 3)) {
					//gGameStateNext = GS_Tutorial;	// Main menu button
					isTutorial = TRUE;

					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
	
				}
				else if (i == p_first_multiple + (p_multiple_increment * 4)) {
					p_isQuitting = TRUE;

					// Stop all audio
					AEAudioStopGroup(soundGroup);
					AEAudioStopGroup(musicGroup);
					// Audio once button is pressed
					AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				}

			}

		}

		if (p_isQuitting == TRUE) {
			for (int i = p_first_multiple + p_multiple_increment * 2; i <= p_first_multiple + (p_multiple_increment * (button_count - 2)); i += p_multiple_increment) {
				if (AEInputCheckReleased(AEVK_LBUTTON) &&
					isSettings == FALSE &&
					(center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
						center_cursor.y >= button_startY - p_button_Yunit * i - menu_button_scaleY / 2 &&
						center_cursor.y <= button_startY - p_button_Yunit * i + menu_button_scaleY / 2)) {


					if (i == p_first_multiple + p_multiple_increment * 2) {
						p_isQuitting = FALSE;

						AEAudioPlay(gameBGM, musicGroup, 0.25f, 1.f, -1);
						// Audio once button is pressed
						AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
					}
					else if (i == p_first_multiple + p_multiple_increment * 3) {
						gGameStateNext = GS_MainMenu;
						// Audio once button is pressed
						AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
					}
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
		update_portal();			// Portal
		camera_update();			// Camera
		
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
		for (s32 i = p_first_multiple; i <= p_first_multiple + p_multiple_increment * (button_count - 1); i += p_multiple_increment) {
			
			if (center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - p_button_Yunit * i - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - p_button_Yunit * i + menu_button_scaleY / 2
				&& isSettings == FALSE
				&& p_isQuitting == FALSE) {
				
				AEGfxTextureSet(buttonPressed, 0, 0);
			}
			else {
				//isPressed = FALSE;
				AEGfxTextureSet(buttonNotPressed, 0, 0);
			}
			drawMesh(AEVec2{ WINDOWLENGTH_X / 5, WINDOWLENGTH_Y / 12 }, AEVec2 { origin.x, origin.y + WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * i }, PI);

			// 1st button from top
			if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - (p_button_Yunit * p_first_multiple) - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - (p_button_Yunit * p_first_multiple) + menu_button_scaleY / 2)
				&& isSettings == FALSE
				&& p_isQuitting == FALSE
				&& isTutorial == FALSE)
			{

				if (isPressed1 == FALSE) {
					isPressed1 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed1 = FALSE;
			}

			// 2nd button from top
			if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 1))) - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 1))) + menu_button_scaleY / 2)
				&& isSettings == FALSE
				&& p_isQuitting == FALSE
				&& isTutorial == FALSE)
			{

				if (isPressed2 == FALSE) {
					isPressed2 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed2 = FALSE;
			}

			// 3rd button from top
			if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 2))) - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 2))) + menu_button_scaleY / 2)
				&& isSettings == FALSE
				&& isTutorial == FALSE)
			{

				if (isPressed3 == FALSE) {
					isPressed3 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed3 = FALSE;
			}

			// 4th button from top
			if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 3))) - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 3))) + menu_button_scaleY / 2)
				&& isSettings == FALSE
				&& isTutorial == FALSE)
			{

				if (isPressed4 == FALSE) {
					isPressed4 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed4 = FALSE;
			}

			// 5th button from top
			if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
				center_cursor.y >= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 4))) - menu_button_scaleY / 2 &&
				center_cursor.y <= button_startY - (p_button_Yunit * (p_first_multiple + (p_multiple_increment * 4))) + menu_button_scaleY / 2)
				&& isSettings == FALSE
				&& p_isQuitting == FALSE
				&& isTutorial == FALSE)
			{

				if (isPressed5 == FALSE) {
					isPressed5 = TRUE;
					// Hovering over button sfx
					AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);
				}
			}
			else {
				isPressed5 = FALSE;
			}

		
		}

		// --------- Texts ---------
		AEGfxPrint(Albam_fontID, (s8*)"PAUSED", -0.27f, 0.55f, 2.0f, 1, 1, 0);
		AEGfxPrint(Albam_fontID, (s8*)"RESUME", -0.09f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 9 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"RESTART", -0.1f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 11 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"SETTINGS", -0.11f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 13 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"TUTORIAL", -0.11f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 15 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);
		AEGfxPrint(Albam_fontID, (s8*)"MAIN MENU", -0.12f, (WINDOWLENGTH_Y / 2 - WINDOWLENGTH_Y / 20 * 17 - WINDOWLENGTH_Y / 44) / (WINDOWLENGTH_Y / 2.0f), 0.85f, 1, 1, 1);

		if (isSettings == TRUE) {
			settings_draw();	// Draw menu to screen
			settings_update();	// Update collision within menu

			// if "Back" button pressed, exit settings menu
			if (AEInputCheckReleased(AEVK_LBUTTON) &&
				center_cursor.x >= -WINDOWLENGTH_X / 6.f && center_cursor.x <= WINDOWLENGTH_X / 6.f &&
				center_cursor.y >= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 - WINDOWLENGTH_Y / 16.f &&
				center_cursor.y <= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 27 + WINDOWLENGTH_Y / 16.f) {
				AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);

				isSettings = FALSE;
			}
		}

		if (isTutorial == TRUE) {
			tutorial_draw();	// Draw menu to screen
			tutorial_update();	// Update collision within menu
			

			if (AEInputCheckReleased(AEVK_LBUTTON) &&
				world_center_cursor.x >= buttonX - button_scaleX / 2 && world_center_cursor.x <= buttonX + button_scaleX / 2 &&
				world_center_cursor.y >= buttonY - button_scaleY / 2 &&
				world_center_cursor.y <= buttonY + button_scaleY / 2) {
				AEAudioPlay(buttonClickedAudio, soundGroup, 0.75f, 1.f, 0);
				isTutorial = FALSE;
			}
		}

		if (p_isQuitting == TRUE) {
			// --------- Make whole screen translucent ---------
			AEGfxSetTransparency(0.55f);
			AEGfxTextureSet(nullptr, 0, 0);
			drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

			AEGfxSetTransparency(1.f);

			AEGfxTextureSet(p_quitWindow, 0, 0);
			drawMesh(AEVec2{ quit_scaleX, quit_scaleY }, origin, PI);

			for (int i = p_first_multiple + p_multiple_increment * 2; i <= p_first_multiple + (p_multiple_increment * (button_count - 2)); i += p_multiple_increment) {

				if ((center_cursor.x >= button_leftEdge && center_cursor.x <= button_rightEdge &&
					center_cursor.y >= button_startY - p_button_Yunit * i - menu_button_scaleY / 2 &&
					center_cursor.y <= button_startY - p_button_Yunit * i + menu_button_scaleY / 2)
					&& isSettings == FALSE)
					AEGfxTextureSet(buttonPressed, 0, 0);

				else AEGfxTextureSet(buttonNotPressed, 0, 0);

				drawMesh(AEVec2{ menu_button_scaleX, menu_button_scaleY }, AEVec2{ origin.x, origin.y + button_startY - p_button_Yunit * i }, PI);

				// ------ Texts ------

				float text_originY = ((AEGfxGetWinMaxY() - origin.y) / (WINDOWLENGTH_Y / 2)) - 1.f;
				float text_originX = ((AEGfxGetWinMaxX() - origin.x) / (WINDOWLENGTH_X / 2)) - 1.f;

				AEGfxPrint(Albam_fontID, (s8*)"Are you sure", text_originX - 0.15f, text_originY + 0.6f, 0.75f, 0, 0, 0);
				AEGfxPrint(Albam_fontID, (s8*)"you want to quit", text_originX - 0.19f, text_originY + 0.45f, 0.75f, 0, 0, 0);
				AEGfxPrint(Albam_fontID, (s8*)"the game?", text_originX - 0.13f, text_originY + 0.3f , 0.75f, 0, 0, 0);
				AEGfxPrint(Albam_fontID, (s8*)"All progress", text_originX - 0.141f , text_originY + 0.15f, 0.75f, 0, 0, 0);
				AEGfxPrint(Albam_fontID, (s8*)"will be lost.", text_originX - 0.139f, text_originY, 0.75f, 0, 0, 0);


				AEGfxPrint(Albam_fontID, (s8*)"BACK", -0.059, text_originY - 0.32f, 0.75f, 1, 1, 1);
				AEGfxPrint(Albam_fontID, (s8*)"QUIT GAME", -0.11, text_originY - 0.53f, 0.75f, 1, 1, 1);
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
	settings_unload();			// Settings Menu
	tutorial_unload();			// Tutorial Menu


	// Texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(background_layer1Tex);
	AEGfxTextureUnload(background_layer2Tex);
	AEGfxTextureUnload(background_layer3Tex);
	AEGfxTextureUnload(p_quitWindow);

	// Mesh free
	AEGfxMeshFree(square_mesh);

	// Informing the system about the loop's end
	AESysFrameEnd();
}