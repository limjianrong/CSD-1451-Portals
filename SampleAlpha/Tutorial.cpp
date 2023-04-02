#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Enemy.hpp"
#include "boss.hpp"
#include "Utilities.hpp"
#include "Tutorial.hpp"
#include <iostream>

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
extern AEVec2 world_center_cursor;
extern Boss boss;

// --- Audio ---
extern AEAudio buttonClickedAudio, buttonHoverAudio;
extern AEAudioGroup soundGroup;
static bool TisPressed;

// --- Buttons ---
f32 button_scaleX{ WINDOWLENGTH_X / 5 };		// width of button
f32 button_scaleY{ WINDOWLENGTH_Y / 12 };		// height of button
f32 buttonX, buttonY;							// button positions

// top row asset positions
f32 assetX, assetY;
// bottom row asset positions
f32 asset2X, asset2Y;
f32 asset_width{ 70.f };
f32 asset_height{ 70.f };

//AEVec2 asset2_scale[asset2_count];

void tutorial_load(void) {
	// buttons
	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	// background
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest_resized.png");
	// player
	assets[_player] = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Characters/platformChar_walk2.png");
	// portals
	assets[portals] = AEGfxTextureLoad("Assets/simplified-platformer-pack/PNG/Tiles/platformPack_tile023.png");
	// range
	assets[range] = AEGfxTextureLoad("Assets/greencircle.png");
	// enemy1
	assets[enemy1] = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0055.png");
	// enemy2
	assets2[enemy2] = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0051.png");
	// enemy3
	assets2[enemy3] = AEGfxTextureLoad("Assets/pixel-line-platformer/Tiles/tile_0053.png");
	// enemy3 warning
	assets2[enemy3_warning] = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/powerup_wings.png");
	//boss
	assets2[_boss] = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_fly.png");

	mesh_load();
}
void tutorial_init(void) {
	buttonX = origin.x;
	buttonY = origin.y - WINDOWLENGTH_Y / 2.5f;

	// first row assets to be drawn from assetX
	assetX = AEGfxGetWinMinX() + 200.f;
	// first row assets y-position
	assetY = AEGfxGetWinMinY() + WINDOWLENGTH_Y/2 + WINDOWLENGTH_Y / 2.3f;

	// second row assets to be drawn from asset2X
	asset2X = assetX;
	// second row assets y-position
	asset2Y = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2 + WINDOWLENGTH_Y / 10;

}

void tutorial_update(void) {
	// get cursor coordinates
	variables_update();

	// buttons located relative to origin
	buttonX = origin.x;
	buttonY = origin.y - WINDOWLENGTH_Y / 2.5f;

	// first row assets to be drawn from assetX
	assetX = AEGfxGetWinMinX() + 200.f;
	// first row assets y-position
	assetY = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2 + WINDOWLENGTH_Y / 2.3f;

	// second row assets to be drawn from asset2X
	asset2X = assetX;
	// second row assets y-position
	asset2Y = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2 + WINDOWLENGTH_Y / 10;
	
}
void tutorial_draw(void) {
	variables_update();

	// buttons located relative to origin
	buttonX = origin.x;
	buttonY = origin.y - WINDOWLENGTH_Y / 2.5f;

	// first row assets to be drawn from assetX
	assetX = AEGfxGetWinMinX() + 200.f;
	// first row assets y-position
	assetY = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2 + WINDOWLENGTH_Y / 2.3f;

	// second row assets to be drawn from asset2X
	asset2X = assetX;
	// second row assets y-position
	asset2Y = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2 + WINDOWLENGTH_Y / 10;

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ------- Background -------
	AEGfxTextureSet(backgroundTex, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

	// ------- Drawing of button mesh + Setting texture -------
	if (world_center_cursor.x >= buttonX - button_scaleX / 2 && world_center_cursor.x <= buttonX + button_scaleX / 2 &&
		world_center_cursor.y >= buttonY - button_scaleY / 2 &&
		world_center_cursor.y <= buttonY + button_scaleY / 2) 

		AEGfxTextureSet(buttonPressed, 0, 0);

	else AEGfxTextureSet(buttonNotPressed, 0, 0);
			
	drawMesh(AEVec2{ button_scaleX, button_scaleY }, AEVec2{ buttonX, buttonY }, PI);

	if (world_center_cursor.x >= buttonX - button_scaleX / 2 && world_center_cursor.x <= buttonX + button_scaleX / 2 &&
		world_center_cursor.y >= buttonY - button_scaleY / 2 &&
		world_center_cursor.y <= buttonY + button_scaleY / 2) {

		if (TisPressed == FALSE) {
			TisPressed = TRUE;
			AEAudioPlay(buttonHoverAudio, soundGroup, 0.25f, 1.f, 0);

		}
	}
	else {
		TisPressed = FALSE;
	}
	

	// ------- Drawing of top row asset mesh + Setting texture -------
	for (int j = 0; j < asset_count; j++) {
		AEGfxTextureSet(assets[j], 0, 0);
		drawMesh(AEVec2{ asset_width, asset_height }, AEVec2{ assetX + j * (WINDOWLENGTH_X / 4), assetY }, PI);
	}

	// ------- Drawing of bottom row asset mesh + Setting texture -------
	for (int k = 0; k < asset2_count; k++) {
		AEGfxTextureSet(assets2[k], 0, 0);

		if (k == asset_count) {
			//drawMesh(AEVec2{ asset_width, asset_height }, AEVec2{ asset2X + k * (WINDOWLENGTH_X / 4), asset2Y }, 0);
			AEMtx33Scale(&boss.scale, boss.dimensions.x, boss.dimensions.y);
			AEMtx33Trans(&boss.translate, boss.center.x, boss.center.y);
			AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);
			AEGfxSetTransform(boss.matrix.m);
			//AEGfxTextureSet(assets2[k], 0.0f, 0.0f);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}
		else {
			drawMesh(AEVec2{ asset_width, asset_height }, AEVec2{ asset2X + k * (WINDOWLENGTH_X / 4), asset2Y }, PI);

		}
	}


	// ------ Texts ------

	f32 firstText = 0.7f;	// texts for 1st row assets start from this y-position

	// ----- Button -----
	AEGfxPrint(Albam_fontID, (s8*)"BACK", -0.05f, -0.83f, 0.75F, 1.0f, 1.0f, 1.0f);
	
	// ----- Player -----
	AEGfxPrint(Albam_fontID, (s8*)"A for left.", -0.84f, firstText, 0.55f, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"D for right.", -0.845f, firstText - 0.1f, 0.55f, 0.0f, 0.0f, 0.0f);

	// ----- Portal -----
	AEGfxPrint(Albam_fontID, (s8*)"Right-click to create.", -0.4f, firstText, 0.55F, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"F to undo.", -0.32f, firstText - 0.1f, 0.55F, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"Teleports player, bullets,", -0.42f, firstText - 0.2f, 0.55F, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"and enemies.", -0.34f, firstText - 0.3f, 0.55F, 0.0f, 0.0f, 0.0f);

	// ----- Portal Range -----
	AEGfxPrint(Albam_fontID, (s8*)"Portal creation", 0.14f, firstText, 0.55F, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"range.", 0.21f, firstText - 0.1f, 0.55F, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"Appears red when", 0.13f, firstText - 0.2f, 0.55F, 0.0f, 0.0f, 0.0f);
	AEGfxPrint(Albam_fontID, (s8*)"portal creation disabled.", 0.1f, firstText - 0.3f, 0.55F, 0.0f, 0.0f, 0.0f);

	// ----- Enemy 1 -----
	AEGfxPrint(Albam_fontID, (s8*)"Avoid contact with", 0.62f, firstText, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"enemy.", 0.72f, firstText - 0.1f, 0.55F, 0, 0, 0);


	
	f32 secondText = 0.05f;	// texts for row 2 assets start from this y-position

	// ----- Enemy 2 -----
	AEGfxPrint(Albam_fontID, (s8*)"Avoid bullets from", -0.87f, secondText, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"enemy.", -0.79f, secondText - 0.1f, 0.55F, 0, 0, 0);

	// ----- Enemy 3 -----
	AEGfxPrint(Albam_fontID, (s8*)"Avoid contact with", -0.38f, secondText, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"enemy.", -0.3f, secondText - 0.1f, 0.55F, 0, 0, 0);

	// ------ Enemy 3 warning ------
	AEGfxPrint(Albam_fontID, (s8*)"Appears when enemy", 0.12f, secondText, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"is incoming.", 0.19f, secondText - 0.1f, 0.55F, 0, 0, 0);

	// ------ Boss ------
	AEGfxPrint(Albam_fontID, (s8*)"Boss shoots bullets", 0.62f, secondText, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"and lasers.", 0.67f, secondText - 0.1f, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"Charges towards player", 0.57f, secondText - 0.2f, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"occassionally.", 0.67f, secondText - 0.3f, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"Defeat the boss and", 0.6f, secondText - 0.4f, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"collide with the", 0.64f, secondText - 0.5f, 0.55F, 0, 0, 0);
	AEGfxPrint(Albam_fontID, (s8*)"door to win!", 0.66f, secondText - 0.6f, 0.55F, 0, 0, 0);
	
	AEGfxPrint(Albam_fontID, (s8*)"Press 'P' to pause game", -0.26f, secondText - 0.7f, 0.75F, 0.54f, 0, 1);


}

void tutorial_free(void) {

}

void tutorial_unload(void) {
	// Button texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);

	// Assets unload 
	AEGfxTextureUnload(assets[_player]);
	AEGfxTextureUnload(assets[portals]);
	AEGfxTextureUnload(assets[range]);
	AEGfxTextureUnload(assets[enemy1]);
	AEGfxTextureUnload(assets2[enemy2]);
	AEGfxTextureUnload(assets2[enemy3]);
	AEGfxTextureUnload(assets2[enemy3_warning]);
	AEGfxTextureUnload(assets2[_boss]);
}
