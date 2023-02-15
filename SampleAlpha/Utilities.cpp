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

s8 create_font() {
	return AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
}

AEVec2 get_cursor_center_position() {
	// Get mouse's x and y positions from TOP LEFT corner of display screen
	AEVec2 cursor; // cursor coords, origin is top left corner of screen
	s32 x_cursor, y_cursor;
	AEInputGetCursorPosition(&x_cursor, &y_cursor);
	AEVec2Set(&cursor, x_cursor, y_cursor);

	// X: right +ve, left -ve		Y: up +ve, down -ve
	AEVec2 center_cursor; // cursor coords, origin is middle of screen
	s32 x_middle_cursor = x_cursor - WINDOWXLENGTH / 2;
	s32 y_middle_cursor = WINDOWYLENGTH / 2 - y_cursor;
	AEVec2Set(&center_cursor, x_middle_cursor, y_middle_cursor);
	return center_cursor;
}