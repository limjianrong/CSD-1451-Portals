
#include "AEEngine.h"
#include "GameState_Platformer.hpp"
#include "Utilities.hpp"
#include "boss.hpp"
#include "Player.hpp"
//#include "weapon_fire.hpp"
#include <iostream>
#include <fstream>
extern Player_stats player;
Boss boss;
Bullet bullet;
Laser_beam laser_beam;

static bool isRunning = FALSE;
// ----- Normalization -----
f32 adj, opp, angle;
f32 dist_boss2bullet, dist_boss2player;  // no longer using
AEVec2 normalized_vector; // direction vector from player to cursor

// --- Mesh ---
extern AEGfxVertexList* square_mesh;	// Created square mesh

std::ifstream boss_ifs{};

void boss_load() {
	boss.standTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_fly.png");
	boss.deadTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/spikeBall_2.png");
	laser_beam.picture = AEGfxTextureLoad("Assets/laser_beam_picture.png");

	// Bullet texture
	bullet.bulletTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/gold_1.png");
	boss_ifs.open("Assets/textFiles/boss_stats.txt");
	if (!boss_ifs) {
		std::cout << "\nFailed to open boss_stats.txt";
	}
	std::string str{};
	boss_ifs >> str >> boss.width;
	boss_ifs >> str >> boss.height;
	boss_ifs >> str >> boss.x_pos;
	boss_ifs >> str >> boss.y_pos;
	boss_ifs >> str >> boss.velocity;
	boss_ifs >> str >> boss.range_x;
	boss_ifs >> str >> boss.range_y;
	boss_ifs >> str >> boss.Hp;
	boss_ifs >> str >> boss.charge_cooldown;
	boss_ifs >> str >> boss.charge_range;
	boss_ifs >> str >> boss.charge_velocity;
	boss_ifs >> str >> laser_beam.width;
	boss_ifs >> str >> laser_beam.height;
	boss_ifs >> str >> laser_beam.cooldown;
	boss_ifs >> str >> laser_beam.duration;

	boss_ifs.close();

}


void boss_init () {

	// ---- Attack #1  : Laser beam ----
	laser_beam.cooldown_reset = laser_beam.cooldown;
	laser_beam.duration_reset = laser_beam.duration;


	// ---- Attack #2  :  Bullet ----
	bullet.x = boss.x_pos;			// Bullet x position
	bullet.y = boss.y_pos;			// Bullet y position
	bullet.width = 20.0f;				// Bullet width
	bullet.height = 20.0f;				// Bullet height
	bullet.speed = 5.0f;					// Bullet speed
	bullet.timer = BOSS_TIMER;			// Bullet timer between each bullet
	bullet.isTimerActive = FALSE;				// Indicator for timer activeness
	bullet.isTeleported = FALSE;				// Indicator for teleporation
	bullet.isShooting = FALSE;				// Indicator to check whether bullet is still shooting

	
}

void boss_draw() {

	// -------------  Boss   ---------------
	if (boss.Hp > 0) {
		AEMtx33Scale(&boss.scale, boss.width, boss.height);
		AEMtx33Trans(&boss.translate, boss.x_pos, boss.y_pos);
		AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);


		// -------------  Draw Attack 1 (Laser beam)   ---------------
		if (laser_beam.status == TRUE) {
			AEGfxTextureSet(laser_beam.picture, 0.0f, 0.0f);
			AEGfxSetTransform(laser_beam.matrix.m);
			AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		}

		// -------------  Attack 2 (Bullet)   ---------------
		bullet_draw();

		// -------------  Attack 3 (Dash)   ---------------

	}
	else {  // --- Boss dead ---
		AEMtx33Scale(&boss.scale, boss.width, boss.height);
		AEMtx33Trans(&boss.translate, boss.x_pos, boss.y_pos);
		AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);
		AEGfxSetTransform(boss.matrix.m);
		AEGfxTextureSet(boss.deadTex, 0.0f, 0.0f);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
	AEGfxSetTransform(boss.matrix.m);
	AEGfxTextureSet(boss.standTex, 0.0f, 0.0f);
	AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
}

void boss_update() {

	if (boss.Hp > 0) {

		//boss movement(Up and Down for now)
		boss_movement();

		//boss attack #1
		boss_laser_beam();

		//boss attack #2
		bullet_update();

		//boss attack #3
		boss_charge();
	}
}

void boss_free() {
	
}

void boss_unload() {

	// Texture unload
	AEGfxTextureUnload(bullet.bulletTex);

	// Texture unload
	AEGfxTextureUnload(boss.standTex);
	AEGfxTextureUnload(boss.deadTex);
	AEGfxTextureUnload(laser_beam.picture);
}

void boss_movement() {
	//boss movement UP and DOWN
	if (boss.direction == UP) {

		boss.y_pos += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * boss.velocity;
		if (player.y >= 0) {
			if (boss.y_pos + (boss.height / 2) - (player.y) > static_cast<f32>(AEGetWindowHeight() / 2)) {
				boss.direction = DOWN;
			}
		}

		else if (player.y < 0) {
			if (boss.y_pos + (boss.height / 2) > static_cast<f32>(AEGetWindowHeight() / 2)) {
				boss.direction = DOWN;
			}
		}
	}

	if (boss.direction == DOWN) {
		boss.y_pos -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * boss.velocity;

		if (player.y >= 0) {
			if (boss.y_pos - (boss.height / 2) - (player.y) < static_cast<f32>(-AEGetWindowHeight() / 2)) {

				boss.direction = UP;
			}
		}

		else if (player.y < 0) {
			if (boss.y_pos - (boss.height / 2) < static_cast<f32>(-AEGetWindowHeight() / 2)) {

				boss.direction = UP;
			}
		}
	}
	AEVec2Set(&boss.center, boss.x_pos, boss.y_pos);
}

// ------------------------ ATTACK #2 ------------------------------ //
void bullet_update() {

	// ----------  Boss  ----------
	if (player.y <= boss.y_pos) {			// Player below boss
		opp = player.y - boss.y_pos;
		//direction_y = DOWN;
	}
	else if (player.y >= boss.y_pos) {		// Player above boss
		opp = boss.y_pos - player.y;
		//direction_y = UP;
	}

	if (player.x >= boss.x_pos) {			// Player right of boss
		adj = player.x - boss.x_pos;
		//direction_x = RIGHT;
	}
	else if (player.x <= boss.x_pos) {		// Player left of boss
		adj = boss.x_pos - player.x;
		//direction_x = LEFT;
	}

	// ---- Normalization ----
	angle = AEATan(opp / adj); // get angle between player & boss (in rad)
	AEVec2Set(&normalized_vector, AECos(angle), AESin(angle));
	AEVec2Scale(&normalized_vector, &normalized_vector, bullet.speed);

	// distance between boss -> bullet, boss -> player
	dist_boss2bullet = sqrt((bullet.x - boss.x_pos) * (bullet.x - boss.x_pos) + (bullet.y - boss.y_pos) * (bullet.y - boss.y_pos));
	dist_boss2player = sqrt((boss.x_pos - player.x) * (boss.x_pos - player.x) + (boss.y_pos - player.y) * (boss.y_pos - player.y));


	// If player is within boss range (300x500 FOR NOW)
	if (player.x >= (boss.x_pos - boss.range_x) && player.x <= (boss.x_pos + boss.range_x) &&
		player.y >= (boss.y_pos - boss.range_y) && player.y <= (boss.y_pos + boss.range_y)) {

		// --- Enable shooting ---
		isRunning = TRUE;

		// If timer is over
		if (bullet.isTimerActive == FALSE) {
			// ---- Loops bullet ----
			//if (dist_boss2bullet < dist_boss2player && isRunning == TRUE) {
			if (dist_boss2bullet <= 400 && isRunning == TRUE) {	// Bullet disappears after 400 units

				// ----- Movement of bullet from boss to player -----
				if (player.y <= boss.y_pos) bullet.y += normalized_vector.y;
				else if (player.y >= boss.y_pos) bullet.y -= normalized_vector.y;
				if (player.x >= boss.x_pos) bullet.x += normalized_vector.x;
				else if (player.x <= boss.x_pos) bullet.x -= normalized_vector.x;
			}
			else {

				// --- Resets bullet ---
				bullet.x = boss.x_pos;
				bullet.y = boss.y_pos;
				bullet.isTeleported = FALSE;

				// If player x within 100 units of boss
				if (player.x >= (boss.x_pos - 100) && player.x <= boss.x_pos) {
					bullet.isTimerActive = TRUE;		// Enable bullet delay
				}
			}
		}
	}
	else { // No longer in range of boss

		// ---- Loops bullet ----
		//if (dist_boss2bullet < dist_boss2player && isRunning == TRUE) {
		if (dist_boss2bullet <= 400 && isRunning == TRUE) {	// Bullet disappears after 400 units

			// ----- Movement of bullet from boss to player -----
			if (player.y <= boss.y_pos) bullet.y += normalized_vector.y;
			else if (player.y >= boss.y_pos) bullet.y -= normalized_vector.y;
			if (player.x >= boss.x_pos) bullet.x += normalized_vector.x;
			else if (player.x <= boss.x_pos) bullet.x -= normalized_vector.x;
		}
		else {

			// --- Disable shooting ---
			isRunning = FALSE;
			// --- Resets bullet ---
			bullet.x = boss.x_pos;
			bullet.y = boss.y_pos;
			bullet.isTeleported = FALSE;
		}
	}

	// ----- Bullet collision with player -----
	AEVec2Set(&bullet.center, bullet.x, bullet.y);
	if (AETestRectToRect(&bullet.center, bullet.width, bullet.height, &player.center, player.width, player.height)) {
		bullet.x = boss.x_pos;				// Reset bullet x
		bullet.y = boss.y_pos;				// Reset bullet y
		bullet.isTimerActive = TRUE;		// Enable bullet delay
		--player.Hp;
	}

	// ----- Bullet collision with boss -----
	if (AETestRectToRect(&bullet.center, bullet.width, bullet.height, &boss.center, boss.width, boss.height) && bullet.isTeleported) {
		bullet.x = boss.x_pos;
		bullet.y = boss.y_pos;
		bullet.isTeleported = FALSE;
		bullet.isTimerActive = TRUE;		// Enable bullet delay
		--boss.Hp;
	}

	// ----- Resets bullet timer (Delay inbetween bullets) -----
	if (bullet.isTimerActive == TRUE) {
		bullet.timer -= AEFrameRateControllerGetFrameTime();
	}
	if (bullet.timer <= 0) {
		bullet.timer = BOSS_TIMER;
		bullet.isTimerActive = FALSE;
	}

	//// If bullet is within range of boss
	//if (bullet.x >= (boss.x_pos - boss.width) && bullet.x <= (boss.x_pos + boss.width) &&
	//	bullet.y >= (boss.y_pos) - boss.height && bullet.y <= (boss.y_pos) + boss.height) {


	// ----- Bullet collision with enemy1 -----
	/*if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy1_a.x, enemy1_a.y) == TRUE) {
		bullet_x = boss.x_pos;
		bullet_y = boss.y_pos;
		--enemy1_a.Hp;
	}
	if (isbullet_enemy_colliding(bullet_x, bullet_y, enemy1_b.x, enemy1_b.y) == TRUE) {
		bullet_x = boss.x_pos;
		bullet_y = boss.y_pos;
		--enemy1_b.Hp;
	}*/
}

void bullet_draw() {

	// ---- Loops bullet ----
	//if (dist_boss2bullet < dist_boss2player && isRunning == TRUE) {
	if (dist_boss2bullet <= 400 && isRunning == TRUE) {	// Bullet disappears after 400 units

		AEMtx33 weapon_scale = { 0 };
		AEMtx33Scale(&weapon_scale, bullet.width, bullet.height); // scaling it up
		AEMtx33 weapon_translate = { 0 };
		AEMtx33Trans(&weapon_translate, bullet.x, bullet.y); // shifts along x & y axis
		AEMtx33 weapon_rotate = { 0 };
		AEMtx33Rot(&weapon_rotate, angle); // rotation
		AEMtx33 weapon_transform = { 0 };
		AEMtx33Concat(&weapon_transform, &weapon_rotate, &weapon_scale);
		AEMtx33Concat(&weapon_transform, &weapon_translate, &weapon_transform);
		AEGfxSetTransform(weapon_transform.m);
		AEGfxTextureSet(bullet.bulletTex, 0, 0);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}

}

void boss_laser_beam() {
	--laser_beam.cooldown;
	if (laser_beam.cooldown ==0 ) {
		// set laser_beam to be firing left if player is on left of boss, firing right if player is on right of boss
		laser_beam.status = TRUE;
		laser_beam.duration = laser_beam.duration_reset;
	}
	if (laser_beam.status == TRUE) {
		--laser_beam.duration;

		//fix the x-direction that the laser_beam is firing at, but the laser_beam's y value should still be
		//following the boss's y position

		if (player.x < boss.x_pos) {
			AEVec2Set(&laser_beam.center, boss.x_pos - laser_beam.width / 2, boss.y_pos);
		}
		else {
			AEVec2Set(&laser_beam.center, boss.x_pos + laser_beam.width / 2, boss.y_pos);
		}
		//calculate the transformations of the laser beam
		laser_beam.matrix = {};
		AEMtx33Scale(&laser_beam.scale, laser_beam.width, laser_beam.height);
		AEMtx33Trans(&laser_beam.translate, laser_beam.center.x, laser_beam.center.y);
		AEMtx33Concat(&laser_beam.matrix, &laser_beam.translate, &laser_beam.scale);

		if (laser_beam.duration==0) {
			laser_beam.status = FALSE;
			laser_beam.cooldown = 300.0f;
			laser_beam.damaged_player = false;
			return;
		}

		if (AETestRectToRect(&laser_beam.center, laser_beam.width, laser_beam.height, &player.center, player.width, player.height)) {
			if (laser_beam.damaged_player == FALSE) {
				--player.Hp;
				laser_beam.damaged_player = TRUE;
			}
		}
	}


}

void boss_charge() {

	//decrement timer for boss's charge cooldown

	--boss.charge_cooldown;

	//boss will charge towards the player when cooldown = 0 and player is within a certain range
	if (boss.charge_cooldown == 0.0f && AECalcDistPointToRect(&boss.center, &player.center, player.width, player.height) < boss.charge_range) {
		boss.previous_direction = boss.direction;
		boss.direction = STOP;
		boss.return_to_position = false;
		boss.charge_token = 1;

		//set vector for direction for boss to charge in
		AEVec2Set(&boss.original_position, boss.x_pos, boss.y_pos);
		AEVec2Sub(&boss.charge_direction, &boss.center, &player.center);

		//set vector for the endpoint of the boss's charge
		AEVec2Set(&boss.charge_endpoint, player.x, player.y);
		//get angle between boss and player (in radians)
		boss.charge_angle = AEATan(abs(boss.charge_direction.y) / abs(boss.charge_direction.x));

		//determine where the boss will charge towards
		if (player.x < boss.x_pos && player.y > boss.y_pos) {
			boss.charge_towards = TOP_LEFT;

		}
		else if (player.x < boss.x_pos && player.y < boss.y_pos) {
			boss.charge_towards = BTM_LEFT;

		}
		else if (player.x > boss.x_pos && player.y > boss.y_pos) {
			boss.charge_towards = TOP_RIGHT;
		}

		else if (player.x > boss.x_pos && player.y < boss.y_pos) {
			boss.charge_towards = BTM_RIGHT;
		}

	}


	if (boss.charge_token == true) {
		//if player is top left of boss
		if (boss.charge_towards == TOP_LEFT && boss.return_to_position == false) {
			boss.x_pos -= boss.charge_velocity * cos(boss.charge_angle);
			boss.y_pos += boss.charge_velocity * sin(boss.charge_angle);
			if (boss.x_pos < boss.charge_endpoint.x && boss.y_pos > boss.charge_endpoint.y) {
				boss.return_to_position = true;
			}
		}
		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == TOP_LEFT && boss.return_to_position == true) {
			boss.x_pos += boss.charge_velocity * cos(boss.charge_angle);
			boss.y_pos -= boss.charge_velocity * sin(boss.charge_angle);
			if (boss.x_pos > boss.original_position.x && boss.y_pos < boss.original_position.y) {
				boss.charge_token = false;
				boss.x_pos = boss.original_position.x;
				boss.y_pos = boss.original_position.y;
				if (boss.previous_direction == UP) {
					boss.direction = UP;
					boss.charge_cooldown = 1000.0f;
				}
				else {
					boss.direction = DOWN;
					boss.charge_cooldown = 1000.0f;
				}
			}
		}


		//if player is bottom left of boss
		else if (boss.charge_towards == BTM_LEFT && boss.return_to_position == false) {

			boss.x_pos -= boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos -= boss.charge_velocity * (sin(boss.charge_angle));

			if (boss.x_pos < boss.charge_endpoint.x && boss.y_pos < boss.charge_endpoint.y) {
				boss.return_to_position = true;
			}
		}

		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == BTM_LEFT && boss.return_to_position == true) {
			boss.x_pos += boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos += boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos > boss.original_position.x && boss.y_pos > boss.original_position.y) {

				//std::cout << "\nback to original position";
				boss.charge_token = false;
				boss.x_pos = boss.original_position.x;
				boss.y_pos = boss.original_position.y;
				if (boss.previous_direction == UP) {
					boss.direction = UP;
					boss.charge_cooldown = 1000.0f;
				}
				else {
					boss.direction = DOWN;
					boss.charge_cooldown = 1000.0f;
				}
			}

		}

		//if player is top right of boss
		else if (boss.charge_towards == TOP_RIGHT && boss.return_to_position == false) {
			boss.x_pos += boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos += boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos > boss.charge_endpoint.x && boss.y_pos > boss.charge_endpoint.y) {
				boss.return_to_position = true;
			}
		}

		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == TOP_RIGHT && boss.return_to_position == true) {
			boss.x_pos -= boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos -= boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos < boss.original_position.x && boss.y_pos < boss.original_position.y) {
				boss.charge_token = true;
				boss.x_pos = boss.original_position.x;
				boss.y_pos = boss.original_position.y;
				if (boss.previous_direction == UP) {
					boss.direction = UP;
					boss.charge_cooldown = 1000.0f;
				}
				else {
					boss.direction = DOWN;
					boss.charge_cooldown = 1000.0f;
				}
			}
		}


		//if player is bottom right of boss
		else if (boss.charge_towards == BTM_RIGHT && boss.return_to_position == false) {
			boss.x_pos += boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos -= boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos > boss.charge_endpoint.x && boss.y_pos < boss.charge_endpoint.y) {
				boss.return_to_position = true;
			}
		}

		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == BTM_RIGHT && boss.return_to_position == true) {

			boss.x_pos -= boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos += boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos < boss.original_position.x && boss.y_pos >boss.original_position.y) {

				boss.charge_token = false;
				boss.x_pos = boss.original_position.x;
				boss.y_pos = boss.original_position.y;
				if (boss.previous_direction == UP) {
					boss.direction = UP;
					boss.charge_cooldown = 1000.0f;
				}
				else {
					boss.direction = DOWN;
					boss.charge_cooldown = 1000.0f;
				}
			}
		}
	}
}