
#include "AEEngine.h"
#include "Player.hpp"			// For player_stats
#include "Enemy.hpp"			// For Enemy2 stats
#include "weapon_fire.hpp"		// For Enemy2_bullet
#include "portal_feature.hpp"	// For portal range
#include "Utilities.hpp"		// For cursor coords, 
#include "Upgrades.hpp"
#include "GameState_Platformer.hpp"	 // For isPaused

#include "GameState_Mainmenu.hpp"  // For fontID


#include <iostream>

#include <stdlib.h>     // srand, rand

#define MAX_UPGRADES 4
Card upgrades[MAX_UPGRADES];

// --- Upgrades ---
static bool isUpgradeTime{ FALSE };	 // Boolean for storing upgradeTime
s32 selected;	// temp variable to store selected card
static AEVec2 left_card_center, middle_card_center, right_card_center;
static s32 card1, card2, card3;

// ----- Pause Screen -----
extern bool isPaused;

// ----- Portal Upgrades -----
extern float portal_range;

// ----- Game Objects -----
extern Player_stats player;
extern Enemy1_stats enemy1[MAX_ENEMIES_1];		// Array of struct enemy1
extern Enemy2_stats enemy2[MAX_ENEMIES_2];		// Array of struct enemy2
extern Bullet bullet_enemy2[MAX_ENEMIES_2];		// Array of struct enemy2's bullet
Shield shield;

// ----- Mesh & Texture -----
static AEMtx33 scale, rotate, translate, transform; // temp
extern AEGfxVertexList* square_mesh;	// Created square mesh
extern f32 originX, originY;

// Shield
static f32 Shield_X, Shield_Y;
static bool isShieldActive;

// ----- Cursor positions -----
extern AEVec2 cursor;				 // Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;		 // Origin is CENTER of window
extern AEVec2 world_center_cursor;  // Origin is CENTER of window


void upgrades_load() {
	upgrades[MAX_HP_card].Texture = AEGfxTextureLoad("Assets/Max_HP_card.png");
	upgrades[MOVEMENT_SPEED_card].Texture = AEGfxTextureLoad("Assets/Speed_card.png");
	upgrades[PORTAL_RANGE_card].Texture = AEGfxTextureLoad("Assets/Portal_Range_card.png");
	upgrades[SHIELD_card].Texture = AEGfxTextureLoad("Assets/Shield_UP_card.png");

	shield.Texture = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/bubble.png");
}

void upgrades_init() {

	// --- Card type ---
	upgrades[MAX_HP_card].type = MAX_HP_card;
	upgrades[MOVEMENT_SPEED_card].type = MOVEMENT_SPEED_card;
	upgrades[PORTAL_RANGE_card].type = PORTAL_RANGE_card;
	upgrades[SHIELD_card].type = SHIELD_card;


	isShieldActive = FALSE;

	for (s32 i = 0; i < MAX_UPGRADES; ++i) {
		upgrades[i].rotation	= PI;				// Card rotation
		upgrades[i].width		= CARD_WIDTH;		// Card width
		upgrades[i].height		= CARD_HEIGHT;		// Card height
	}

	card1 = card2 = card3 = NO_card;
	selected = NO_card;

	// --- Shield Upgrade ---
	shield.x = player.x;					// Shield bubble x position
	shield.y = player.y;					// Shield bubble y position
	shield.rotation = PI;					// Shield bubble rotation
	shield.width = player.width + 20;		// Shield bubble height
	shield.height = player.height + 20;		// Shield bubble width

}

void upgrade_draw() {

	if (isUpgradeTime) {
		// --------- Make whole screen translucent ---------
		AEGfxSetTransparency(0.55f);
		AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, originX, originY);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxTextureSet(nullptr, 0, 0);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

		// --------- Drawing cards ---------
		AEGfxSetTransparency(1.0f);
		/*for (s32 i = 0; i < MAX_UPGRADES; ++i) {
			AEMtx33Scale(&upgrades[i].scale, upgrades[i].width, upgrades[i].height);
			AEMtx33Rot(&upgrades[i].rotate, upgrades[i].rotation);
			AEMtx33Trans(&upgrades[i].translate, upgrades[i].x, upgrades[i].y);
			AEMtx33Concat(&upgrades[i].transform, &upgrades[i].rotate, &upgrades[i].scale);
			AEMtx33Concat(&upgrades[i].transform, &upgrades[i].translate, &upgrades[i].transform);
			AEGfxSetTransform(upgrades[i].transform.m);
			AEGfxTextureSet(upgrades[i].Texture, 0, 0);
			AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);
		}*/

		AEMtx33Scale(&scale, upgrades[MAX_HP_card].width, upgrades[MAX_HP_card].height);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMinX() + AEGetWindowWidth() / 4, AEGfxGetWinMinY() + AEGetWindowHeight() / 2);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);

		switch (card1) {
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
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);



		AEMtx33Scale(&scale, upgrades[MAX_HP_card].width, upgrades[MAX_HP_card].height);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMinX() + AEGetWindowWidth() / 2, AEGfxGetWinMinY() + AEGetWindowHeight() / 2);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		switch (card2) {
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
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);



		AEMtx33Scale(&scale, upgrades[MAX_HP_card].width, upgrades[MAX_HP_card].height);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, AEGfxGetWinMaxX() - AEGetWindowWidth() / 4, AEGfxGetWinMinY() + AEGetWindowHeight() / 2);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		switch (card3) {
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
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	}
	if (isShieldActive) {
		shield_upgrade_draw();
	}
}

void upgrade_update() {
	//CHEAT
	if (AEInputCheckTriggered(AEVK_Q)) {
		player.justLeveledUp = TRUE;
	}

	AEVec2Set(&left_card_center, AEGfxGetWinMinX() + AEGetWindowWidth() / 4, AEGfxGetWinMinY() + AEGetWindowHeight() / 2);
	AEVec2Set(&middle_card_center, AEGfxGetWinMinX() + AEGetWindowWidth() / 2, AEGfxGetWinMinY() + AEGetWindowHeight() / 2);
	AEVec2Set(&right_card_center, AEGfxGetWinMaxX() - AEGetWindowWidth() / 4, AEGfxGetWinMinY() + AEGetWindowHeight() / 2);


	// ----- Open upgrade screen -----
	if (player.justLeveledUp) {
		player.justLeveledUp = FALSE;	// Reset bool
		isUpgradeTime = TRUE;			// Enable UpgradeTime!!
		//isPaused = TRUE;				// Pause game

		// Initialize random seed:
		srand(AEFrameRateControllerGetFrameCount());
		// Generate random number between 0 and 3
		card1 = rand() % 4;			// Card1 type
		card2 = rand() % 4;			// Card2 type
		card3 = rand() % 4;			// Card3 type

		/*std::cout << "card1: " << card1 << std::endl;
		std::cout << "card2: " << card2 << std::endl;
		std::cout << "card3: " << card3 << std::endl;
		std::cout << "---------------" << std::endl;*/
	}
	if (AEInputCheckTriggered(AEVK_LBUTTON) && isUpgradeTime) {

		// Left card clicked
		if (AETestRectToRect(&left_card_center, CARD_WIDTH, CARD_HEIGHT, &world_center_cursor, 0.1f, 0.1f)) {
			selected = card1;
		}
		// Middle card clicked
		else if (AETestRectToRect(&middle_card_center, CARD_WIDTH, CARD_HEIGHT, &world_center_cursor, 0.1f, 0.1f)) {
			selected = card2;
		}
		// Right card clicked
		else if (AETestRectToRect(&right_card_center, CARD_WIDTH, CARD_HEIGHT, &world_center_cursor, 0.1f, 0.1f)) {
			selected = card3;
		}
		else {
			selected = NO_card;
		}

		//std::cout << "Mouse: " << world_center_cursor.x << std::endl;
		//std::cout << "Selected: " << selected << std::endl;

		if (selected != NO_card) {
			switch (selected) {
			case MAX_HP_card: player.Max_Hp += 1;
				std::cout << "Max HP: " << player.Max_Hp << std::endl;
				break;
			case MOVEMENT_SPEED_card: player.Speed += 0.5;
				std::cout << "Speed: " << player.Speed << std::endl;
				break;
			case PORTAL_RANGE_card: portal_range += 50;
				std::cout << "Portal range: " << portal_range << std::endl;
				break;
			case SHIELD_card: 
				// Enable shield
				if (!isShieldActive) {
					isShieldActive = TRUE;
				}
				std::cout << "Shield cd: " << isShieldActive << std::endl;
				break;
			}
		}
		// ----- Close upgrade screen -----
		isUpgradeTime = FALSE;
	}

	shield_upgrade_update();

}


void shield_upgrade_update() {

	// Shield follows player x & y
	if (isShieldActive) {
		shield.x = player.x;
		shield.y = player.y;
		AEVec2Set(&shield.center, shield.x, shield.y);
	}

	// ----- Disable shield when shot by Enemy2 -----
	for (s32 i = 0; i < MAX_ENEMIES_2; ++i) {
		// ----- Bullet collision with enemy2 -----
		if (AETestRectToRect(&bullet_enemy2[i].center, bullet_enemy2[i].width, bullet_enemy2[i].height, &player.center, player.width, player.height)) {
			isShieldActive = FALSE;
		}
	}



}

void shield_upgrade_draw() {

	AEGfxSetTransparency(0.6f);
	// Creates a shield size 70x70
	AEMtx33Scale(&shield.scale, PLAYER_WIDTH + 20, PLAYER_HEIGHT + 20);
	AEMtx33Rot(&shield.rotate, PI);
	AEMtx33Trans(&shield.translate, shield.x, shield.y);
	AEMtx33Concat(&shield.transform, &shield.rotate, &shield.scale);
	AEMtx33Concat(&shield.transform, &shield.translate, &shield.transform);
	AEGfxTextureSet(shield.Texture, 0, 0);
	AEGfxSetTransform(shield.transform.m);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

}