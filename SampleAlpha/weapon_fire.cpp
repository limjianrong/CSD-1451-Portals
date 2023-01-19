#include "AEEngine.h"

#include "weapon_fire.hpp"
#include "Player.hpp"
#include <iostream>

struct xy_position {
	int x, y;
};

AEGfxVertexList* shootMesh;
//f32 rotation{ 60 };

void weapon_fire_initialise(void) {

	

	// create mesh
	/*AEGfxMeshStart();
	AEGfxVertexAdd(60.0f, -40.0f, 0xFFFF0000, 0.0f, 0.0f);
	shootMesh = AEGfxMeshEnd();*/

}

void weapon_fire (f32 player_x, f32 player_y) {

	// Get mouse's x and y positions relative to the window screen space
	s32 x_cursor, y_cursor;
	AEInputGetCursorPosition(&x_cursor, &y_cursor);
		// All x and y coordinates starts from top left corner of window
		// Changes background colour when globe is pressed with LEFT MOUSE BUTTON
		AEVec2 point;
		AEVec2 center;
		AEVec2Set(&point, (f32)x_cursor, (f32)y_cursor);
		AEVec2Set(&center, AEGfxGetWinMaxX() + player_x, AEGfxGetWinMaxY() - player_y);
		if (AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
			if (AETestPointToRect(&point, &center, 50.0f, 50.0f)) AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		}

		/*AEVec2 bullet_endpoint;
		AEVec2Set(&bullet_endpoint, (f32)x_cursor, (f32)y_cursor);
		AEVec2 bullet_startpoint;
		AEVec2Set(&bullet_endpoint, player_x, player_y);*/

		// Create a scale matrix that scales by 100 x and y
		//AEMtx33 scale = { 0 };
		//AEMtx33Scale(&scale, 100.f, 100.f);

		//// Create a rotation matrix that rotates by rotation degrees
		//AEMtx33 rotate = { 0 };
		//AEMtx33Rot(&rotate, rotation);


		//AEMtx33 translate = { 0 };
		//AEMtx33Trans(&translate, player_x, player_y);
		//// Concat the matrices (TRS)
		//AEMtx33 transform = { 0 };
		//AEMtx33Concat(&transform, &rotate, &scale);
		//AEMtx33Concat(&transform, &translate, &transform);
		//// Choose the transform to use
		//AEGfxSetTransform(transform.m);


		//// Actually drawing the mesh 
		//AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);




		/*f32 opp, adj;
		opp = bullet_startpoint.y - bullet_endpoint.y;
		adj = bullet_endpoint.x - bullet_startpoint.x;


		AEVec2 final_position;
		AEVec2Scale(&final_position, &opp, adj);

		xy_position bullet_endpoint;
		bullet_endpoint.x = x_cursor;
		bullet_endpoint.y = y_cursor;

		xy_position bullet_startpoint;
		bullet_startpoint.x = player_x;
		bullet_startpoint.y = player_y;*/


}