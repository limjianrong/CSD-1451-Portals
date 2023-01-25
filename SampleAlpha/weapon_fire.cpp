#include "AEEngine.h"

#include "weapon_fire.hpp"
#include "Player.hpp"
#include <math.h>
#include <iostream>


AEGfxVertexList* shootMesh;
s32 x_cursor, y_cursor;
AEVec2 cursor;
AEVec2 center;

int direction_x, direction_y;
//int state;
f32 bullet_x;
f32 bullet_y;
f32 adj, opp;

enum Bullet_Direction { LEFT, RIGHT, UP, DOWN };

void bullet_initialise(void) {

	// create mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFC3209E, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFC3209E, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFC3209E, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFC3209E, 0.0f, 1.0f);
	//AEGfxVertexAdd(-1.f, -1.f, 0xFFC3209E, 0.0f, 1.0f);
	//AEGfxVertexAdd(-1.f, -2.f, 0xFFC3209E, 0.0f, 1.0f);
	shootMesh = AEGfxMeshEnd();

}
// parameters are x & y values that are being translated
void weapon_fire (f32 player_x, f32 player_y, int state) {

	if (state == 1) {
		bullet_x = player_x;
		bullet_y = player_y;
		//state = 0;

		// Get mouse's x and y positions relative to the window screen space
		s32 x_cursor, y_cursor;
		AEInputGetCursorPosition(&x_cursor, &y_cursor);

		// All x and y coordinates starts from top left corner of window
		// Changes background colour when globe is pressed with LEFT MOUSE BUTTON
		AEVec2 cursor; // cursor coords
		AEVec2 center; // player coords
		AEVec2Set(&cursor, x_cursor, y_cursor);
		AEVec2Set(&center, AEGfxGetWinMaxX() + player_x, AEGfxGetWinMaxY() - player_y);
		if (AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
			if (AETestPointToRect(&cursor, &center, 50.0f, 50.0f)) AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		}


		if (cursor.y >= center.y) {		// cursor below player
			opp = cursor.y - center.y;
			direction_y = DOWN;
		}
		else if (cursor.y <= center.y) {	// cursor above player	
			opp = center.y - cursor.y;
			direction_y = UP;
		}

		if (cursor.x >= center.x) {		// cursor right of player
			adj = cursor.x - center.x;
			direction_x = RIGHT;
		}
		else if (cursor.x <= center.x) {	// cursor left of player
			adj = center.x - cursor.x;
			direction_x = LEFT;
		}

		//f64 length = sqrt(adj * adj + opp * opp);
		f64 angle = AEATan(opp / adj); // get angle between cursor & player (in rad)
		AEVec2 normalized_vector;
		AEVec2Set(&normalized_vector, AECos(angle), AESin(angle));


		if (cursor.y >= center.y) bullet_y -= normalized_vector.y * 5;
		else if (cursor.y <= center.y) bullet_y += normalized_vector.y * 5;
		if (cursor.x >= center.x) bullet_x += normalized_vector.x * 5;
		else if (cursor.x <= center.x) bullet_x -= normalized_vector.x * 5;



		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEMtx33 weapon_scale = { 0 };
		AEMtx33Scale(&weapon_scale, 20.f, 20.f); // scaling it up
		AEMtx33 weapon_translate = { 0 };
		AEMtx33Trans(&weapon_translate, bullet_x, bullet_y); // shifts along x & y axis
		AEMtx33 weapon_rotate = { 0 };
		AEMtx33Rot(&weapon_rotate, angle); // rotation

		AEMtx33 weapon_transform = { 0 };
		AEMtx33Concat(&weapon_transform, &weapon_rotate, &weapon_scale);
		AEMtx33Concat(&weapon_transform, &weapon_translate, &weapon_transform);

		AEGfxSetTransform(weapon_transform.m);
		AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);

		//printf("bulletX %lf cursorX %lf\n", bullet_x, cursor.x);
		//condition to reset state
		if (bullet_x >= cursor.x && bullet_y <= cursor.y && direction_y == UP && direction_x == RIGHT) { // TOP-RIGHT
			state = 0;
		}
		else if (bullet_x >= cursor.x && bullet_y >= cursor.y && direction_y == DOWN && direction_x == RIGHT) { // BOTTOM-RIGHT
			state = 0;
		}
		else if (bullet_x <= cursor.x && bullet_y <= cursor.y && direction_y == UP && direction_x == LEFT) { // BOTTOM-RIGHT
			state = 0;
		}
		else if (bullet_x <= cursor.x && bullet_y >= cursor.y && direction_y == DOWN && direction_x == LEFT) { // BOTTOM-RIGHT
			state = 0;
		}


		//while (bullet_x <= point.x && bullet_y <= point.y) {
			//if (AEInputCheckCurr(AEVK_LBUTTON)) bullet_x += 10;
			//if (AEInputCheckCurr(AEVK_RBUTTON)) bullet_y += 10;

	}

}

//void bullet_mesh(void) {
//
//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
//	AEMtx33 weapon_scale = { 0 };
//	AEMtx33Scale(&weapon_scale, 20.f, 20.f); // scaling it up
//	AEMtx33 weapon_translate = { 0 };
//	AEMtx33Trans(&weapon_translate, 100.f, 200.f); // also moving along x & y axis
//	AEMtx33 weapon_rotate = { 0 };
//	AEMtx33Rot(&weapon_rotate, 0); // rotation
//
//	AEMtx33 weapon_transform = { 0 };
//	AEMtx33Concat(&weapon_transform, &weapon_rotate, &weapon_scale);
//	AEMtx33Concat(&weapon_transform, &weapon_translate, &weapon_transform);
//
//	AEGfxSetTransform(weapon_transform.m);
//	//AEGfxSetPosition(x_cursor, y_cursor);
//	AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);
//
//}