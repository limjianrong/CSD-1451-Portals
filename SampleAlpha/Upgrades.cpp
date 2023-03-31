
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

// ----- Portal Upgrades -----
extern float portal_max_range;

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

// --- Shield ---
bool isShieldActive;

std::ifstream upgrades_ifs{};

// ----- Cursor positions -----
extern AEVec2 cursor;				 // Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;		 // Origin is CENTER of window
extern AEVec2 world_center_cursor;   // Origin is CENTER of window


void upgrades_load() {
	upgrades[MAX_HP_card].Texture = AEGfxTextureLoad("Assets/Max_HP_card.png");
	upgrades[MOVEMENT_SPEED_card].Texture = AEGfxTextureLoad("Assets/Speed_card.png");
	upgrades[PORTAL_RANGE_card].Texture = AEGfxTextureLoad("Assets/Portal_Range_card.png");
	upgrades[SHIELD_card].Texture = AEGfxTextureLoad("Assets/Shield_UP_card.png");

	shield.Texture = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/bubble.png");
}

void upgrades_init() {

	upgrades_ifs.open("Assets/textFiles/upgrades.txt");
	if (!upgrades_ifs) {
		std::cout << "\nFailed to open upgrades.txt";
	}
	std::string str{};
	upgrades_ifs >> str >> CARD_WIDTH;
	upgrades_ifs >> str >> CARD_HEIGHT;
	upgrades_ifs >> str >> MAX_HP_INCREMENT;
	upgrades_ifs >> str >> SPEED_INCREMENT;
	upgrades_ifs >> str >> PORTAL_RANGE_INCREMENT;
	upgrades_ifs.close();

	// ---- Card type ----
	upgrades[MAX_HP_card].type = MAX_HP_card;
	upgrades[MOVEMENT_SPEED_card].type = MOVEMENT_SPEED_card;
	upgrades[PORTAL_RANGE_card].type = PORTAL_RANGE_card;
	upgrades[SHIELD_card].type = SHIELD_card;
	left_card = middle_card = right_card = selected = NO_card;

	// --- Shield Upgrade ---
	shield.center.x = player.center.x;					// Shield bubble x position
	shield.center.y = player.center.y;					// Shield bubble y position
	shield.dimensions.x = player.dimensions.x + 20;	// Shield bubble width
	shield.dimensions.y = player.dimensions.y + 20;	// Shield bubble height
	isShieldActive = FALSE;						// Disable shield
}

void upgrade_draw() {

	if (isUpgradeTime) {
		// --------- Make whole screen translucent ---------
		AEGfxSetTransparency(0.55f);
		AEGfxTextureSet(nullptr, 0, 0);
		drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

		// --------- Drawing cards ---------
		AEGfxSetTransparency(1.0f);

		render_card(left_card,	AEGfxGetWinMinX() + AEGetWindowWidth() / 4.0f, 2.f);
		render_card(middle_card,AEGfxGetWinMinX() + AEGetWindowWidth() / 2.0f, 2.f);
		render_card(right_card, AEGfxGetWinMaxX() - AEGetWindowWidth() / 4.0f, 2.f);
	}
	if (isShieldActive) {
		render_shield();
	}
}

void upgrades_free() {

}

void upgrades_unload() {

	// Texture unload
	AEGfxTextureUnload(upgrades[MAX_HP_card].Texture);
	AEGfxTextureUnload(upgrades[MOVEMENT_SPEED_card].Texture);
	AEGfxTextureUnload(upgrades[PORTAL_RANGE_card].Texture);
	AEGfxTextureUnload(upgrades[SHIELD_card].Texture);
	AEGfxTextureUnload(shield.Texture);
}

void upgrade_update() {
	// DEV TOOL
	if (AEInputCheckTriggered(AEVK_Q)) {
		player.justLeveledUp = TRUE;
	}

	// Set card center for collision check
	AEVec2Set(&left_card_center,	AEGfxGetWinMinX() + AEGetWindowWidth() / 4.0f, AEGfxGetWinMinY() + AEGetWindowHeight() / 2.0f);
	AEVec2Set(&middle_card_center,	AEGfxGetWinMinX() + AEGetWindowWidth() / 2.0f, AEGfxGetWinMinY() + AEGetWindowHeight() / 2.0f);
	AEVec2Set(&right_card_center,	AEGfxGetWinMaxX() - AEGetWindowWidth() / 4.0f, AEGfxGetWinMinY() + AEGetWindowHeight() / 2.0f);

	//std::cout << player.justLeveledUp << std::endl;
	/*bool temp = FALSE;
	if (AEInputCheckReleased(AEVK_LBUTTON)) {
		temp = TRUE;
	}
	if (temp) {
		AEAudioPlay(player.levelUpAudio, soundGroup, 1.f, 1.f, 0);
		temp = FALSE;
	}*/
	// ----- Open upgrade screen -----
	if (player.justLeveledUp) {
		player.justLeveledUp = FALSE;		// Reset bool
		isUpgradeTime = TRUE;				// Enable UpgradeTime!!
		//isPaused = TRUE;					// Pause game

		// Initialize random seed:
		srand(AEFrameRateControllerGetFrameCount());
		// Generate random number between 0 and 3
		left_card	= rand() % 4;			// left card type
		middle_card	= rand() % 4;			// middle card type
		right_card	= rand() % 4;			// right card type
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
				// Enable shield
				if (!isShieldActive) {
					isShieldActive = TRUE;
				}
				break;
			}

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

	shield_update();
}


void shield_update() {

	// Shield follows player x & y
	if (isShieldActive) {
		shield.center.x = player.center.x;
		shield.center.y = player.center.y;
	}

	// ----- Disable shield when shot by Enemy2 -----
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		// ----- Bullet collision with enemy2 -----
		if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &player.center, player.dimensions.x, player.dimensions.y)) {
			isShieldActive = FALSE;
		}
	}
}

void render_shield() {

	// Creates a shield
	AEGfxSetTransparency(0.6f);
	AEGfxTextureSet(shield.Texture, 0, 0);
	drawMesh(shield.dimensions, shield.center, SHIELD_ROTATION);
}

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
	drawMesh(AEVec2{ CARD_WIDTH, CARD_HEIGHT }, AEVec2{ transX, AEGfxGetWinMinY() + AEGetWindowHeight() / offsetY }, CARD_ROTATION);

}