#pragma once

#include "AEEngine.h"

// ---- Upgrade Card ----
static f32 CARD_WIDTH;					// Card width
static f32 CARD_HEIGHT;					// Card height
#define CARD_ROTATION PI				// Card rotation
#define MAX_UPGRADES 4					// Change this to max amount of upgrades

// ---- Upgrades ----
static f32 shieldRegenFrameDelay;		// Shield regenerate based on frame elapsed
static s32 MAX_HP_INCREMENT;			// Increment for max hp upgrade
static f32 SPEED_INCREMENT;				// Increment for speed upgrade
static f32 PORTAL_RANGE_INCREMENT;		// Increment for portal range upgrade
static f32 SHIELD_COOLDOWN;				// Decrement for shield cooldown timer
#define SHIELD_COOLDOWN_CAP 50			// Lowest cooldown for shield upgrade


static enum card_type {
	MAX_HP_card,
	MOVEMENT_SPEED_card,
	PORTAL_RANGE_card,
	SHIELD_card, 
	NO_card
};

struct Card {
	s32 type;
	AEVec2 center;

	// ---- Draw ----
	AEGfxTexture* Texture;
};


struct Shield 
{
	AEVec2 center;
	AEVec2 dimensions;		// x: Width, y: Height

	// ---- Draw ----
	AEGfxTexture* Texture;
};

void upgrades_load();
void upgrades_init();
void upgrade_draw();
void upgrade_update();
void upgrades_free();
void upgrades_unload();


void shield_update();
void render_shield();
void render_card(s32 card, f32 transX, f32 offsetY);