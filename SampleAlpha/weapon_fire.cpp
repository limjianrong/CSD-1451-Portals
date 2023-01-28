#include "AEEngine.h"

#include "weapon_fire.hpp"
#include "Utilities.hpp"
#include "Player.hpp"
#include <math.h>
#include <iostream>

AEGfxVertexList* shootMesh;
AEGfxTexture* bulletA;
AEVec2 cursor; // cursor coords, origin is top left corner of screen
AEVec2 center_cursor; // cursor coords, origin is middle of screen
AEVec2 player_center; // player coords, origin is middle of screen
AEVec2 normalized_vector; // direction vector from player to cursor

int direction_x, direction_y;
int prevState;
bool isRunning;

f32 bullet_x, bullet_y;
f32 adj, opp;

enum Bullet_Direction { LEFT, RIGHT, UP, DOWN };

void bullet_initialise(void) {

	// load texture
	bulletA = AEGfxTextureLoad("Assets/simplified_png/PNG/Tiles/platformPack_tile011.png");
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
	AEVec2Set(&cursor, x_cursor, y_cursor);
	
	// X: right +ve, left -ve		Y: up +ve, down -ve
	s32 x_middle_cursor = x_cursor - WINDOWXLENGTH / 2;
	s32 y_middle_cursor = WINDOWYLENGTH / 2 - y_cursor;
	AEVec2Set(&center_cursor, x_middle_cursor, y_middle_cursor);
	AEVec2Set(&player_center, player_x, player_y);

	// Changes background colour when globe is pressed with LEFT MOUSE BUTTON
	if (AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
		if (AETestPointToRect(&center_cursor, &player_center, 50.0f, 50.0f)) AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	}

	if (center_cursor.y <= player_center.y) {		// cursor below player
		opp = center_cursor.y - player_center.y;
		direction_y = DOWN;
	}
	else if (center_cursor.y >= player_center.y) {	// cursor above player	
		opp = player_center.y - center_cursor.y;
		direction_y = UP;
	}

	if (center_cursor.x >= player_center.x) {		// cursor right of player
		adj = center_cursor.x - player_center.x;
		direction_x = RIGHT;
	}
	else if (center_cursor.x <= player_center.x) {	// cursor left of player
		adj = player_center.x - center_cursor.x;
		direction_x = LEFT;
	}

	f32 speed = 20.0f;
	f64 angle = AEATan(opp / adj); // get angle between cursor & player (in rad)
	AEVec2Set(&normalized_vector, AECos(angle), AESin(angle));
	AEVec2Scale(&normalized_vector, &normalized_vector, speed);

	// distance between player -> bullet, player -> cursor
	f32 dist_player2bullet = sqrt((bullet_x - player_center.x) * (bullet_x - player_center.x) + (bullet_y - player_center.y) * (bullet_y - player_center.y));
	f32 dist_player2cursor = sqrt((player_center.x - center_cursor.x) * (player_center.x - center_cursor.x) + (player_center.y - center_cursor.y) * (player_center.y - center_cursor.y));

	// if LEFT MOUSE clicked
	if (state == 1) {
		isRunning = TRUE;
		// using normalized_vector
		if ((bullet_x != player_center.x) && (bullet_y != player_center.y) && prevState == 0) {
			bullet_x = player_center.x;
			bullet_y = player_center.y;
			prevState = 1;
		}
		if (center_cursor.y <= player_center.y) bullet_y += normalized_vector.y;
		else if (center_cursor.y >= player_center.y) bullet_y -= normalized_vector.y;
		if (center_cursor.x >= player_center.x) bullet_x += normalized_vector.x;
		else if (center_cursor.x <= player_center.x) bullet_x -= normalized_vector.x;
		
		// draw bullet_mesh if between cursor & player
		// checking coordinates rather than distance for collision?
		if (((bullet_x <= center_cursor.x && bullet_y <= center_cursor.y && direction_y == UP && direction_x == RIGHT) ||
			(bullet_x <= center_cursor.x && bullet_y >= center_cursor.y && direction_y == DOWN && direction_x == RIGHT) ||
			(bullet_x >= center_cursor.x && bullet_y <= center_cursor.y && direction_y == UP && direction_x == LEFT) ||
			(bullet_x >= center_cursor.x && bullet_y >= center_cursor.y && direction_y == DOWN && direction_x == LEFT)) &&
			(bullet_x <= WINDOWXLENGTH / 2) && (bullet_x >= -(WINDOWXLENGTH / 2)) &&	//bullet within play screen
			(bullet_y <= WINDOWYLENGTH / 2) && (bullet_y >= -(WINDOWYLENGTH / 2)))
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
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
			// Set the texture
			AEGfxTextureSet(bulletA, 0, 0);
			AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);
		}
		else // loops bullet
		{ 
			bullet_x = player_center.x;
			bullet_y = player_center.y;
		}
	}

	// if LEFT MOUSE not clicked
	else if (state == 0) 
	{
		if ((dist_player2bullet < dist_player2cursor) && isRunning == TRUE) 
		{
			if (center_cursor.y <= player_center.y) bullet_y += normalized_vector.y;
			else if (center_cursor.y >= player_center.y) bullet_y -= normalized_vector.y;
			if (center_cursor.x >= player_center.x) bullet_x += normalized_vector.x;
			else if (center_cursor.x <= player_center.x) bullet_x -= normalized_vector.x;
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEMtx33 weapon_scale1 = { 0 };
			AEMtx33Scale(&weapon_scale1, 20.f, 20.f); // scaling it up
			AEMtx33 weapon_translate1 = { 0 };
			AEMtx33Trans(&weapon_translate1, bullet_x, bullet_y); // shifts along x & y axis
			AEMtx33 weapon_rotate1 = { 0 };
			AEMtx33Rot(&weapon_rotate1, angle); // rotation
			AEMtx33 weapon_transform1 = { 0 };
			AEMtx33Concat(&weapon_transform1, &weapon_rotate1, &weapon_scale1);
			AEMtx33Concat(&weapon_transform1, &weapon_translate1, &weapon_transform1);
			AEGfxSetTransform(weapon_transform1.m);
			// Set the texture to pTex
			AEGfxTextureSet(bulletA, 0, 0);
			AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);
		}
		else // if bullet reached cursor
		{ 
			isRunning = FALSE;
			bullet_x = player_center.x;
			bullet_y = player_center.y;
			prevState = 0;
		}

	}

}
