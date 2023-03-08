#include "AEEngine.h"
#include "Player.hpp"
#include "Utilities.hpp"
#include "Upgrades.hpp"
// for fontID
#include "GameState_Mainmenu.hpp"


extern Player_stats player;
extern s8 Albam_fontID;
s8* t1;
s8* t2;
s8* t3;
//card c1, c2, c3;
//card c1{ -300.f, 0.f };
//card c2{ 0.f, 0.f };
//card c3{ 300.f, 0.f };

#define MAX_UPGRADES 3
Card upgrades[MAX_UPGRADES];

bool visible{ false };
bool isUpgradeTime{ FALSE };

// ----- Mesh & Texture -----
static AEMtx33 scale, rotate, translate, transform;
AEGfxVertexList* uMesh;
extern f32 originX, originY;

f64 deltaTime{};
f64 lastFrameTime{};
f64 idle_time{ 2.f };

void upgrades_load() {
	upgrades[0].Texture = AEGfxTextureLoad("Assets/card.png");
	uMesh = create_Square_Mesh();
}

void upgrades_init() {

	upgrades[0].x = -300;
	upgrades[0].y = 0;
	upgrades[0].type = HP_card;

	upgrades[1].x = 0;
	upgrades[1].y = 0;
	upgrades[1].type = MOVEMENT_SPEED_card;

	upgrades[2].x = 300;
	upgrades[2].y = 0;
	upgrades[2].type = PORTAL_RANGE_card;

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
			AEGfxTextureSet(upgrades[0].Texture, 0, 0);
			AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

}

void upgrade_update() {

	// ----- Open upgrade screen -----
	if (player.justLeveledUp) {
		player.justLeveledUp = FALSE;	// Reset bool
		isUpgradeTime = TRUE;			// Enable UpgradeTime!!
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON) && isUpgradeTime) {
		for (s32 i = 0; i < MAX_UPGRADES; ++i) {
			switch (upgrades[i].type)
			case HP_card:
			case MOVEMENT_SPEED_card:
			case PORTAL_RANGE_card:
			default: break;
		}
		// ----- Close upgrade screen -----
		isUpgradeTime = FALSE;
	}

}




void draw_upgrade_cards() {

	//if (player.Level == 1 && visible) {
		
		/*AEMtx33Scale(&scale, CARD_WIDTH, CARD_HEIGHT);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, c1.x, c1.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(card_texture, 0, 0);
		AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);
		
		AEMtx33Scale(&scale, CARD_WIDTH, CARD_HEIGHT);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, c2.x, c2.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(card_texture, 0, 0);
		AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);

		AEMtx33Scale(&scale, CARD_WIDTH, CARD_HEIGHT);
		AEMtx33Rot(&rotate, PI);
		AEMtx33Trans(&translate, c3.x, c3.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(card_texture, 0, 0);
		AEGfxMeshDraw(uMesh, AE_GFX_MDM_TRIANGLES);

		t1 = (s8*)"upgrade";
		t2 = (s8*)"upgrade";
		t3 = (s8*)"upgrade";
		AEGfxPrint(Albam_fontID, t1, -0.7f, 0.f, 0.5f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(Albam_fontID, t2, -0.1f, c2.y/100.f, 0.5f, 0.0f, 0.0f, 0.0f);
		AEGfxPrint(Albam_fontID, t3, 0.5f, c3.y/100.f, 0.5f, 0.0f, 0.0f, 0.0f);*/

		
	//}
}

void update_upgrade_cards() {

	
	if ((player.Level != 0) && !visible ) {
		visible = true;
		idle_time -= AEFrameRateControllerGetFrameTime();
		if (idle_time > 0) {
			draw_upgrade_cards();
		}
	
		else {
			visible = false;
		}

		if (!visible) idle_time = 2.f;
	}
}