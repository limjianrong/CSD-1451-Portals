/*!**************************************************************************************************
\file     weapon_fire.cpp
\author   Lin ZhaoZhi
\par      DP email: z.lin@digipen.edu
\par      Course: CSD 1451
\par      Software Engineering Project 2
\date     29-01-2023

\brief
  This source file implements the functions used to initialize and bullet mechanics.

  The function includes:
  - bullet_initialise
	Load texture, initialise mesh

  - weapon_fire
	Shoots bullet from player to cursor position
	Deals damage to enemy if bullet collides with enemy position

  - isbullet_enemy_colliding
	Returns true if bullet is colliding with enemy, else returns false
*****************************************************************************************************/
#include "AEEngine.h"

#include "weapon_fire.hpp"
#include "Utilities.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "boss.hpp"

// ----- Mesh & Texture -----
AEGfxVertexList* shootMesh;
AEGfxTexture* bulletA;

// ----- Cursor -----
AEVec2 center_cursor; // cursor coords, origin is middle of screen
AEVec2 player_center; // player coords, origin is middle of screen
AEVec2 normalized_vector; // direction vector from player to cursor

int direction_x, direction_y;
int prevState;
bool isRunning;

// ----- Objects -----
extern Enemy_stats enemy1, enemy2;
extern Player_stats player;
extern Boss boss;

f32 bullet_x, bullet_y;
f32 adj, opp;
f64 angle;
#define speed 15.0f

//enum Bullet_Direction { LEFT, RIGHT, UP, DOWN };


/*!**************************************************************************************************
\brief
	Loads texture being used for bullet
	Save created mesh into AEGfxVertexList
*******************************************************************************************************/
void bullet_initialise(void) {

	// load texture
	bulletA = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/gold_1.png");
	// create mesh
	shootMesh = create_Square_Mesh();

}

void bullet_update() {


	// ----------  Boss  ----------
	if (player.y <= boss.y_pos)			// Player below boss
		opp = player.y - boss.y_pos;

	else if (player.y >= boss.y_pos)	// Player above boss
		opp = player.y - boss.y_pos;

	if (player.x >= boss.x_pos)			// Player right of boss
		adj = player.x - boss.x_pos;

	else if (player.x <= boss.x_pos)	// Player left of boss
		adj = boss.x_pos - player.x;


	angle = AEATan(opp / adj); // get angle between cursor & player (in rad)
	AEVec2Set(&normalized_vector, AECos(angle), AESin(angle));
	AEVec2Scale(&normalized_vector, &normalized_vector, speed);

	// distance between player -> bullet, player -> cursor
	//f32 dist_player2bullet = sqrt((bullet_x - player_center.x) * (bullet_x - player_center.x) + (bullet_y - player_center.y) * (bullet_y - player_center.y));
	//f32 dist_player2cursor = sqrt((player_center.x - center_cursor.x) * (player_center.x - center_cursor.x) + (player_center.y - center_cursor.y) * (player_center.y - center_cursor.y));

	

	// ----- Movement of bullet from boss to player -----
	if (player.y <= boss.y_pos) bullet_y += normalized_vector.y;
	else if (player.y >= boss.y_pos) bullet_y -= normalized_vector.y;
	if (player.x >= boss.x_pos) bullet_x += normalized_vector.x;
	else if (player.x <= boss.x_pos) bullet_x -= normalized_vector.x;

	if (bullet_x == player.x && bullet_y == player.y) // loops bullet
	{
		bullet_x = boss.x_pos;
		bullet_y = boss.y_pos;
	}

	// ----- Bullet collision with enemy1 -----
	if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy1.x, enemy1.y) == TRUE) {
		bullet_x = player_center.x;
		bullet_y = player_center.y;
		--enemy1.Hp;
	}
	if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy2.x, enemy2.y) == TRUE) {
		bullet_x = player_center.x;
		bullet_y = player_center.y;
		--enemy2.Hp;
	}
}


void bullet_draw() {

	if (!(bullet_x == player.x && bullet_y == player.y))
	{
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
}

/*!**************************************************************************************************
\brief
	Gets player and cursor position and draws a bullet from player to cursor for every frame.
	When left clicked is pressed, bullet will shoot continuously.
	When left clicked is not pressed, last bullet shot from player will continue to travel towards cursor and disappear once it reaches cursor.

\param[in] player_x
	x coordinates of player

\param[in] player_y
	y coordinates of player

\param[in] state
	state of mouse button
	1 if left mouse button is pressed
	0 if left mouse button is not pressed
*******************************************************************************************************/
// parameters are x & y values that are being translated
//void weapon_fire (f32 player_x, f32 player_y, int state) {
//
//	// Get mouse's x and y positions from middle of display screen
//	center_cursor = get_cursor_center_position();
//	AEVec2Set(&player_center, player_x, player_y);
//
//	// Changes background colour when globe is pressed with LEFT MOUSE BUTTON
//	/*if (AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
//		if (AETestPointToRect(&center_cursor, &player_center, 50.0f, 50.0f)) AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
//	}*/
//
//	if (center_cursor.y <= player_center.y) {		// cursor below player
//		opp = center_cursor.y - player_center.y;
//		direction_y = DOWN;
//	}
//	else if (center_cursor.y >= player_center.y) {	// cursor above player	
//		opp = player_center.y - center_cursor.y;
//		direction_y = UP;
//	}
//
//	if (center_cursor.x >= player_center.x) {		// cursor right of player
//		adj = center_cursor.x - player_center.x;
//		direction_x = RIGHT;
//	}
//	else if (center_cursor.x <= player_center.x) {	// cursor left of player
//		adj = player_center.x - center_cursor.x;
//		direction_x = LEFT;
//	}
//
//	//f32 speed = 15.0f;
//	f64 angle = AEATan(opp / adj); // get angle between cursor & player (in rad)
//	AEVec2Set(&normalized_vector, AECos(angle), AESin(angle));
//	AEVec2Scale(&normalized_vector, &normalized_vector, speed);
//
//	// distance between player -> bullet, player -> cursor
//	f32 dist_player2bullet = sqrt((bullet_x - player_center.x) * (bullet_x - player_center.x) + (bullet_y - player_center.y) * (bullet_y - player_center.y));
//	f32 dist_player2cursor = sqrt((player_center.x - center_cursor.x) * (player_center.x - center_cursor.x) + (player_center.y - center_cursor.y) * (player_center.y - center_cursor.y));
//
//	// if LEFT MOUSE clicked
//	if (state == 1) {
//		isRunning = TRUE;
//		// using normalized_vector
//		if ((bullet_x != player_center.x) && (bullet_y != player_center.y) && prevState == 0) {
//			bullet_x = player_center.x;
//			bullet_y = player_center.y;
//			prevState = 1;
//		}
//		if (center_cursor.y <= player_center.y) bullet_y += normalized_vector.y;
//		else if (center_cursor.y >= player_center.y) bullet_y -= normalized_vector.y;
//		if (center_cursor.x >= player_center.x) bullet_x += normalized_vector.x;
//		else if (center_cursor.x <= player_center.x) bullet_x -= normalized_vector.x;
//		
//		// draw bullet_mesh if between cursor & player
//		// checking coordinates rather than distance for collision?
//		if (((bullet_x <= center_cursor.x && bullet_y <= center_cursor.y && direction_y == UP && direction_x == RIGHT) ||
//			(bullet_x <= center_cursor.x && bullet_y >= center_cursor.y && direction_y == DOWN && direction_x == RIGHT) ||
//			(bullet_x >= center_cursor.x && bullet_y <= center_cursor.y && direction_y == UP && direction_x == LEFT) ||
//			(bullet_x >= center_cursor.x && bullet_y >= center_cursor.y && direction_y == DOWN && direction_x == LEFT)) &&
//			(bullet_x <= WINDOWLENGTH_X / 2) && (bullet_x >= -(WINDOWLENGTH_X / 2)) &&	//bullet within play screen
//			(bullet_y <= WINDOWLENGTH_Y / 2) && (bullet_y >= -(WINDOWLENGTH_Y / 2)))
//		{
//			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//			AEMtx33 weapon_scale = { 0 };
//			AEMtx33Scale(&weapon_scale, 20.f, 20.f); // scaling it up
//			AEMtx33 weapon_translate = { 0 };
//			AEMtx33Trans(&weapon_translate, bullet_x, bullet_y); // shifts along x & y axis
//			AEMtx33 weapon_rotate = { 0 };
//			AEMtx33Rot(&weapon_rotate, angle); // rotation
//			AEMtx33 weapon_transform = { 0 };
//			AEMtx33Concat(&weapon_transform, &weapon_rotate, &weapon_scale);
//			AEMtx33Concat(&weapon_transform, &weapon_translate, &weapon_transform);
//			AEGfxSetTransform(weapon_transform.m);
//			// Set the texture
//			AEGfxTextureSet(bulletA, 0, 0);
//			AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);
//
//		}
//		else // loops bullet
//		{ 
//			bullet_x = player_center.x;
//			bullet_y = player_center.y;
//		}
//
//		if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy1.x, enemy1.y) == TRUE) {
//			bullet_x = player_center.x;
//			bullet_y = player_center.y;
//			--enemy1.Hp;
//		}
//		if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy2.x, enemy2.y) == TRUE) {
//			bullet_x = player_center.x;
//			bullet_y = player_center.y;
//			--enemy2.Hp;
//		}
//	}
//
//	// if LEFT MOUSE not clicked
//	else if (state == 0) 
//	{
//		if ((dist_player2bullet < dist_player2cursor) && isRunning == TRUE) 
//		{
//			if (center_cursor.y <= player_center.y) bullet_y += normalized_vector.y;
//			else if (center_cursor.y >= player_center.y) bullet_y -= normalized_vector.y;
//			if (center_cursor.x >= player_center.x) bullet_x += normalized_vector.x;
//			else if (center_cursor.x <= player_center.x) bullet_x -= normalized_vector.x;
//			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//			AEMtx33 weapon_scale1 = { 0 };
//			AEMtx33Scale(&weapon_scale1, 20.f, 20.f); // scaling it up
//			AEMtx33 weapon_translate1 = { 0 };
//			AEMtx33Trans(&weapon_translate1, bullet_x, bullet_y); // shifts along x & y axis
//			AEMtx33 weapon_rotate1 = { 0 };
//			AEMtx33Rot(&weapon_rotate1, angle); // rotation
//			AEMtx33 weapon_transform1 = { 0 };
//			AEMtx33Concat(&weapon_transform1, &weapon_rotate1, &weapon_scale1);
//			AEMtx33Concat(&weapon_transform1, &weapon_translate1, &weapon_transform1);
//			AEGfxSetTransform(weapon_transform1.m);
//			// Set the texture to pTex
//			AEGfxTextureSet(bulletA, 0, 0);
//			AEGfxMeshDraw(shootMesh, AE_GFX_MDM_TRIANGLES);
//
//			if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy1.x, enemy1.y) == TRUE) {
//				bullet_x = player_center.x;
//				bullet_y = player_center.y;
//				isRunning = FALSE;
//				--enemy1.Hp;
//			}
//			if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy2.x, enemy2.y) == TRUE) {
//				bullet_x = player_center.x;
//				bullet_y = player_center.y;
//				isRunning = FALSE;
//				--enemy2.Hp;
//			}
//		}
//		else // if bullet reached cursor
//		{ 
//			isRunning = FALSE;
//			bullet_x = player_center.x;
//			bullet_y = player_center.y;
//			prevState = 0;
//		}
//
//	}
//}
/*!**************************************************************************************************
\brief
	Gets player and cursor position and draws a bullet from player to cursor for every frame.
	When left clicked is pressed, bullet will shoot continuously.
	When left clicked is not pressed, last bullet shot from player will continue to travel towards cursor and disappear once it reaches cursor.

\param[in] bullet_x
	x coordinates of bullet

\param[in] bullet_y
	y coordinates of bullet

\return
	TRUE if bullet is colliding with enemy, else FALSE
*******************************************************************************************************/
bool isbullet_enemy_colliding(f32 bullet_x, f32 bullet_y, f32 enemy_x, f32 enemy_y) {
	f32 dist_bullet2enemy = sqrt((bullet_x - enemy_x) * (bullet_x - enemy_x) + (bullet_y - enemy_y) * (bullet_y - enemy_y));
	if (dist_bullet2enemy <= 25) return TRUE;
	else return FALSE;
}
