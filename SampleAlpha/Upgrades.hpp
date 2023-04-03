/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Upgrades.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This header file contains all the declaration in relation to card upgrade system used when player levels up
==================================================================================*/

#pragma once
#include "AEEngine.h"

// ---- Upgrade Card ----
static f32 CARD_WIDTH;					// Card width
static f32 CARD_HEIGHT;					// Card height
#define MAX_UPGRADES 4					// Max amount of upgrades

// ---- Upgrades ----
static f32 shieldRegenFrameDelay;		// Shield regenerate based on frame elapsed
static s32 MAX_HP_INCREMENT;			// Increment for max hp upgrade
static f32 SPEED_INCREMENT;				// Increment for speed upgrade
static f32 PORTAL_RANGE_INCREMENT;		// Increment for portal range upgrade
static f32 SHIELD_COOLDOWN;				// Decrement for shield cooldown timer
static f32 SHIELD_COOLDOWN_CAP;			// Lowest cooldown for shield upgrade
static f32 SHIELD_WIDTH;				// Shield width
static f32 SHIELD_HEIGHT;				// Shield width

// ----- Enum for type of cards shown in upgrade card popup -----
enum card_type {
	MAX_HP_card,
	MOVEMENT_SPEED_card,
	PORTAL_RANGE_card,
	SHIELD_card, 
	NO_card
};

// ----- Struct for card object -----
struct Card {
	s32 type;
	AEVec2 center;

	// ---- Draw ----
	AEGfxTexture* Texture;
};

// ----- Struct for shield object -----
struct Shield 
{
	AEVec2 center;
	AEVec2 dimensions;		// x: Width, y: Height

	// ---- Draw ----
	AEGfxTexture* Texture;
};

// ----- Main Functions -----
void upgrades_load();
void upgrades_init();
void upgrade_draw();
void upgrade_update();
void upgrades_free();
void upgrades_unload();

// ----- Utility Functions -----
void shield_update();
void render_shield();
void render_card(s32 card, f32 transX, f32 offsetY);