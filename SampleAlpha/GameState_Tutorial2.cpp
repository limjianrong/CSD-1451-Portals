#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "Enemy3.hpp"
#include "boss.hpp"
#include "GameState_Tutorial2.hpp"

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;

static AEGfxTexture* assets[asset3_count];

// --- External variables ---
extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID; // text font
extern AEVec2 origin; // center coordinates of screen
extern AEVec2 center_cursor; // cursor coordinates 
extern Boss boss;

// button positions
extern f32 buttonX, buttonY;
// top row asset positions
extern f32 assetX, assetY;


AEVec2 asset3_scale[asset3_count];

void GameStateTutorial2Load(void) {
	// buttons
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	// background
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest_resized.png");
	// enemy 3
	assets[enemy3] = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/wingMan3.png");
	// enemy 3 warning symbol
	assets[enemy3_warning] = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/powerup_wings.png");
	// boss
	assets[_boss] = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_still_stand.png");

	mesh_load();
}
void GameStateTutorial2Init(void) {

	asset3_scale[enemy3] = { ENEMY3_WIDTH, ENEMY3_HEIGHT };
	asset3_scale[enemy3_warning] = { WARNING_WIDTH, WARNING_HEIGHT };
	asset3_scale[_boss] = { boss.width/3, boss.height/3 };

}

void GameStateTutorial2Update(void) {
	// get cursor coordinates
	variables_update();

	// handle clicking of buttons 
	for (s32 i = 0; i <= WINDOWLENGTH_X / 2; i += WINDOWLENGTH_X / 2) {
		if (AEInputCheckReleased(AEVK_LBUTTON) &&
			(center_cursor.x >= buttonX - WINDOWLENGTH_X / 6 + i && center_cursor.x <= buttonX + WINDOWLENGTH_X / 6 + i &&
				center_cursor.y >= buttonY - WINDOWLENGTH_Y / 16 &&
				center_cursor.y <= buttonY + WINDOWLENGTH_Y / 16)) {
			if (i == 0) gGameStateNext = GS_MainMenu;
			else if (i == WINDOWLENGTH_X / 2) gGameStateNext = GS_Tutorial;
		}
	}
}
void GameStateTutorial2Draw(void) {
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
	for (int i = 0; i <= WINDOWLENGTH_X / 2; i += WINDOWLENGTH_X / 2) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 3, WINDOWLENGTH_Y / 8); // button scale
		AEMtx33Trans(&translate, buttonX + i, buttonY); // x = center, start counting y from bottom of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		if (center_cursor.x >= buttonX - WINDOWLENGTH_X / 6 + i && center_cursor.x <= buttonX + WINDOWLENGTH_X / 6 + i &&
			center_cursor.y >= buttonY - WINDOWLENGTH_Y / 16 &&
			center_cursor.y <= buttonY + WINDOWLENGTH_Y / 16)
			AEGfxTextureSet(buttonPressed, 0, 0);
		else AEGfxTextureSet(buttonNotPressed, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// ------- Drawing of top row asset mesh + Setting texture -------
	for (int j = 0; j <= 2; j += 1) {
		AEMtx33Scale(&scale, asset3_scale[j].x, asset3_scale[j].y); // unique asset scale
		AEMtx33Trans(&translate, assetX + j * (WINDOWLENGTH_X / 3), assetY); // draws assets WINDOWXLENGTH_X / 3 apart
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(assets[j], 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}



	// ------ Texts ------

	// ----- Buttons -----
	AEGfxPrint(Albam_fontID, (s8*)"Main Menu", -0.7, -0.83, 0.95F, 1, 1, 1);
	AEGfxPrint(Albam_fontID, (s8*)"Previous", 0.33, -0.83, 0.95F, 1, 1, 1);

	// ----- Enemy 3 -----
	AEGfxPrint(Albam_fontID, (s8*)"Flying enemy", -0.85, 0.43, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"matches your", -0.86, 0.28, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"y-position while", -0.88, 0.13, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"it is off screen", -0.87, -0.02, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"and flies along it.", -0.9, -0.17, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"Avoid contact", -0.88, -0.32, 0.75F, 0, 0, 0);


	// ----- Warning symbol -----
	AEGfxPrint(Albam_fontID, (s8*)"Appears when", -0.21, 0.43, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"flying enemy is near", -0.28, 0.28, 0.75F, 0, 0, 0);

	// ----- Boss -----
	AEGfxPrint(Albam_fontID, (s8*)"Boss shoots", 0.47, 0.43, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"bullets and", 0.48, 0.28, 0.75F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"laser beams", 0.47, 0.13, 0.75F, 0, 0, 0);

}

void GameStateTutorial2Free(void) {

}
void GameStateTutorial2Unload(void) {
	// Button texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);
	AEGfxTextureUnload(assets[enemy3]);
	AEGfxTextureUnload(assets[enemy3_warning]);
	AEGfxTextureUnload(assets[_boss]);

	
	
	// Mesh free
	AEGfxMeshFree(square_mesh);

	// Informing the system about the loop's end
	AESysFrameEnd();
}
