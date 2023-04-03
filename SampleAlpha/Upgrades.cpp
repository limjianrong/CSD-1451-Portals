/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Upgrades.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This source file contains all the functions in relation to card upgrade system used when player levels up
==================================================================================*/

#include "AEEngine.h"
#include "Player.hpp"				// For player_stats
#include "Enemy.hpp"				// For Enemy2 stats
#include "boss.hpp"					// For Enemy2_bullet
#include "portal_feature.hpp"		// For portal range
#include "Utilities.hpp"			// For cursor coords, 
#include "Upgrades.hpp"
#include "GameState_Platformer.hpp"	// For isPaused
#include "GameState_Mainmenu.hpp"	// For fontID

#include <iostream>					// For std
#include <fstream>					// For fileIO
#include <stdlib.h>					// For srand, rand

// --- Upgrades ---
static bool isUpgradeTime{ FALSE };			// Boolean for storing upgradeTime
static s32 selected;						// Variable to store selected card
static AEVec2 left_card_center, middle_card_center, right_card_center;
static s32 left_card, middle_card, right_card;

// ----- Pause Screen -----
extern bool isPaused;

// ----- Portal & Shield Upgrades -----
extern float portal_max_range;
static bool attainShieldUpgrade;
static s32 shieldDelay;

// ----- Game Objects -----
extern Player_stats player;
extern Enemy1_stats enemy1[MAX_ENEMIES_1];		// Array of struct enemy1
extern Enemy2_stats enemy2[MAX_ENEMIES_2];		// Array of struct enemy2
extern Bullet bullet_enemy2[MAX_ENEMIES_2];		// Array of struct enemy2's bullet
Shield shield;
Card upgrades[MAX_UPGRADES];

// ----- Mesh & Texture -----
static AEMtx33 scale, rotate, translate, transform; // temp
extern AEGfxVertexList* square_mesh;	// Created square mesh
extern AEVec2 origin;

// ---- FileIO ----
std::ifstream upgrades_ifs;

// ----- Cursor positions -----
extern AEVec2 cursor;				 // Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;		 // Origin is CENTER of window
extern AEVec2 world_center_cursor;   // Origin is CENTER of window

extern AEAudio cardAudio, levelUpAudio, receivedUpgradeAudio;
extern AEAudioGroup soundGroup;

/*!**************************************************************************************************
\brief
  Load all textures for card upgrade system
*****************************************************************************************************/
void upgrades_load() {
	upgrades[MAX_HP_card].Texture			= AEGfxTextureLoad("Assets/Max_HP_card.png");
	upgrades[MOVEMENT_SPEED_card].Texture	= AEGfxTextureLoad("Assets/Speed_card.png");
	upgrades[PORTAL_RANGE_card].Texture		= AEGfxTextureLoad("Assets/Portal_Range_card.png");
	upgrades[SHIELD_card].Texture			= AEGfxTextureLoad("Assets/Shield_UP_card.png");
	shield.Texture							= AEGfxTextureLoad("Assets/jumperpack/PNG/Items/bubble.png");

}

/*!**************************************************************************************************
\brief
  Initialise all variables and boolean for card upgrade system. Reset to initialise state when game restarts
*****************************************************************************************************/
void upgrades_init() {

	upgrades_ifs.open("Assets/textFiles/upgrades.txt");
	if (!upgrades_ifs) {
		std::cout << "\nFailed to open upgrades.txt";
	}
	std::string str{};
	upgrades_ifs >> str >> CARD_WIDTH;
	upgrades_ifs >> str >> CARD_HEIGHT;
	upgrades_ifs >> str >> SHIELD_WIDTH;
	upgrades_ifs >> str >> SHIELD_HEIGHT;
	upgrades_ifs >> str >> shieldRegenFrameDelay;
	upgrades_ifs >> str >> MAX_HP_INCREMENT;
	upgrades_ifs >> str >> SPEED_INCREMENT;
	upgrades_ifs >> str >> PORTAL_RANGE_INCREMENT;
	upgrades_ifs >> str >> SHIELD_COOLDOWN;
	upgrades_ifs >> str >> SHIELD_COOLDOWN_CAP;
	upgrades_ifs.close();

	// ---- Card type ----
	upgrades[MAX_HP_card].type = MAX_HP_card;
	upgrades[MOVEMENT_SPEED_card].type = MOVEMENT_SPEED_card;
	upgrades[PORTAL_RANGE_card].type = PORTAL_RANGE_card;
	upgrades[SHIELD_card].type = SHIELD_card;
	left_card = middle_card = right_card = selected = NO_card;

	// --- Shield Upgrade ---
	shield.center.x			= player.center.x;							// Shield bubble x position
	shield.center.y			= player.center.y;							// Shield bubble y position
	shield.dimensions.x		= player.dimensions.x + SHIELD_WIDTH;		// Shield bubble width
	shield.dimensions.y		= player.dimensions.y + SHIELD_HEIGHT;		// Shield bubble height
	player.isShieldActive	= FALSE;									// Disable shield
	
}

/*!**************************************************************************************************
\brief
  Draw all mesh for card upgrade system with its respective texture
*****************************************************************************************************/
void upgrade_draw() {

	if (player.isShieldActive) {
		render_shield();
	}

	if (isUpgradeTime) {
		// --------- Make whole screen translucent ---------
		AEGfxSetTransparency(0.55f);
		AEGfxTextureSet(nullptr, 0, 0);
		drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, NULL);

		// --------- Drawing cards ---------
		AEGfxSetTransparency(1.0f);

		render_card(left_card,	AEGfxGetWinMinX() + AEGetWindowWidth() / 4.0f, 2.f);
		render_card(middle_card,AEGfxGetWinMinX() + AEGetWindowWidth() / 2.0f, 2.f);
		render_card(right_card, AEGfxGetWinMaxX() - AEGetWindowWidth() / 4.0f, 2.f);
	}
}

/*!**************************************************************************************************
\brief
  Open up a upgrade menu whenever player levels up for them to choose out of 3 randomised cards. It
  gives the player its respective upgrade for card chosen

  Currently there's 4 upgrade cards:
  Increased Portal range, Increase Maximum health, Increase Player movement speed, Decrease shield regeneration cooldown
*****************************************************************************************************/
void upgrade_update() {
	// DEV TOOL (Does not level up the player)
	if (AEInputCheckTriggered(AEVK_Q)) {
		player.justLeveledUp = TRUE;
	}

	// Set card center for collision check
	AEVec2Set(&left_card_center, AEGfxGetWinMinX() + AEGetWindowWidth() / 4.0f, AEGfxGetWinMinY() + AEGetWindowHeight() / 2.0f);
	AEVec2Set(&middle_card_center, AEGfxGetWinMinX() + AEGetWindowWidth() / 2.0f, AEGfxGetWinMinY() + AEGetWindowHeight() / 2.0f);
	AEVec2Set(&right_card_center, AEGfxGetWinMaxX() - AEGetWindowWidth() / 4.0f, AEGfxGetWinMinY() + AEGetWindowHeight() / 2.0f);

	// ----- Open upgrade screen -----
	if (player.justLeveledUp) {

		// Audio upon levelling up
		AEAudioPlay(levelUpAudio, soundGroup, 0.5f, 1.f, 0);

		player.justLeveledUp = FALSE;		// Reset bool
		isUpgradeTime = TRUE;				// Enable UpgradeTime!!
		//isPaused = TRUE;					// Pause game

		// Initialize random seed:
		srand(AEFrameRateControllerGetFrameCount());
		// Generate random number between 0 and 3
		left_card = rand() % 4;			// left card type
		middle_card = rand() % 4;			// middle card type
		right_card = rand() % 4;			// right card type
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON) && isUpgradeTime) {

		// Left card clicked
		selected = AETestRectToRect(&left_card_center, CARD_WIDTH, CARD_HEIGHT, &world_center_cursor, 0.1f, 0.1f) ? left_card :
			AETestRectToRect(&middle_card_center, CARD_WIDTH, CARD_HEIGHT, &world_center_cursor, 0.1f, 0.1f) ? middle_card :
			AETestRectToRect(&right_card_center, CARD_WIDTH, CARD_HEIGHT, &world_center_cursor, 0.1f, 0.1f) ? right_card : NO_card;

		// Gives respective upgrades
		if (selected != NO_card) {
			switch (selected) {
			case MAX_HP_card: player.Max_Hp += MAX_HP_INCREMENT;
				break;
			case MOVEMENT_SPEED_card: player.Speed += SPEED_INCREMENT;
				break;
			case PORTAL_RANGE_card: portal_max_range += PORTAL_RANGE_INCREMENT;
				break;
			case SHIELD_card:
				attainShieldUpgrade = TRUE;		// Attained shield upgrade
				player.isShieldActive = TRUE;	// Activate shield
				shieldRegenFrameDelay -= SHIELD_COOLDOWN;

				// Cooldown cap for players that kept choosing the upgrade for Shield Cooldown
				if (shieldRegenFrameDelay <= SHIELD_COOLDOWN_CAP) {
					shieldRegenFrameDelay = SHIELD_COOLDOWN_CAP;
				}
				break;
			}
			// Audio upon choosing an upgrade
			AEAudioPlay(receivedUpgradeAudio, soundGroup, 0.5f, 1.f, 0);

#ifdef DEBUG
			switch (selected) {
			case MAX_HP_card:
				std::cout << "Max HP: " << player.Max_Hp << std::endl;
				break;
			case MOVEMENT_SPEED_card:
				std::cout << "Speed: " << player.Speed << std::endl;
				break;
			case PORTAL_RANGE_card:
				std::cout << "Portal range: " << portal_max_range << std::endl;
				break;
			case SHIELD_card:
				std::cout << "Shield cd: " << isShieldActive << std::endl;
				break;
			}
#endif // DEBUG

		}
		// ----- Close upgrade screen -----
		isUpgradeTime = FALSE;
	}

	// Shield updating
	shield_update();
}

/*!**************************************************************************************************
\brief
  Free all meshes or object instances
*****************************************************************************************************/
void upgrades_free() {

}

/*!**************************************************************************************************
\brief
  Unload all textures for card upgrade system
*****************************************************************************************************/
void upgrades_unload() {

	// Texture unload
	AEGfxTextureUnload(upgrades[MAX_HP_card].Texture);
	AEGfxTextureUnload(upgrades[MOVEMENT_SPEED_card].Texture);
	AEGfxTextureUnload(upgrades[PORTAL_RANGE_card].Texture);
	AEGfxTextureUnload(upgrades[SHIELD_card].Texture);
	AEGfxTextureUnload(shield.Texture);
}

/*!**************************************************************************************************
\brief
  Function in charge of updating shield.

  Regenerates shield based on frameElapsed
*****************************************************************************************************/
void shield_update() {

	// Shield follows player x & y
	shield.center.x = player.center.x;
	shield.center.y = player.center.y;
	
	// Regenerate shield
	if (attainShieldUpgrade) {
		if (!player.isShieldActive) {
			shieldDelay++;
			if (shieldDelay == shieldRegenFrameDelay) {
				player.isShieldActive = TRUE;	// Reactivate shield
				shieldDelay = NULL;				// Reset shield delay
			}
		}
	}
}

/*!**************************************************************************************************
\brief
  Render/Draws a shield that follows the player
*****************************************************************************************************/
void render_shield() {

	// Draw shield when active
	if (player.isShieldActive) {
		AEGfxSetTransparency(0.6f);
		AEGfxTextureSet(shield.Texture, 0, 0);
		drawMesh(shield.dimensions, shield.center, NULL);
	}
}

/*!**************************************************************************************************
\brief
  Render/Draw the cards in upgrade card menu popup
*****************************************************************************************************/
void render_card(s32 card, f32 transX, f32 offsetY)
{
	// ------ Draw all cards ------
	switch (card) {
	case MAX_HP_card:
		AEGfxTextureSet(upgrades[MAX_HP_card].Texture, 0, 0);
		break;
	case MOVEMENT_SPEED_card:
		AEGfxTextureSet(upgrades[MOVEMENT_SPEED_card].Texture, 0, 0);
		break;
	case PORTAL_RANGE_card:
		AEGfxTextureSet(upgrades[PORTAL_RANGE_card].Texture, 0, 0);
		break;
	case SHIELD_card:
		AEGfxTextureSet(upgrades[SHIELD_card].Texture, 0, 0);
		break;
	}
	drawMesh(AEVec2{ CARD_WIDTH, CARD_HEIGHT }, AEVec2{ transX, AEGfxGetWinMinY() + AEGetWindowHeight() / offsetY }, NULL);

}