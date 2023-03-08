
#include "AEEngine.h"
#include "Player.hpp"			// For player_stats
#include "portal_feature.hpp"	// For portal range
#include "Utilities.hpp"
#include "Upgrades.hpp"
#include "GameState_Platformer.hpp"	 // For isPaused

#include "GameState_Mainmenu.hpp"  // For fontID


#include <iostream>

extern Player_stats player;
//extern s8 Albam_fontID;
//s8* t1;
//s8* t2;
//s8* t3;

#define MAX_UPGRADES 3
Card upgrades[MAX_UPGRADES];

bool visible{ false };
static bool isUpgradeTime{ FALSE };	 // Boolean for storing upgradeTime
s32 selected;	// temp variable to store selected card

// ----- Pause Screen -----
extern bool isPaused;

// ----- Portal Upgrades -----
extern float portal_range;

// ----- Mesh & Texture -----
static AEMtx33 scale, rotate, translate, transform;
AEGfxVertexList* uMesh;
extern f32 originX, originY;

f64 deltaTime{};
f64 lastFrameTime{};
f64 idle_time{ 2.f };

void upgrades_load() {
	upgrades[MAX_HP_card].Texture = AEGfxTextureLoad("Assets/Max_HP_card.png");
	upgrades[MOVEMENT_SPEED_card].Texture = AEGfxTextureLoad("Assets/Speed_card.png");
	upgrades[PORTAL_RANGE_card].Texture = AEGfxTextureLoad("Assets/Portal_Range_card.png");

	uMesh = create_Square_Mesh();
}

void upgrades_init() {
	// NOTE: Hardcoded for now, soon to be randomised

	// Left card
	upgrades[MAX_HP_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;				
	upgrades[MAX_HP_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;				// Center of window
	upgrades[MAX_HP_card].type = MAX_HP_card;

	// Middle card
	upgrades[MOVEMENT_SPEED_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 2;		// Center of window
	upgrades[MOVEMENT_SPEED_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;		// Center of window
	upgrades[MOVEMENT_SPEED_card].type = MOVEMENT_SPEED_card;

	// Right card
	upgrades[PORTAL_RANGE_card].x = AEGfxGetWinMaxX() - AEGetWindowWidth() / 4;
	upgrades[PORTAL_RANGE_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;		// Center of window
	upgrades[PORTAL_RANGE_card].type = PORTAL_RANGE_card;

	for (s32 i = 0; i < MAX_UPGRADES; ++i) {
		upgrades[i].rotation = PI;
		upgrades[i].width = CARD_WIDTH;
		upgrades[i].height = CARD_HEIGHT;
	}

	s32 selected;

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

}

void upgrade_update() {
	//CHEAT
	if (AEInputCheckTriggered(AEVK_Q)) {
		player.justLeveledUp = TRUE;
	}


	// ----- Card positions -----
	// Left card
	upgrades[MAX_HP_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 4;
	upgrades[MAX_HP_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;				// Center of window

	// Middle card
	upgrades[MOVEMENT_SPEED_card].x = AEGfxGetWinMinX() + AEGetWindowWidth() / 2;		// Center of window
	upgrades[MOVEMENT_SPEED_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;		// Center of window

	// Right card
	upgrades[PORTAL_RANGE_card].x = AEGfxGetWinMaxX() - AEGetWindowWidth() / 4;
	upgrades[PORTAL_RANGE_card].y = AEGfxGetWinMinY() + AEGetWindowHeight() / 2;		// Center of window


	// ----- Open upgrade screen -----
	if (player.justLeveledUp) {
		player.justLeveledUp = FALSE;	// Reset bool
		isUpgradeTime = TRUE;			// Enable UpgradeTime!!
		//isPaused = TRUE;				// Pause game
	}
	if (AEInputCheckTriggered(AEVK_LBUTTON) && isUpgradeTime) {

		AEVec2 temp_Mouse = get_cursor_center_position();
		for (s32 i = 0; i < MAX_UPGRADES; ++i) {
			// Checks which upgrade is selected
			AEVec2Set(&upgrades[i].center, upgrades[i].x, upgrades[i].y);
			if (AETestRectToRect(&upgrades[i].center, upgrades[i].width, upgrades[i].height, &temp_Mouse, 0, 0)) {
				selected = upgrades[i].type;
			}
			else {
				selected = NO_card;
			}

			//std::cout << "Mouse: " << temp_Mouse.x << std::endl;
			//std::cout << "Selected: " << selected << std::endl;

			if (selected != NO_card) {
				switch (selected) {
				case MAX_HP_card: player.Max_Hp += 1;
					//std::cout << "Max HP: " << player.Max_Hp << std::endl;
					break;
				case MOVEMENT_SPEED_card: player.Speed += 0.5;
					//std::cout << "Speed: " << player.Speed << std::endl;
					break;
				case PORTAL_RANGE_card: portal_range += 50;
					//std::cout << "Portal range: " << portal_range << std::endl;
					break;
				}
			}
		}
		// ----- Close upgrade screen -----
		isUpgradeTime = FALSE;
	}

}




//void update_upgrade_cards() {
//
//	
//	if ((player.Level != 0) && !visible ) {
//		visible = true;
//		idle_time -= AEFrameRateControllerGetFrameTime();
//		if (idle_time > 0) {
//			//draw_upgrade_cards();
//		}
//	
//		else {
//			visible = false;
//		}
//
//		if (!visible) idle_time = 2.f;
//	}
//}