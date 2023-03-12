
#include "AEEngine.h"
#include "Player.hpp"			// For player_stats
#include "portal_feature.hpp"	// For portal range
#include "Utilities.hpp"		// For cursor coords, 
#include "Upgrades.hpp"
#include "GameState_Platformer.hpp"	 // For isPaused

#include "GameState_Mainmenu.hpp"  // For fontID


#include <iostream>

#include <stdlib.h>     // srand, rand

#define MAX_UPGRADES 12
Card upgrades[MAX_UPGRADES];

// --- COMMENTED OUT ---
bool visible{ false };
f64 deltaTime{};
f64 lastFrameTime{};
f64 idle_time{ 2.f };

// --- Upgrades ---
static bool isUpgradeTime{ FALSE };	 // Boolean for storing upgradeTime
s32 selected;	// temp variable to store selected card
static s32 card1, card2, card3;

// ----- Pause Screen -----
extern bool isPaused;

// ----- Portal Upgrades -----
extern float portal_range;

// ----- Game Objects -----
extern Player_stats player;

// ----- Mesh & Texture -----
static AEMtx33 scale, rotate, translate, transform;
AEGfxVertexList* uMesh;
extern f32 originX, originY;

// Shield
AEGfxTexture* ShieldTex;
static f32 Shield_X, Shield_Y;
static bool isShieldActive;

// ----- Cursor positions -----
extern AEVec2 cursor;				 // Origin at TOP LEFT corner of window
extern AEVec2 center_cursor;		 // Origin is CENTER of window
extern AEVec2 world_center_cursor;  // Origin is CENTER of window


void upgrades_load() {
	upgrades[MAX_HP_card].Texture = upgrades[MAX_HP_card2].Texture = upgrades[MAX_HP_card3].Texture = AEGfxTextureLoad("Assets/Max_HP_card.png");
	upgrades[MOVEMENT_SPEED_card].Texture = upgrades[MOVEMENT_SPEED_card2].Texture = upgrades[MOVEMENT_SPEED_card3].Texture = AEGfxTextureLoad("Assets/Speed_card.png");
	upgrades[PORTAL_RANGE_card].Texture = upgrades[PORTAL_RANGE_card2].Texture = upgrades[PORTAL_RANGE_card3].Texture = AEGfxTextureLoad("Assets/Portal_Range_card.png");
	upgrades[SHIELD_card].Texture = upgrades[SHIELD_card2].Texture = upgrades[SHIELD_card3].Texture = AEGfxTextureLoad("Assets/Shield_UP_card.png");

	ShieldTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/bubble.png");

	uMesh = create_Square_Mesh();
}

void upgrades_init() {
	// NOTE: Hardcoded for now, soon to be randomised

	//// Left card
	//upgrades[MAX_HP_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;				
	//upgrades[MAX_HP_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;				// Center of window
	upgrades[MAX_HP_card].type = MAX_HP_card;
	upgrades[MAX_HP_card2].type = MAX_HP_card;
	upgrades[MAX_HP_card3].type = MAX_HP_card;

	//// Middle card
	//upgrades[MOVEMENT_SPEED_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 2;		// Center of window
	//upgrades[MOVEMENT_SPEED_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;		// Center of window
	upgrades[MOVEMENT_SPEED_card].type = MOVEMENT_SPEED_card;
	upgrades[MOVEMENT_SPEED_card2].type = MOVEMENT_SPEED_card;
	upgrades[MOVEMENT_SPEED_card3].type = MOVEMENT_SPEED_card;

	////// Right card
	////upgrades[PORTAL_RANGE_card].x = AEGfxGetWinMaxX() - AEGetWindowWidth() / 4;
	////upgrades[PORTAL_RANGE_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;		// Center of window
	upgrades[PORTAL_RANGE_card].type = PORTAL_RANGE_card;
	upgrades[PORTAL_RANGE_card2].type = PORTAL_RANGE_card;
	upgrades[PORTAL_RANGE_card3].type = PORTAL_RANGE_card;

	//// Right card
	//upgrades[SHIELD_card].x = AEGfxGetWinMaxX() - AEGetWindowWidth() / 4;
	//upgrades[SHIELD_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;		// Center of window
	upgrades[SHIELD_card].type = SHIELD_card;
	upgrades[SHIELD_card2].type = SHIELD_card2;
	upgrades[SHIELD_card3].type = SHIELD_card3;

	isShieldActive = FALSE;

	for (s32 i = 0; i < MAX_UPGRADES; ++i) {
		upgrades[i].rotation	= PI;				// Card rotation
		upgrades[i].width		= CARD_WIDTH;		// Card width
		upgrades[i].height		= CARD_HEIGHT;		// Card height
	}

	card1 = card2 = card3 = NO_card;
	selected = NO_card;

	deltaTime = 0.0f;
	lastFrameTime = AEFrameRateControllerGetFrameTime();

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
		AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);

		// --------- Drawing cards ---------
		AEGfxSetTransparency(1.0f);
		for (s32 i = 0; i < MAX_UPGRADES; ++i) {
			AEMtx33Scale(&upgrades[i].scale, upgrades[i].width, upgrades[i].height);
			AEMtx33Rot(&upgrades[i].rotate, upgrades[i].rotation);
			AEMtx33Trans(&upgrades[i].translate, upgrades[i].x, upgrades[i].y);
			AEMtx33Concat(&upgrades[i].transform, &upgrades[i].rotate, &upgrades[i].scale);
			AEMtx33Concat(&upgrades[i].transform, &upgrades[i].translate, &upgrades[i].transform);
			AEGfxSetTransform(upgrades[i].transform.m);
			AEGfxTextureSet(upgrades[i].Texture, 0, 0);
			AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);
		}
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


	// ----- Card positions -----
	if (AEInputCheckTriggered(AEVK_J)) {
		// Initialize random seed:
		srand(AEFrameRateControllerGetFrameCount());
		// Generate secret number between 0 and 3
		card1 = rand() % 3;
		card2 = rand() % 3;
		card3 = rand() % 3;

		std::cout << "card1: " << card1 << std::endl;
		std::cout << "card2: " << card2 << std::endl;
		std::cout << "card3: " << card3 << std::endl;
		std::cout << "---------------" << std::endl;
	}

	//// Left card
	//switch (card1) {
	//	case MAX_HP_card: 			upgrades[MAX_HP_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[MAX_HP_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//	case MOVEMENT_SPEED_card:	upgrades[MOVEMENT_SPEED_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[MOVEMENT_SPEED_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//	case PORTAL_RANGE_card:		upgrades[PORTAL_RANGE_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[PORTAL_RANGE_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//	case SHIELD_card:			upgrades[SHIELD_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[SHIELD_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//}

	//// Middle card
	//switch (card2) {
	//	case MAX_HP_card:			upgrades[MAX_HP_card2].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[MAX_HP_card2].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//	case MOVEMENT_SPEED_card:	upgrades[MOVEMENT_SPEED_card2].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[MOVEMENT_SPEED_card2].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//	case PORTAL_RANGE_card:		upgrades[PORTAL_RANGE_card2].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[PORTAL_RANGE_card2].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//	case SHIELD_card:			upgrades[SHIELD_card2].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//								upgrades[SHIELD_card2].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//								break;
	//}

	//// Right card
	//switch (card3) {
	//case MAX_HP_card:			upgrades[MAX_HP_card3].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//							upgrades[MAX_HP_card3].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//							break;
	//case MOVEMENT_SPEED_card:	upgrades[MOVEMENT_SPEED_card3].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//							upgrades[MOVEMENT_SPEED_card3].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//							break;
	//case PORTAL_RANGE_card:		upgrades[PORTAL_RANGE_card3].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//							upgrades[PORTAL_RANGE_card3].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//							break;
	//case SHIELD_card:			upgrades[SHIELD_card3].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	//							upgrades[SHIELD_card3].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;
	//							break;
	//}



	// Left card
	upgrades[MOVEMENT_SPEED_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	upgrades[MOVEMENT_SPEED_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;

	// Middle card
	upgrades[PORTAL_RANGE_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 2;
	upgrades[PORTAL_RANGE_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;

	// Right card
	upgrades[SHIELD_card].x = AEGfxGetWinMaxX() - AEGetWindowWidth() / 4;
	upgrades[SHIELD_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;

	// unused card
	upgrades[MAX_HP_card].x = 2000;
	upgrades[MAX_HP_card].y = 0;



	// ----- Open upgrade screen -----
	if (player.justLeveledUp) {
		player.justLeveledUp = FALSE;	// Reset bool
		isUpgradeTime = TRUE;			// Enable UpgradeTime!!
		//isPaused = TRUE;				// Pause game
	}
	if (AEInputCheckTriggered(AEVK_LBUTTON) && isUpgradeTime) {
		for (s32 i = 0; i < MAX_UPGRADES; ++i) {
			// Checks which upgrade is selected
			AEVec2Set(&upgrades[i].center, upgrades[i].x, upgrades[i].y);
			/*std::cout << "Center X: " << upgrades[i].center.x << std::endl;
			std::cout << "Center Y: " << upgrades[i].center.y << std::endl;
			std::cout << "Mouse X: " << world_center_cursor.x << std::endl;
			std::cout << "Mouse Y: " << world_center_cursor.y << std::endl;
			std::cout << "Player X: " << player.x << std::endl;
			std::cout << "Player Y: " << player.y << std::endl;
			std::cout << "--------------------------------------------" << std::endl;*/
			if (AETestRectToRect(&upgrades[i].center, upgrades[i].width, upgrades[i].height, &world_center_cursor, 0.1f, 0.1f)) {
				selected = upgrades[i].type;
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
				case SHIELD_card2: 
				case SHIELD_card3: shield_upgrade_update();
					std::cout << "Shield cd: " << isShieldActive << std::endl;
					break;
				}
			}
		}
		// ----- Close upgrade screen -----
		isUpgradeTime = FALSE;
	}

}


void shield_upgrade_update() {
	// Enable shield
	if (!isShieldActive) {
		isShieldActive = TRUE;
	}

	// Shield follows player x & y
	if (isShieldActive) {
		Shield_X = player.x;
		Shield_Y = player.y;
	}

}

void shield_upgrade_draw() {

	AEGfxSetTransparency(0.6f);
	// Creates a shield size 70x70
	AEMtx33Scale(&scale, PLAYER_WIDTH + 20, PLAYER_HEIGHT + 20);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Trans(&translate, player.x, player.y);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxTextureSet(ShieldTex, 0, 0);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);

}