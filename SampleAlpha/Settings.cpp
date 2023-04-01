#include "AEEngine.h"
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utilities.hpp"
#include "Settings.hpp"
#include "Audio.hpp"


// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh
static AEGfxTexture* buttonNotPressed, * buttonPressed, * backgroundTex;
AEGfxTexture* volume_bar, * volume_button;

// --- External variables ---
extern AEMtx33 scale, rotate, translate, transform;
extern s8 Albam_fontID;				// text font
extern AEVec2 origin;				// center coordinates of screen
extern AEVec2 center_cursor;		// cursor coordinates 
extern AEVec2 world_center_cursor;	// for calculating button offset

// --- Settings variables ---
bool fullscreen = { false };		// game launches in non-fullscreen mode
AEVec2 vbutton, vbar;				// vector for volume button & bar coordinates
float buttonscalex, buttonscaley;	// x and y scale of volume button
float barscalex, barscaley;			// x and y scale of volume bar
bool volume_adjusted;				// if TRUE, volume has been adjusted by player
AEVec2 button_offset;				// distance offset of button from bar center

// --- Constants ---
f64 vert_pos_offset{ 150.f };		// y-position of volume bar to above origin.y by 150.f

void settings_load(void) {

	buttonNotPressed = AEGfxTextureLoad("Assets/blue_button04.png");
	buttonPressed = AEGfxTextureLoad("Assets/blue_button05.png");
	backgroundTex = AEGfxTextureLoad("Assets/backgroundForest.png");

	volume_bar = AEGfxTextureLoad("Assets/volume_bar.png");
	volume_button = AEGfxTextureLoad("Assets/slider_button.png");

	mesh_load();
}


void settings_init(void) {
	// initialize volume button coordinates and scale
	vbutton.x = origin.x;
	vbutton.y = origin.y + vert_pos_offset;
	buttonscalex = WINDOWLENGTH_X / 27.f;
	buttonscaley = WINDOWLENGTH_Y / 18.f;

	// initialize volume bar coordinates and scale
	vbar.x = origin.x;
	vbar.y = origin.y + vert_pos_offset;
	barscalex = WINDOWLENGTH_X / 2.f;
	barscaley = WINDOWLENGTH_Y / 80.f;
}

void settings_update(void) {

	// get cursor coordinates
	variables_update();

	// if full screen button pressed
	if (AEInputCheckReleased(AEVK_LBUTTON) &&
		center_cursor.x >= -WINDOWLENGTH_X / 6.f && center_cursor.x <= WINDOWLENGTH_X / 6.f &&
		center_cursor.y >= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 15 - WINDOWLENGTH_Y / 16.f &&
		center_cursor.y <= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * 15 + WINDOWLENGTH_Y / 16.f) {
		fullscreen = !fullscreen;

		AESysToggleFullScreen(fullscreen);
	}
	
	// update volume bar position according to center origin
	vbar.x = origin.x;
	vbar.y = origin.y + vert_pos_offset;
}

void settings_draw(void) {
	// get cursor and center origin coordinates
	variables_update();
	 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTintColor(1, 1, 1, 1.0f);

	// ------- Background -------
	AEGfxTextureSet(backgroundTex, 0, 0);
	drawMesh(AEVec2{ WINDOWLENGTH_X, WINDOWLENGTH_Y }, origin, PI);

	// ------- Drawing of mesh + Setting texture -------
	for (int i = 15; i <= 27; i += 12) {
		AEMtx33Scale(&scale, WINDOWLENGTH_X / 3.f, WINDOWLENGTH_Y / 8.f); // scale of button
		AEMtx33Trans(&translate, origin.x, origin.y + WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * i); // x = screen center, start counting y from bottom of screen
		AEMtx33Rot(&rotate, PI); // rotation
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		if (center_cursor.x >= -WINDOWLENGTH_X / 6.f && center_cursor.x <= WINDOWLENGTH_X / 6.f &&
			center_cursor.y >= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * i - WINDOWLENGTH_Y / 16.f &&
			center_cursor.y <= WINDOWLENGTH_Y / 2.f - WINDOWLENGTH_Y / 30.f * i + WINDOWLENGTH_Y / 16.f)
			AEGfxTextureSet(buttonPressed, 0, 0); 
		else AEGfxTextureSet(buttonNotPressed, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	if (AETestPointToRect(&center_cursor, &vbutton, barscalex, barscaley * 5) && AEInputCheckReleased(AEVK_LBUTTON)) {
		volume_adjusted = TRUE;
		
		// calculate offset distance
		AEVec2Sub(&button_offset, &world_center_cursor, &origin);
		
		// button x coordinates to never exceed volume bar edges
		//if (vbutton.x < -AEGetWindowWidth() / 4.f + origin.x) vbutton.x = -AEGetWindowWidth() / 4.f + origin.x;
		//if (vbutton.x > AEGetWindowWidth() / 4.f + origin.x) vbutton.x = AEGetWindowWidth() / 4.f + origin.x;
	}
  

	//draw volume bar
	AEMtx33Scale(&scale, barscalex, barscaley);
	AEMtx33Trans(&translate, vbar.x, vbar.y);
	AEMtx33Rot(&rotate, 0);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(volume_bar, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// default button position
	if (volume_adjusted == FALSE) {
		//draw volume button
		AEMtx33Scale(&scale, buttonscalex, buttonscaley);
		AEMtx33Trans(&translate, origin.x, origin.y + vert_pos_offset);
		AEMtx33Rot(&rotate, 0);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxTextureSet(volume_button, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

	
	//draw updated volume button position 
	AEMtx33Scale(&scale, buttonscalex, buttonscaley);
	AEMtx33Trans(&translate, origin.x + button_offset.x, origin.y + vert_pos_offset);
	AEMtx33Rot(&rotate, 0);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(volume_button, 0, 0);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);

	// text on buttons
	AEGfxPrint(Albam_fontID, (s8*)"Full Screen", (f32) - 0.2,(f32) - 0.05, (f32)0.95F, (f32)1,(f32) 1, (f32)1);
	AEGfxPrint(Albam_fontID, (s8*)"Back", (f32) - 0.1, (f32) - 0.3, (f32)0.95F, (f32)1, (f32)1, (f32)1);

}
void settings_free() {

}

void settings_unload(void) {

	//// FontID unload
	//AEGfxDestroyFont(Albam_fontID);

	// Button texture unload
	AEGfxTextureUnload(buttonNotPressed);
	AEGfxTextureUnload(buttonPressed);
	AEGfxTextureUnload(backgroundTex);
	
	// Volume texture unload
	AEGfxTextureUnload(volume_bar);
	AEGfxTextureUnload(volume_button);

	// Mesh free
	//AEGfxMeshFree(square_mesh);

	// Informing the system about the loop's end
	//AESysFrameEnd();
}