#include "AEEngine.h"

#include "weapon_fire.hpp"
#include "Player.hpp"
#include <math.h>
#include <iostream>

struct xy_position {
	int x, y;
};

AEGfxVertexList* shootMesh;
s32 x_cursor, y_cursor;
AEVec2 point;
AEVec2 center;
//f32 rotation{ 60 };

void bullet_initialise(void) {

	// create mesh
	AEGfxMeshStart();
	/*AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);*/
	AEGfxVertexAdd(-1.f, -1.f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxVertexAdd(-1.f, -2.f, 0xFFC3209E, 0.0f, 1.0f);
	shootMesh = AEGfxMeshEnd();

}

void bullet_mesh(void) {

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEMtx33 weapon_scale = { 0 };
	AEMtx33Scale(&weapon_scale, 100.f, 100.f); // moving along x & y axis cuz a line
	AEMtx33 weapon_translate = { 0 };
	AEMtx33Trans(&weapon_translate, 100.f, 100.f); // also moving along x & y axis
	AEMtx33 weapon_rotate = { 0 };
	AEMtx33Rot(&weapon_rotate, 0); // rotation

	AEMtx33 weapon_transform = { 0 };
	AEMtx33Concat(&weapon_transform, &weapon_rotate, &weapon_scale);
	AEMtx33Concat(&weapon_transform, &weapon_translate, &weapon_transform);

	AEGfxSetTransform(weapon_transform.m);
	//AEGfxSetPosition(x_cursor, y_cursor);
	AEGfxMeshDraw(shootMesh, AE_GFX_MDM_LINES);
	//AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);

}

// parameters are x & y values that are being translated
void weapon_fire (f32 player_x, f32 player_y) {

	// Get mouse's x and y positions relative to the window screen space
	s32 x_cursor, y_cursor;
	AEInputGetCursorPosition(&x_cursor, &y_cursor);

	// All x and y coordinates starts from top left corner of window
	// Changes background colour when globe is pressed with LEFT MOUSE BUTTON
	AEVec2 point; // cursor coords
	AEVec2 center; // player coords
	AEVec2Set(&point, x_cursor, y_cursor);
	AEVec2Set(&center, AEGfxGetWinMaxX() + player_x, AEGfxGetWinMaxY() - player_y);
	if (AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
		if (AETestPointToRect(&point, &center, 50.0f, 50.0f)) AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	}

	//if (cursor is to right of player)
		f32 opp = point.y - center.y;
		f32 adj = point.x - center.x;
		f64 angle = AEATanDeg(opp / adj); // get angle between cursor & player
		AEVec2 normalized_angle;
		AEVec2Set(&normalized_angle, AECosDeg(angle), AESinDeg(angle));
		//printf("x = %lf  y = %lf\n", normalized_angle.x, normalized_angle.y);
		


	/*AEVec2 bullet_endpoint;
	AEVec2Set(&bullet_endpoint, (f32)x_cursor, (f32)y_cursor);
	AEVec2 bullet_startpoint;
	AEVec2Set(&bullet_endpoint, player_x, player_y);*/

		



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