/*!**************************************************************************************************
\file     Utilities.cpp
\author   Lin ZhaoZhi
\par      DP email: z.lin@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file implements the functions used to initialize and bullet mechanics.

  The function includes:
  - create_Square_Mesh
	Creates a square mesh of width 1 and height 1
*****************************************************************************************************/
#include "AEEngine.h"
#include "Utilities.hpp"

// --- Mesh ---
AEGfxVertexList* square_mesh;	// Created square mesh
// --- Mouse coords ---
AEVec2 cursor;					// Origin at TOP LEFT corner of window
AEVec2 center_cursor;			// Origin is CENTER of window
AEVec2 world_center_cursor;		// Origin is CENTER of window
// --- Window ---
f32 originX, originY;			// Center of screen, no matter where the camera moves

/*!**************************************************************************************************
\brief
	Creates a mesh for usage to all function, able to draw after saving it to a variable with similar type (AEGfxVertexList)

\return
  Pointer to AEGfxVertexList. Saving it into a similar type allows it to be called and drawn
*******************************************************************************************************/
AEGfxVertexList* create_Square_Mesh() {
	AEGfxVertexList* create_mesh;
	AEGfxMeshStart();
	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures 0.0 - 1.0
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
	create_mesh = AEGfxMeshEnd();
	return create_mesh;
}

//AEVec2 get_cursor_center_position() {
//	// Get mouse's x and y positions from TOP LEFT corner of display screen
//	AEVec2 cursor; // cursor coords, origin is top left corner of screen
//	s32 x_cursor, y_cursor;
//	AEInputGetCursorPosition(&x_cursor, &y_cursor);
//	AEVec2Set(&cursor, x_cursor, y_cursor);
//
//	// X: right +ve, left -ve		Y: up +ve, down -ve
//	AEVec2 center_cursor; // cursor coords, origin is middle of screen
//	s32 x_middle_cursor = x_cursor - WINDOWLENGTH_X / 2;
//	s32 y_middle_cursor = WINDOWLENGTH_Y / 2 - y_cursor;
//	AEVec2Set(&center_cursor, x_middle_cursor, y_middle_cursor);
//	return center_cursor;
//}

void mesh_load() {
	AEGfxMeshStart();
	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures 0.0 - 1.0
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
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
	AEVec2Set(&cursor, x_cursor, y_cursor);

	// Origin is CENTER of window
	s32 x_middle_cursor = x_cursor - WINDOWLENGTH_X / 2;
	s32 y_middle_cursor = WINDOWLENGTH_Y / 2 - y_cursor;
	AEVec2Set(&center_cursor, x_middle_cursor, y_middle_cursor);
	
	// Origin is CENTER of window
	AEVec2Set(&world_center_cursor, center_cursor.x + AEGfxGetWinMinX() - (-WINDOWLENGTH_X / 2), center_cursor.y + AEGfxGetWinMinY() - (-WINDOWLENGTH_Y / 2));


	// --------- Gets CENTER origin of constantly moving screen ---------
	originX = AEGfxGetWinMinX() + WINDOWLENGTH_X / 2;
	originY = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2;
}
