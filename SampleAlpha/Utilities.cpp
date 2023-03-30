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
AEVec2 origin;					// Center of screen, no matter where the camera moves
//f32 originX, originY;			// Center of screen, no matter where the camera moves

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
	AEVec2Set(&cursor, static_cast<f32>(x_cursor), static_cast<f32>(y_cursor));

	// Origin is CENTER of window
	s32 x_middle_cursor = x_cursor - WINDOWLENGTH_X / 2;
	s32 y_middle_cursor = WINDOWLENGTH_Y / 2 - y_cursor;
	AEVec2Set(&center_cursor, static_cast<f32>(x_middle_cursor), static_cast<f32>(y_middle_cursor));
	
	// Origin is CENTER of window
	AEVec2Set(&world_center_cursor, center_cursor.x + AEGfxGetWinMinX() - (-WINDOWLENGTH_X / 2.0f), center_cursor.y + AEGfxGetWinMinY() - (-WINDOWLENGTH_Y / 2.0f));


	// --------- Gets CENTER origin of constantly moving screen ---------
	/*originX = AEGfxGetWinMinX() + WINDOWLENGTH_X / 2;
	originY = AEGfxGetWinMinY() + WINDOWLENGTH_Y / 2;*/

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

//bool CollisionIntersection_RectRect(Enemy1_stats& obj1, Player_stats& obj2) {
//
//	// ----- AABB min max check -----
//	// Check if first object's max point is smaller than second object's min point on the x axis
//	if ((obj2.center.x + obj2.width / 2.f) < (obj1.center.x - obj1.dimensions.x / 2.f))
//		return false;
//	// Check if first object's min point is larger than second object's max point on the x axis
//	if ((obj2.center.x - obj2.width / 2.f) > (obj1.center.x + obj1.dimensions.x / 2.f))
//		return false;
//	// Check if first object's max point is smaller than second object's min point on the y axis
//	if ((obj2.center.y + obj2.height / 2.f) < (obj1.center.y - obj1.dimensions.y / 2.f))
//		return false;
//	// Check if first object's min point is larger than second object's max point on the y axis
//	if ((obj2.center.y - obj2.height / 2.f) > (obj1.center.y + obj1.dimensions.y / 2.f))
//		return false;
//
//	return true;
//
//}

void GameObjects::RenderObject() {

	drawMesh(dimensions, center, PI);
}

void GameObjects::Render_HealthBar() {

	// Draw enemies if alive
	if (Hp > 0 && status == TRUE) {
		AEGfxTextureSet(NULL, 0, 0);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTintColor(0, 0, 0, 1.f);
		drawMesh(AEVec2{ 80.f, 15.f }, AEVec2{ center.x, center.y + dimensions.y / 2.f + 10.f }, PI);

		f32 health_percentage = ((float)Hp / (float)Max_Hp) * 100.f;
		/*if (health_percentage >= 80.f) {
			AEGfxSetTintColor(0.f, 1.f, 0.f, 1.f);
		}
		else if (health_percentage >= 40.f) {
			AEGfxSetTintColor(1.f, 1.f, 0.f, 1.f);
		}
		else {
			AEGfxSetTintColor(1.f, 0.f, 0.f, 1.f);
		}*/
		if (health_percentage >= 80.f) {
			AEGfxSetTintColor(0, 255, 0, 1.f);
		}
		else if (health_percentage >= 40.f) {
			AEGfxSetTintColor(255, 255, 0, 1.f);
		}
		else {
			AEGfxSetTintColor(255, 0, 0, 1.f);
		}
		drawMesh(AEVec2{ (float)Hp / (float)Max_Hp * 80.f , 15.f }, 
				 AEVec2{ (float)center.x - (((float)Max_Hp - (float)Hp) / (float)Max_Hp * 40.f), center.y + dimensions.y / 2.f + 10.f}, PI);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	}
}