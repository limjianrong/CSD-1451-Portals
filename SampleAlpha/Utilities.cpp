/*==================================================================================
* All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Utilities.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lin ZhaoZhi, z.lin@digipen.edu
* Secondary Authors:	-
*
* Brief:
  This source file contains declaration of functions for global use in every source file.
  Contains only variable and functions that are commonly used in most source file, such
  as cursor position with origin being the center of window, cursor position with origin
  being top left corner of window, origin of window no matter the location in game .etc
==================================================================================*/
#include "AEEngine.h"
#include "Utilities.hpp"

// --- Mesh ---
AEGfxVertexList* square_mesh;	// Created square mesh
// --- Mouse coords ---
AEVec2 cursor;					// Origin at TOP LEFT corner of window
AEVec2 center_cursor;			// Origin is CENTER of window
AEVec2 world_center_cursor;		// Origin is CENTER of window
// --- Window ---
AEVec2 origin;					// Center of screen, no matter where the camera moves

void mesh_load() {
	AEGfxMeshStart();
	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures 0.0 - 1.0
	// Saving the mesh (list of triangles) in pMesh
		AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	square_mesh = AEGfxMeshEnd();
}

void variables_update() {

	// ------------------ MOUSE COORDS ------------------- //
	// ---------- Get mouse's x and y positions ---------- //
	// -- X: right +ve, left -ve	Y: up +ve, down -ve -- //
	// --------------------------------------------------- //

	// Origin at TOP LEFT corner of window
	s32 x_cursor, y_cursor;
	AEInputGetCursorPosition(&x_cursor, &y_cursor);
	AEVec2Set(&cursor, static_cast<f32>(x_cursor), static_cast<f32>(y_cursor));

	// Origin is CENTER of window
	s32 x_middle_cursor = x_cursor - WINDOWLENGTH_X / 2;
	s32 y_middle_cursor = WINDOWLENGTH_Y / 2 - y_cursor;
	AEVec2Set(&center_cursor, static_cast<f32>(x_middle_cursor), static_cast<f32>(y_middle_cursor));
	
	// Origin is CENTER of window
	AEVec2Set(&world_center_cursor, center_cursor.x + AEGfxGetWinMinX() - (-WINDOWLENGTH_X / 2.0f), center_cursor.y + AEGfxGetWinMinY() - (-WINDOWLENGTH_Y / 2.0f));


	// --------- Gets CENTER origin of constantly moving screen ---------
	origin.x = AEGfxGetWinMinX() + WINDOWLENGTH_X / 2;
	origin.y = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2;

}

void drawMesh(AEVec2 scale, AEVec2 trans, f32 rot)
{
	AEMtx33 transform, Scale, Rot, Trans;

	AEMtx33Scale(&Scale, scale.x, scale.y);
	AEMtx33Rot(&Rot, rot);
	AEMtx33Trans(&Trans, trans.x, trans.y);
	AEMtx33Concat(&transform, &Rot, &Scale);
	AEMtx33Concat(&transform, &Trans, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}

void GameObjects::RenderObject() {

	drawMesh(dimensions, center, NULL);
}

void GameObjects::Render_HealthBar() {

	// Draw enemies if alive
	if (Hp > 0 && status == TRUE) {
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTintColor(0, 0, 0, 1.f);
		//drawMesh(AEVec2{ 80.f, 15.f }, AEVec2{ center.x, center.y + dimensions.y / 2.f + 10.f }, PI);
		drawMesh(AEVec2{ dimensions.x , 15.f }, AEVec2{ center.x, center.y + dimensions.y / 2.f + 10.f }, NULL);

		f32 health_percentage = ((float)Hp / (float)Max_Hp) * 100.f;
		if (health_percentage >= 80.f) {
			AEGfxSetTintColor(0, 255, 0, 1.f);
		}
		else if (health_percentage >= 40.f) {
			AEGfxSetTintColor(255, 255, 0, 1.f);
		}
		else {
			AEGfxSetTintColor(255, 0, 0, 1.f);
		}
		drawMesh(AEVec2{ (float)Hp / (float)Max_Hp * dimensions.x , 15.f },
			AEVec2{ (float)center.x - (((float)Max_Hp - (float)Hp) / (float)Max_Hp * dimensions.x / 2.f), center.y + dimensions.y / 2.f + 10.f }, NULL);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	}
}

bool check_window_out_of_focus() {
	HWND hWnd = GetForegroundWindow(); // get handle to the foreground window
	HWND myWnd = FindWindow(NULL, "CSD 1451 Portals");

	if (hWnd != myWnd) // compare the foreground window to your program's window
	{
		return true;
	}
	return false;
}