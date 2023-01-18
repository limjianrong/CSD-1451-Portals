#include "AEEngine.h"

#include "Player.hpp"
#include <iostream> //for std::cout
int playersize{ 200 };

namespace portal {
	AEMtx33 rotation{};
	AEMtx33 translate{};
	AEMtx33 finalportal{};
	f32 x{ -100.0 };
	f32 y{ 150.0 };
	AEVec2 center{ portal::x, portal::y };
	AEVec2* pointertoportalcenter{&center};
	f32 rotateby{0};
	
	 
}

f32 rotation{ 0 };
f32 x{  }, y{  };
AEGfxVertexList* pMesh;
AEGfxVertexList* trianglemesh;
AEGfxVertexList* portalmesh;
AEGfxVertexList* hexagonmesh;
AEVec2 player_center{ x,y };
AEVec2* pointertoplayercenter{&player_center};
AEGfxTexture* yellowball;
AEGfxTexture* pTex;
void initialize_player(int playersize) { //PLAYERSIZE is not used for now

	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	yellowball = AEGfxTextureLoad("Assets/ball9.png");

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();
	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	// Saving the mesh (list of triangles) in pMesh
	trianglemesh = AEGfxMeshEnd();

}

void draw_player(int playersize) {
	

	// Create a scale matrix that scales by 100 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 50.f, 50.f);

	// Create a rotation matrix that rotates by rotation degrees
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, rotation);

	
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, x, y);
	// Concat the matrices (TRS)
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	// Choose the transform to use
	AEGfxSetTransform(transform.m);

	// Set the texture to pTex
	AEGfxTextureSet(pTex, 0, 0);
	
	// Actually drawing the mesh 
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Call player movement function so x & y values can be translated (to be able to move)
	player_movement();

	AEMtx33 scale2 {};
	AEMtx33Scale(&scale2, 100.0f, 100.0f);
	AEMtx33 translate2{};
	AEMtx33 finalform{};
	AEMtx33Trans(&translate2, 100.0f, 100.0f);
	AEMtx33Concat(&finalform, &scale2, &translate2);
	AEGfxSetTransform(finalform.m);
	
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(pTex, 0, 0);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Get mouse's x and y positions relative to the window screen space
	s32 x_cursor, y_cursor;
	AEInputGetCursorPosition(&x_cursor, &y_cursor);

//------------------------------- CAN IGNORE -------------------------------------------------------------------------
	// All x and y coordinates starts from top left corner of window
	// Changes background colour when globe is pressed with LEFT MOUSE BUTTON
	AEVec2 point;
	AEVec2 center;
	AEVec2Set(&point, x_cursor, y_cursor);
	AEVec2Set(&center, AEGfxGetWinMaxX() + x, AEGfxGetWinMaxY() - y);
	if (AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
		if (AETestPointToRect(&point, &center, 50.0f, 50.0f)) AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	}
//------------------------------- CAN IGNORE -------------------------------------------------------------------------

}

void player_movement(void) {
	// A key pressed
	if (AEInputCheckCurr(AEVK_A)) {
		x -= 1;
		rotation += 0.1f;
	}
	// D key pressed
	else if (AEInputCheckCurr(AEVK_D)) {
		x += 1;
		rotation -= 0.1f;
	}
	// W key pressed (No rotation)
	if (AEInputCheckPrev(AEVK_W) && AEInputCheckCurr(AEVK_W)) y += 5;
	// S key pressed (No rotation)
	else if (AEInputCheckPrev(AEVK_S) && AEInputCheckCurr(AEVK_S)) y -= 5;

}

void initialize_player_portal(void) {
	AEGfxMeshStart();
	AEGfxTriAdd(
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f,30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	/*AEGfxTriAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f,
		-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f,
		30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);*/
	AEGfxVertexAdd(-30.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(30.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	portalmesh = AEGfxMeshEnd();
	
	
	//drawing a hexagon mesh
	/*AEGfxMeshStart();
	AEGfxVertexAdd(10.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(30.0f, 10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(30.0f, -10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(10.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-10.0f, -30.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-30.0f, -10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-30.0f, 10.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxVertexAdd(-10.0f, 30.0f, 0xFFFF0000, 0.0f, 0.0f);
	
	hexagonmesh = AEGfxMeshEnd();*/

	
}
void player_portal(void) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//AEMtx33Rot(&portal::rotation, portal::rotateby);
	//AEMtx33Trans(&portal::translate, -100.0f, 150.0f);
	//AEMtx33Concat(&portal::finalportal, &portal::translate, &portal::rotation); //order is impt! translate -> rotation =/= rotation -> translate
	//AEGfxSetTransform(portal::finalportal.m);
	AEGfxSetPosition(portal::x, portal::y); //careful! setposition affects the rotation of the object that is drawn to output
	AEGfxMeshDraw(portalmesh, AE_GFX_MDM_TRIANGLES);
	if (portal::rotateby <= 360.0) {
		portal::rotateby += 0.01f;
	}

	if(AETestRectToRect(portal::pointertoportalcenter, 60.0f, 60.0f, pointertoplayercenter, 50.0f, 50.0f)) {
		portal::x = 500.0f;
		std::cout << "collision detected" << std::endl;
	}

	//AEGfxMeshDraw(hexagonmesh, AE_GFX_MDM_LINES_STRIP);
	/*if (AEInputCheckCurr(AEVK_M)) {
		rotation += 10;
	}*/
}

void check_player_and_portal_collide(void) {
	AETestRectToRect(portal::pointertoportalcenter, 60.0f, 60.0f, pointertoplayercenter, 50.0f, 50.0f);

}