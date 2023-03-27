#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Enemy.hpp"
#include "Utilities.hpp"
#include "GameState_Tutorial.hpp"

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;

static AEGfxTexture* assets[asset_count];
static AEGfxTexture* assets2[asset2_count];

// --- External variables ---
extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID; // text font
extern AEVec2 origin;		 // center coordinates of screen
extern AEVec2 center_cursor; // cursor coordinates 
// button positions
f32 buttonX, buttonY;
// top row asset positions
f32 assetX, assetY;
// bottom row asset positions
f32 asset2X, asset2Y;

AEVec2 asset2_scale[asset2_count];

void GameStateTutorialLoad(void) {
	// buttons
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	// background
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");
	// player
	assets[_player] = AEGfxTextureLoad("Assets/jumperpack/PNG/Players/bunny1_stand.png");
	// portals
	assets[portals] = AEGfxTextureLoad("Assets/card.png");
	// range
	assets[range] = AEGfxTextureLoad("Assets/greencircle.png");
	// enemy1
	assets2[enemy1] = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/spikeMan_stand.png");
	// enemy2
	assets2[enemy2] = AEGfxTextureLoad("Assets/bat.png");

	mesh_load();
}
void GameStateTutorialInit(void) {
	buttonX = origin.x - WINDOWLENGTH_X / 4; //length 1/3 od x screen
	buttonY = origin.y - WINDOWLENGTH_Y / 2.5; //heoght 1/8 of y screen

	assetX = origin.x - WINDOWLENGTH_X / 3;
	assetY = origin.y + WINDOWLENGTH_Y / 3;

	asset2X = origin.x - WINDOWLENGTH_X / 6;
	asset2Y = origin.y;

	//asset_scale[_player] = { player.width, player.height };
	//asset_scale[portals] = { 40.f, 40.f };
	//asset_scale[range] = { 40.f, 40.f };
	asset2_scale[enemy1] = { ENEMY1_WIDTH, ENEMY1_HEIGHT };
	asset2_scale[enemy2] = { ENEMY2_WIDTH, ENEMY2_HEIGHT };
}

void GameStateTutorialUpdate(void) {
	// get cursor coordinates
	variables_update();

	// handle clicking of buttons 
	for (s32 i = 0; i <= WINDOWLENGTH_X/2; i += WINDOWLENGTH_X/2) {
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			(center_cursor.x >= buttonX - WINDOWLENGTH_X / 6 + i && center_cursor.x <= buttonX + WINDOWLENGTH_X / 6 + i &&
			center_cursor.y >= buttonY - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= buttonY + WINDOWLENGTH_Y / 16)) {
			if (i == 0) gGameStateNext = GS_MainMenu;
			else if (i == WINDOWLENGTH_X/2) gGameStateNext = GS_Tutorial2;
		}
	}
}
void GameStateTutorialDraw(void) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEMtx33Scale(&scale, WINDOWLENGTH_X, WINDOWLENGTH_Y);
	AEMtx33Trans(&translate, origin.x, origin.y);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(backgroundTex, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// ------- Drawing of button mesh + Setting texture -------
	for (int i = 0; i <= WINDOWLENGTH_X/2; i += WINDOWLENGTH_X/2) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 3 , WINDOWLENGTH_Y / 8); // button scale
		AEMtx33Trans(&translate, buttonX + i, buttonY); // x = center, start counting y from bottom of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		if (center_cursor.x >= buttonX - WINDOWLENGTH_X/6 + i && center_cursor.x <= buttonX + WINDOWLENGTH_X/6 + i &&
			center_cursor.y >= buttonY - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= buttonY + WINDOWLENGTH_Y / 16)
			AEGfxTextureSet(buttonPressed, 0, 0);
		else AEGfxTextureSet(buttonNotPressed, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// ------- Drawing of top row asset mesh + Setting texture -------
	for (int j = 0; j <= 2; j += 1) {
		AEMtx33Scale(&scale, 70.f , 70.f); // button scale
		AEMtx33Trans(&translate, assetX + j*(WINDOWLENGTH_X/3), assetY); // x = center, start counting y from bottom of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(assets[j], 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// ------- Drawing of bottom row asset mesh + Setting texture -------
	for (int k = 0; k <= 1; k += 1) {
		AEMtx33Scale(&scale, asset2_scale[k].x, asset2_scale[k].y); // button scale
		AEMtx33Trans(&translate, asset2X + (k * (WINDOWLENGTH_X / 3)) % WINDOWLENGTH_X, asset2Y); // x = center, start counting y from bottom of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(assets2[k], 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// ------ Texts ------

	// ----- Buttons -----
	AEGfxPrint(Albam_fontID, (s8*)"Main Menu", -0.7, -0.83, 0.95F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Next", 0.39, -0.83, 0.95F, 1, 1, 1);
	
	// ----- Player -----
	AEGfxPrint(Albam_fontID, (s8*)"A for left", -0.8, 0.43, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"D for right", -0.81, 0.28, 0.75F, 0, 0, 0);

	// ----- Portal -----
	AEGfxPrint(Albam_fontID, (s8*)"Mouse right-click", -0.25, 0.43, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"to create portals", -0.28, 0.28, 0.75F, 0, 0, 0);

	// ----- Portal Range -----
	AEGfxPrint(Albam_fontID, (s8*)"Portal creation", 0.43, 0.43, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"range", 0.55, 0.28, 0.75F, 0, 0, 0);

	// ----- Enemy 1 -----
	AEGfxPrint(Albam_fontID, (s8*)"Avoid contact with", -0.67, -0.25, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"enemy located on", -0.66, -0.4, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"platforms", -0.53, -0.55, 0.75F, 0, 0, 0);

	// ----- Enemy 2 -----
	AEGfxPrint(Albam_fontID, (s8*)"Avoid bullets from", 0.07, -0.25, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"enemy", 0.25, -0.4, 0.75F, 0, 0, 0);
}

void GameStateTutorialFree(void) {

}
void GameStateTutorialUnload(void) {
	// Button texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	AEGfxTextureUnload(assets[_player]);
	AEGfxTextureUnload(assets[portals]);
	AEGfxTextureUnload(assets[range]);
	AEGfxTextureUnload(assets2[enemy1]);
	AEGfxTextureUnload(assets2[enemy2]);

	//unload all assets

	// Mesh free
	AEGfxMeshFree(square_mesh);

	// Informing the system about the loop's end
	AESysFrameEnd();
}
