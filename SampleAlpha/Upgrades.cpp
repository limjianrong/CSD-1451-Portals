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
card c1{ -300.f, 0.f };
card c2{ 0.f, 0.f };
card c3{ 300.f, 0.f };

bool visible{ false };

// ----- Mesh & Texture -----
extern AEMtx33 scale, rotate, translate, transform;
AEGfxVertexList* uMesh;
AEGfxTexture* card_texture;

f64 deltaTime{};
f64 lastFrameTime{};
f64 idle_time{ 2.f };

void upgrades_init() {
	card_texture = AEGfxTextureLoad("Assets/card.png");
	uMesh = create_Square_Mesh();

	deltaTime = 0.0f;
	lastFrameTime = AEFrameRateControllerGetFrameTime();

}


void draw_upgrade_cards() {
	// Tell the engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set the tint to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	
	if (player.Level == 1 && visible) {
		
		AEMtx33Scale(&scale, CARD_WIDTH, CARD_HEIGHT);
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
		AEGfxPrint(Albam_fontID, t3, 0.5f, c3.y/100.f, 0.5f, 0.0f, 0.0f, 0.0f);

		
	}
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