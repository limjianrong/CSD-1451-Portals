#include "AEEngine.h"

#include "weapon_fire.hpp"
#include "Utilities.hpp"
#include "Player.hpp"
#include <math.h>
#include <iostream>

AEGfxVertexList* shootMesh;
s32 x_cursor, y_cursor;
AEVec2 cursor;
AEVec2 center;

int direction_x, direction_y;
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
	shootMesh = AEGfxMeshEnd();

}
// parameters are x & y values that are being translated
void weapon_fire (f32 player_x, f32 player_y, int state) {

	// Get mouse's x and y positions from TOP LEFT corner of display screen
	s32 x_cursor, y_cursor;
	AEInputGetCursorPosition(&x_cursor, &y_cursor);

	// All x and y coordinates starts from top left corner of window
	AEVec2 cursor; // cursor coords
	// X: right +ve, left -ve		Y: up +ve, down -ve
	s32 x_middle_cursor = x_cursor - WINDOWXLENGTH / 2;
	s32 y_middle_cursor = WINDOWYLENGTH / 2 - y_cursor;
	AEVec2 center; // player coords
	AEVec2Set(&cursor, x_middle_cursor, y_middle_cursor);
	AEVec2Set(&center, player_x, player_y);

	// Changes background colour when globe is pressed with LEFT MOUSE BUTTON
	if (AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
		if (AETestPointToRect(&cursor, &center, 50.0f, 50.0f)) AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	}

	if (cursor.y <= center.y) {		// cursor below player
		opp = cursor.y - center.y;
		direction_y = DOWN;
	}
	else if (cursor.y >= center.y) {	// cursor above player	
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

	// get time from prev frame in secs
	//AEFrameRateControllerGetFrameTime();

	f32 speed = 2000.0f;
	f64 angle = AEATan(opp / adj); // get angle between cursor & player (in rad)
	/*f32 length1 = adj * AECos(angle);
	f32 length2 = opp * AESin(angle);
	printf("%lf & %lf\n", length1, length2);*/
	AEVec2 normalized_vector;
	AEVec2Set(&normalized_vector, AECos(angle), AESin(angle));
	AEVec2Scale(&normalized_vector, &normalized_vector, speed * AEFrameRateControllerGetFrameTime());

	if (state == 1) {
		// using normalized_vector
		//printf("bulletY %lf centerY %lf\n", bullet_y, center.y);
		if (cursor.y <= center.y) bullet_y += normalized_vector.y;
		else if (cursor.y >= center.y) bullet_y -= normalized_vector.y;
		if (cursor.x >= center.x) bullet_x += normalized_vector.x;
		else if (cursor.x <= center.x) bullet_x -= normalized_vector.x;
		// draw bullet_mesh if between cursor & player
		if (((bullet_x <= cursor.x && bullet_y <= cursor.y && direction_y == UP && direction_x == RIGHT) ||
			(bullet_x <= cursor.x && bullet_y >= cursor.y && direction_y == DOWN && direction_x == RIGHT) ||
			(bullet_x >= cursor.x && bullet_y <= cursor.y && direction_y == UP && direction_x == LEFT) ||
			(bullet_x >= cursor.x && bullet_y >= cursor.y && direction_y == DOWN && direction_x == LEFT)) &&
			(bullet_x <= WINDOWXLENGTH / 2) && (bullet_x >= -(WINDOWXLENGTH / 2)) &&	//bullet within play screen
			(bullet_y <= WINDOWYLENGTH / 2) && (bullet_y >= -(WINDOWYLENGTH / 2)))
		{

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

		}
		else {
			bullet_x = center.x;
			bullet_y = center.y;
		}
	}
	else if (state == 0) {
		f32 dist_player2bullet = sqrt((bullet_x - center.x) * (bullet_x - center.x) + (bullet_y - center.y) * (bullet_y - center.y));
		f32 dist_player2cursor = sqrt((center.x - cursor.x) * (center.x - cursor.x) + (center.y - cursor.y) * (center.y - cursor.y));
		//if (dist_player2bullet > dist_player2cursor) {
		//	 //stop looping
		//}
		if (dist_player2bullet < dist_player2cursor) {

			if (cursor.y <= center.y) bullet_y += normalized_vector.y;
			else if (cursor.y >= center.y) bullet_y -= normalized_vector.y;
			if (cursor.x >= center.x) bullet_x += normalized_vector.x;
			else if (cursor.x <= center.x) bullet_x -= normalized_vector.x;
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
		}
		 
		 
		//if ((bullet_x >= center.x && bullet_y >= center.y && direction_y == UP && direction_x == RIGHT) ||
		//	(bullet_x >= center.x && bullet_y <= center.y && direction_y == DOWN && direction_x == RIGHT) ||
		//	(bullet_x <= center.x && bullet_y >= center.y && direction_y == UP && direction_x == LEFT) ||
		//	(bullet_x <= center.x && bullet_y <= center.y && direction_y == DOWN && direction_x == LEFT))
		//{
		//	if ((bullet_x >= cursor.x && bullet_y >= cursor.y && direction_y == UP && direction_x == RIGHT) ||
		//		(bullet_x >= cursor.x && bullet_y <= cursor.y && direction_y == DOWN && direction_x == RIGHT) ||
		//		(bullet_x <= cursor.x && bullet_y >= cursor.y && direction_y == UP && direction_x == LEFT) ||
		//		(bullet_x <= cursor.x && bullet_y <= cursor.y && direction_y == DOWN && direction_x == LEFT))
		//	{
		//		bullet_x = center.x;
		//		bullet_y = center.y;
		//		temp = FALSE;
		//	}

		//	if (cursor.y <= center.y) bullet_y += normalized_vector.y;
		//	else if (cursor.y >= center.y) bullet_y -= normalized_vector.y;
		//	if (cursor.x >= center.x) bullet_x += normalized_vector.x;
		//	else if (cursor.x <= center.x) bullet_x -= normalized_vector.x;

		//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//	AEMtx33 weapon_scale = { 0 };
		//	AEMtx33Scale(&weapon_scale, 20.f, 20.f); // scaling it up
		//	AEMtx33 weapon_translate = { 0 };
		//	AEMtx33Trans(&weapon_translate, bullet_x, bullet_y); // shifts along x & y axis
		//	AEMtx33 weapon_rotate = { 0 };
		//	AEMtx33Rot(&weapon_rotate, angle); // rotation

		//	AEMtx33 weapon_transform = { 0 };
		//	AEMtx33Concat(&weapon_transform, &weapon_rotate, &weapon_scale);
		//	AEMtx33Concat(&weapon_transform, &weapon_translate, &weapon_transform);

		//	AEGfxSetTransform(weapon_transform.m);
		//	AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);

		//}
	}

}
