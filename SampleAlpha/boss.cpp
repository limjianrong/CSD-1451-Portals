
#include "AEEngine.h"
#include "GameState_Platformer.hpp"
#include "Utilities.hpp"
#include "boss.hpp"
#include "Player.hpp"
#include "weapon_fire.hpp"
#include <iostream>

extern Player_stats player;
Boss boss;
Laser_beam laser_beam;

void boss_load() {
	laser_beam.mesh = boss.mesh = create_Square_Mesh();
	boss.standTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_fly.png");
	boss.deadTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/spikeBall_2.png");
	laser_beam.picture = AEGfxTextureLoad("Assets/laser_beam_picture.png");
	if (!laser_beam.picture) {
		std::cout << "failed to load laser beam picture";
	}
}

void boss_init () {

	bullet_init();
}

void draw_boss() {

	// -------------  Boss   ---------------
	if (boss.Hp > 0) {
		AEMtx33Scale(&boss.scale, boss.width, boss.height);
		AEMtx33Trans(&boss.translate, boss.x_pos, boss.y_pos);
		AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);


		// -------------  Draw Attack 1 (Laser beam)   ---------------
		if (laser_beam.status == TRUE) {
			AEGfxTextureSet(laser_beam.picture, 0.0f, 0.0f);
			AEGfxSetTransform(laser_beam.matrix.m);
			AEGfxMeshDraw(laser_beam.mesh, AE_GFX_MDM_TRIANGLES);
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
		AEGfxMeshDraw(boss.mesh, AE_GFX_MDM_TRIANGLES);
	}
	AEGfxSetTransform(boss.matrix.m);
	AEGfxTextureSet(boss.standTex, 0.0f, 0.0f);
	AEGfxMeshDraw(boss.mesh, AE_GFX_MDM_TRIANGLES);
}

void update_boss() {

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

void boss_laser_beam() {
	--laser_beam.cooldown;
	if (laser_beam.cooldown ==0 ) {
		// set laser_beam to be firing left if player is on left of boss, firing right if player is on right of boss
		laser_beam.status = TRUE;
		laser_beam.duration = 200.0f;
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
			laser_beam.damaged_player = FALSE;
			return;
		}

		if (AETestRectToRect(&laser_beam.center, laser_beam.width, laser_beam.height, &player.center, PLAYER_WIDTH, PLAYER_HEIGHT)) {
			if (laser_beam.damaged_player == FALSE) {
				--player.Lives;
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
		boss.return_to_position = 0;
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


	if (boss.charge_token == 1) {
		//if player is top left of boss
		if (boss.charge_towards == TOP_LEFT && boss.return_to_position == 0) {
			boss.x_pos -= boss.charge_velocity * cos(boss.charge_angle);
			boss.y_pos += boss.charge_velocity * sin(boss.charge_angle);
			if (boss.x_pos < boss.charge_endpoint.x && boss.y_pos > boss.charge_endpoint.y) {
				boss.return_to_position = 1;
			}
		}
		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == TOP_LEFT && boss.return_to_position == 1) {
			boss.x_pos += boss.charge_velocity * cos(boss.charge_angle);
			boss.y_pos -= boss.charge_velocity * sin(boss.charge_angle);
			if (boss.x_pos > boss.original_position.x && boss.y_pos < boss.original_position.y) {
				boss.charge_token = 0;
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
		else if (boss.charge_towards == BTM_LEFT && boss.return_to_position == 0) {

			boss.x_pos -= boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos -= boss.charge_velocity * (sin(boss.charge_angle));

			if (boss.x_pos < boss.charge_endpoint.x && boss.y_pos < boss.charge_endpoint.y) {
				boss.return_to_position = 1;
			}
		}

		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == BTM_LEFT && boss.return_to_position == 1) {
			boss.x_pos += boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos += boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos > boss.original_position.x && boss.y_pos > boss.original_position.y) {

				//std::cout << "\nback to original position";
				boss.charge_token = 0;
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
		else if (boss.charge_towards == TOP_RIGHT && boss.return_to_position == 0) {
			boss.x_pos += boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos += boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos > boss.charge_endpoint.x && boss.y_pos > boss.charge_endpoint.y) {
				boss.return_to_position = 1;
			}
		}

		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == TOP_RIGHT && boss.return_to_position == 1) {
			boss.x_pos -= boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos -= boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos < boss.original_position.x && boss.y_pos < boss.original_position.y) {
				boss.charge_token = 0;
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
		else if (boss.charge_towards == BTM_RIGHT && boss.return_to_position == 0) {
			boss.x_pos += boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos -= boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos > boss.charge_endpoint.x && boss.y_pos < boss.charge_endpoint.y) {
				boss.return_to_position = 1;
			}
		}

		//boss has finished charging forward, now return to original position
		else if (boss.charge_towards == BTM_RIGHT && boss.return_to_position == 1) {

			boss.x_pos -= boss.charge_velocity * (cos(boss.charge_angle));
			boss.y_pos += boss.charge_velocity * (sin(boss.charge_angle));
			if (boss.x_pos < boss.original_position.x && boss.y_pos >boss.original_position.y) {

				boss.charge_token = 0;
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