/*==================================================================================
* All content � 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					boss.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors:	Lin ZhaoZhi (z.lin@digipen.edu)
*
* Brief:
  This source file implements the functions used for the boss attacks 1,2 and 3. As well
  as functions used for boss movement and teleportation.
==================================================================================*/

#include "boss.hpp"
#include "Player.hpp"
#include "Utilities.hpp"

// ---- Game Objects / Classes ----
extern Player_stats player;
Boss boss; //boss object
Bullet bullet;
Laser_beam laser_beam; //boss laser beam attack
Boss_charge boss_charge; //boss charge attack
Boss_teleport boss_teleport; //boss teleportation
static bool isRunning = false;
// ---- Normalization ----
f32 adj, opp, angle;
f32 dist_boss2bullet, dist_boss2player;  // no longer using
AEVec2 normalized_vector; // direction vector from player to cursor

// ---- Mesh ----
extern AEGfxVertexList* square_mesh;	// Created square mesh

// ---- Audio ----
extern AEAudio laserAudio, damageAudio, deathAudio, bulletAudio;
extern AEAudioGroup soundGroup;

std::ifstream boss_ifs{}; //file stream to load boss stats from

//load the assets used by the boss
void boss_load() {
	boss.standTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/flyMan_fly.png");
	if (!boss.standTex) {
		std::cout << "\nFailed to load flyMan_fly.png";
	}
	boss.deadTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Enemies/spikeBall_2.png");
	if (!boss.deadTex) {
		std::cout << "\nFailed to load spikeBall_2.png";
	}
	laser_beam.picture = AEGfxTextureLoad("Assets/laser_beam_picture.png");
	if (!laser_beam.picture) {
		std::cout << "\nFailed to load laser_beam_picture.png";
	}

	laser_beam.warning_pic = AEGfxTextureLoad("Assets/laser_warning.png");
	if (!laser_beam.warning_pic) {
		std::cout << "\nFailed to load laser_warning.png";
	}

	// Bullet texture
	bullet.bulletTex = AEGfxTextureLoad("Assets/jumperpack/PNG/Items/gold_1.png");
	boss_ifs.open("Assets/textFiles/boss_stats.txt");
	if (!boss_ifs) {
		std::cout << "\nFailed to open boss_stats.txt";
	}

	//load boss stats from std::ifstream
	std::string str{};
	boss_ifs >> str >> boss.status;
	boss_ifs >> str >> boss.dimensions.x;
	boss_ifs >> str >> boss.dimensions.y;
	boss_ifs >> str >> boss.center.x;
	boss_ifs >> str >> boss.center.y;
	boss_ifs >> str >> boss.velocity;
	boss_ifs >> str >> boss.range_x;
	boss_ifs >> str >> boss.range_y;
	boss_ifs >> str >> boss.Hp;
	boss_ifs >> str >> boss_charge.cooldown;
	boss_ifs >> str >> boss_charge.range;
	boss_ifs >> str >> boss_charge.velocity;
	boss_ifs >> str >> laser_beam.width;
	boss_ifs >> str >> laser_beam.height;
	boss_ifs >> str >> laser_beam.cooldown;
	boss_ifs >> str >> laser_beam.max_duration;
	boss_ifs >> str >> laser_beam.buffer_duration;		//let player know 2 seconds in advance of boss laser beam attack
	boss_ifs >> str >> laser_beam.warning_pic_width;	//width of the warning sign
	boss_ifs >> str >> laser_beam.warning_pic_height;	//height of the warning sign
	boss_ifs >> str >> boss_teleport.cooldown;			//cooldown of boss's teleportation
	boss_ifs.close();
}

//initializes some variables used for boss bullet, laser beam, and boss teleportation
void boss_init () {

	//intitialize max hp of boss
	boss.Max_Hp = boss.Hp;
	// ---- Attack #2  :  Bullet ----
	bullet.x = boss.center.x;				// Bullet x position
	bullet.y = boss.center.y;				// Bullet y position
	bullet.width = 20.0f;					// Bullet width
	bullet.height = 20.0f;					// Bullet height
	bullet.speed = 5.0f;					// Bullet speed
	bullet.timer = BOSS_TIMER;				// Bullet timer between each bullet
	bullet.isTimerActive = FALSE;			// Indicator for timer activeness
	bullet.isTeleported = FALSE;			// Indicator for teleporation
	bullet.isShooting = FALSE;				// Indicator to check whether bullet is still shooting

}

//updates the boss while the boss is alive, updates boss movement and boss attacks
void boss_update() {

	// When player is within boss zone
	if (AETestRectToRect(&player.center, player.dimensions.x, player.dimensions.y, &boss.center, 2000.f, 2000.f)) {
		if (boss.Hp > 0) {

			//boss movement
			boss_movement();

			//boss teleportation
			boss_movement_teleport();

			//boss attack #1
			boss_laser_beam_attack();

			//boss attack #2
			bullet_update();

			//boss attack #3
			boss_charge_attack();
		}
		else {
			// --- Boss' death audio ---
			AEAudioPlay(deathAudio, soundGroup, 0.5f, 1.f, 0);
		}
	}
}


//draws the boss
void boss_draw() {

	// When player is within boss zone
	//if (AETestRectToRect(&player.center, player.dimensions.x, player.dimensions.y, &boss.center, 2000.f, 2000.f)) {
		// -------------  Boss   ---------------
		if (boss.Hp > 0) {
			AEMtx33Scale(&boss.scale, boss.dimensions.x, boss.dimensions.y);
			AEMtx33Trans(&boss.translate, boss.center.x, boss.center.y);
			AEMtx33Concat(&boss.matrix, &boss.translate, &boss.scale);


			// -------------  Draw Attack 1 (Laser beam)   ---------------
			draw_laser_beam();

			// -------------  Attack 2 (Bullet)   ---------------
			bullet_draw();

			// -------- Drawing out HP bar ----------
			boss.GameObjects::Render_HealthBar();

		}
		else {  // --- Boss dead ---
			AEGfxTextureSet(boss.deadTex, 0.0f, 0.0f);
			drawMesh(AEVec2{ boss.dimensions.x, boss.dimensions.y }, boss.center, PI);
		}
		AEGfxSetTransform(boss.matrix.m);
		AEGfxTextureSet(boss.standTex, 0.0f, 0.0f);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
		draw_laser_beam_warning();
	//}
}

//frees any objects related to the boss
void boss_free() { //empty by design 
}

//unloads the assets used by the boss 
void boss_unload() {

	// Texture unload
	AEGfxTextureUnload(bullet.bulletTex);

	// Texture unload
	AEGfxTextureUnload(boss.standTex);
	AEGfxTextureUnload(boss.deadTex);
	AEGfxTextureUnload(laser_beam.picture);
	AEGfxTextureUnload(laser_beam.warning_pic);
}

//function for boss to move up and down
void boss_movement() {

	//boss movement up, boss movement will be relative to the player's y position
	if (boss.direction == UP) {
		//if top most edge of the boss is same height as the top of the screen, change direction to move down
		boss.center.y += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * boss.velocity;
		if (boss.center.y + (boss.dimensions.y) / 2 >= AEGfxGetWinMaxY()) {
			boss.direction = DOWN;
		}
	}

	//boss movement down, also relative to player's y position
	if (boss.direction == DOWN) {
		//if bottom most edge of boss is same height as the bottom of the screen, change direction to move up
		boss.center.y -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * boss.velocity;
		if (boss.center.y - (boss.dimensions.y) / 2 <= AEGfxGetWinMinY()) {
			boss.direction = UP;
		}
	}
}

//teleportation of the boss, only occurs when boss hp less than or equal to half of boss's max hp
void boss_movement_teleport() {
	if (boss.Hp <= boss.Max_Hp /2) {
		//increment time elapsed
		boss_teleport.time_elapsed += static_cast<f32>(AEFrameRateControllerGetFrameTime());

		//if time elapsed >= teleport cooldown and boss is not charging towards player
		if (boss_teleport.time_elapsed >= boss_teleport.cooldown && boss_charge.active == false) {
			//randomize the location for the boss's teleport, but it must be within the screen
			boss_teleport.location.x = rand() % AEGetWindowWidth() + AEGfxGetWinMinX();
			boss_teleport.location.y = rand() % AEGetWindowHeight() + AEGfxGetWinMinY();
			
			//if boss's teleport location will collide with player, generate a new location
			while (AETestRectToRect(&boss_teleport.location, boss.dimensions.x, boss.dimensions.y, &player.center, player.dimensions.x, player.dimensions.y)) {
				boss_teleport.location.x = rand() % AEGetWindowWidth() + AEGfxGetWinMinX();
				boss_teleport.location.y = rand() % AEGetWindowHeight() + AEGfxGetWinMinY();
			}

			//teleport the boss to its new randomized location
			boss.center.x = boss_teleport.location.x;
			boss.center.y = boss_teleport.location.y;

			//reset the time elapsed since the boss's last teleport
			boss_teleport.time_elapsed = 0;
		}
	}
}

//  ------------------------------------- ATTACK 1 -----------------------------------------
// function for the boss's laser beam attack, fires a laser beam in the direction of the player
// laser beam has a limited range and damages the player once per attack if the player is collides with
// the laser beam
void boss_laser_beam_attack() {

	laser_beam.time_elapsed += static_cast<f32>(AEFrameRateControllerGetFrameTime());

	//laser beam will not occur when boss attack #3 (boss charge attack) is active
	if (laser_beam.time_elapsed >= laser_beam.cooldown && boss_charge.active == false) {

		// Laser attack #3 audio
		AEAudioPlay(laserAudio, soundGroup, 0.2f, 1.f, 0);

		//set token to draw the laser beam
		laser_beam.active = TRUE;
		//increment the duration that the laser beam has been firing for
		laser_beam.duration += static_cast<f32>(AEFrameRateControllerGetFrameTime());

		//set the direction that the laser beam is firing at
		if (player.center.x < boss.center.x) {
			AEVec2Set(&laser_beam.center, boss.center.x - laser_beam.width / 2, boss.center.y);
		}
		else {
			AEVec2Set(&laser_beam.center, boss.center.x + laser_beam.width / 2, boss.center.y);
		}
		//calculate the transformations of the laser beam
		AEMtx33Scale(&laser_beam.scale, laser_beam.width, laser_beam.height);
		AEMtx33Trans(&laser_beam.translate, laser_beam.center.x, laser_beam.center.y);
		AEMtx33Concat(&laser_beam.final_matrix, &laser_beam.translate, &laser_beam.scale);

		//once the laser beam has been active for long enough, stop firing it
		if (laser_beam.duration >= laser_beam.max_duration) {
			laser_beam.active = FALSE;
			laser_beam.damaged_player = false;

			//reset the duration that the laser beam has fired
			laser_beam.duration = 0;

			//reset the time elapsed since the previous laser beam attack
			laser_beam.time_elapsed = 0;
			return;
		}

		//check if player has collided with the laser beam while the laser beam is firing
		if (AETestRectToRect(&laser_beam.center, laser_beam.width, laser_beam.height, &player.center, player.dimensions.x, player.dimensions.y)) {
			//if player has been damaged by the laser beam within this attack, player will
			//not be damaged again
			if (laser_beam.damaged_player == false) {
				if (player.isShieldActive) {
					player.isShieldActive = FALSE;
				}
				else {
					//--player.Hp;
				}
				laser_beam.damaged_player = true;
			}
		}
	}
} //end of laser_beam_attack

// ------------------------ ATTACK #2 ------------------------------ //
//boss shoots bullets at the player
void bullet_update() {

	// ----------  Boss  ----------
	if (player.center.y <= boss.center.y) {			// Player below boss
		opp = player.center.y - boss.center.y;
	}
	else if (player.center.y >= boss.center.y) {		// Player above boss
		opp = boss.center.y - player.center.y;
	}

	if (player.center.x >= boss.center.x) {			// Player right of boss
		adj = player.center.x - boss.center.x;
	}
	else if (player.center.x <= boss.center.x) {		// Player left of boss
		adj = boss.center.x - player.center.x;
	}

	// ---- Normalization ----
	angle = AEATan(opp / adj); // get angle between player & boss (in rad)
	AEVec2Set(&normalized_vector, AECos(angle), AESin(angle));
	AEVec2Scale(&normalized_vector, &normalized_vector, bullet.speed);

	// distance between boss -> bullet, boss -> player
	dist_boss2bullet = sqrt((bullet.x - boss.center.x) * (bullet.x - boss.center.x) + (bullet.y - boss.center.y) * (bullet.y - boss.center.y));
	dist_boss2player = sqrt((boss.center.x - player.center.x) * (boss.center.x - player.center.x) + (boss.center.y - player.center.y) * (boss.center.y - player.center.y));


	// If player is within boss range
	if (player.center.x >= (boss.center.x - boss.range_x) && player.center.x <= (boss.center.x + boss.range_x) &&
		player.center.y >= (boss.center.y - boss.range_y) && player.center.y <= (boss.center.y + boss.range_y)) {

		// --- Enable shooting ---
		isRunning = TRUE;
		
		// If timer is over
		if (bullet.isTimerActive == FALSE) {
			// ---- Loops bullet ----
			//if (dist_boss2bullet < dist_boss2player && isRunning == TRUE) {
			if (dist_boss2bullet <= 400.f && isRunning == TRUE) {	// Bullet disappears after 400 units

				// ----- Movement of bullet from boss to player -----
				if (player.center.y <= boss.center.y) bullet.y += normalized_vector.y;
				else if (player.center.y >= boss.center.y) bullet.y -= normalized_vector.y;
				if (player.center.x >= boss.center.x) bullet.x += normalized_vector.x;
				else if (player.center.x <= boss.center.x) bullet.x -= normalized_vector.x;

				//std::cout << dist_boss2bullet << std::endl;
				//// --- Bullet sfx everytime bullet is reset ---
				//if (dist_boss2bullet <= 25.f) {
				//	AEAudioPlay(bulletAudio, soundGroup, 0.5f, 1.f, 0);
				//}
			}
			else {
				// --- Resets bullet ---
				bullet.x = boss.center.x;
				bullet.y = boss.center.y;
				bullet.isTeleported = FALSE;

				// If player x within 100 units of boss
				if (player.center.x >= (boss.center.x - 100) && player.center.x <= boss.center.x) {
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
			if (player.center.y <= boss.center.y) bullet.y += normalized_vector.y;
			else if (player.center.y >= boss.center.y) bullet.y -= normalized_vector.y;
			if (player.center.x >= boss.center.x) bullet.x += normalized_vector.x;
			else if (player.center.x <= boss.center.x) bullet.x -= normalized_vector.x;
		}
		else {
			// --- Disable shooting ---
			isRunning = FALSE;
			// --- Resets bullet ---
			bullet.x = boss.center.x;
			bullet.y = boss.center.y;
			bullet.isTeleported = FALSE;
		}
	}

	// ----- Bullet collision with player -----
	AEVec2Set(&bullet.center, bullet.x, bullet.y);
	if (AETestRectToRect(&bullet.center, bullet.width, bullet.height, &player.center, player.dimensions.x, player.dimensions.y)) {
		bullet.x = boss.center.x;				// Reset bullet x
		bullet.y = boss.center.y;				// Reset bullet y
		bullet.isTimerActive = TRUE;		// Enable bullet delay

		if (player.isShieldActive) {
			player.isShieldActive = FALSE;
		}
		else {
			//--player.Hp;
		}
	}

	// ----- Bullet collision with boss -----
	if (AETestRectToRect(&bullet.center, bullet.width, bullet.height, &boss.center, boss.dimensions.x, boss.dimensions.y) && bullet.isTeleported) {
		bullet.x = boss.center.x;
		bullet.y = boss.center.y;
		bullet.isTeleported = FALSE;
		bullet.isTimerActive = TRUE;		// Enable bullet delay
		--boss.Hp;

		// --- Boss' damage audio ---
		AEAudioPlay(damageAudio, soundGroup, 0.5f, 1.f, 0);
	}

	// ----- Resets bullet timer (Delay inbetween bullets) -----
	if (bullet.isTimerActive == TRUE) {
		bullet.timer -= static_cast<f32>(AEFrameRateControllerGetFrameTime());
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

//function for boss attack #3, the boss charges towards the position of the player, which is determined by the
//player's position the moment the flag for the boss charge attack is set to active
//after the boss has reached the endpoint of its charge, it will charge back to its original position
//when it first started the charge attack
void boss_charge_attack() {
	//increment time elapsed since the previous boss charge attack
	boss_charge.time_elapsed += static_cast<f32>(AEFrameRateControllerGetFrameTime());

	//boss will charge towards the player when time_elapsed >= charge attack cooldown and player is within 
	//the boss's charge attack range, the boss must also not be in the middle of other boss attacks
	//such as laser_beam_attack
	if ((boss_charge.time_elapsed >= boss_charge.cooldown) && boss_charge.active == false &&
		AECalcDistPointToRect(&boss.center, &player.center, player.dimensions.x, player.dimensions.y) < boss_charge.range
		&& laser_beam.active == false) {

		//set the flag for boss charge attack to be true
		boss_charge.active = true;

		//when return_to_position is set to true, boss will begin its charging back to
		//its original position
		boss_charge.return_to_position = false;

		//record down the direction of the boss movement before the charge
		boss_charge.previous_direction = boss.direction;

		//stop moving the boss up and down
		boss.direction = STOP;


		//set vector for the endpoint of boss's charge attack, which is the position of the player in that frame
		AEVec2Set(&boss_charge.endpoint, player.center.x, player.center.y);

		//set vector for original position of boss to return to
		AEVec2Set(&boss_charge.original_position, boss.center.x, boss.center.y);

		//set direction for boss to charge towards
		AEVec2Sub(&boss_charge.direction, &player.center, &boss.center);

		//normalize the direction and store it in another variable
		AEVec2Normalize(&boss_charge.normalized_direction, &boss_charge.direction);

		//determine the distance needed to be travelled by the boss, from its
		//original position to the player's position in that frame
		boss_charge.direction_magnitude = AEVec2Length(&boss_charge.direction);
	}

	//if boss is charging forward(1st phase of the attack)
	if (boss_charge.active == true && boss_charge.return_to_position == false) {
		boss.center.x += boss_charge.normalized_direction.x * boss_charge.velocity;
		boss.center.y += boss_charge.normalized_direction.y * boss_charge.velocity;
	}

	//determine the distance that the boss needs to charge, based on whether it is charging to the player, or charging
	//back to its original position
	
	//boss is charging towards player, calculate distance between current boss position and its
	//original position(where it is moving away from)
	if (boss_charge.return_to_position == false) {
		boss_charge.distance_travelled = AEVec2Distance(&boss.center, &boss_charge.original_position);
		
	}

	//else, boss is charging back to its original position, calculate distance between current boss
	//position and its endpoint(where it is moving away from)
	else {
		boss_charge.distance_travelled = AEVec2Distance(&boss.center, &boss_charge.endpoint);
	}

	//if distance travelled is more than distance needed to be travelled, and boss is charging forward
	//make the boss start charging back to its original position (2nd phase of the attack)
	if (boss_charge.distance_travelled >= boss_charge.direction_magnitude && boss_charge.return_to_position == false) {
		boss_charge.return_to_position = true;
		boss_charge.distance_travelled = 0;
	}

	//make the boss charge back to the original position when it first started the charge attack
	if (boss_charge.active == true && boss_charge.return_to_position == true) {
		boss.center.x -= boss_charge.normalized_direction.x * boss_charge.velocity;
		boss.center.y -= boss_charge.normalized_direction.y * boss_charge.velocity;

		//if boss has returned to original position, reset all flags and time elapsed
		if (boss_charge.distance_travelled >= boss_charge.direction_magnitude) {
			boss_charge.active = false;
			boss_charge.return_to_position = false;
			boss_charge.time_elapsed = 0;
			boss.direction = boss_charge.previous_direction;
			boss_charge.direction_magnitude = 0;
			boss_charge.distance_travelled = 0;
			boss_charge.player_damaged = false;
		}

		//check if boss collided with the player during the charge, limit charge attack to only damage the player once per attack
		if (AETestRectToRect(&player.center, player.dimensions.x, player.dimensions.y, &boss.center, boss.dimensions.x, boss.dimensions.y)) {
			if (!boss_charge.player_damaged) {
				boss_charge.player_damaged = true;
				if (player.isShieldActive) {
					player.isShieldActive = FALSE;
				}
				else {
					//--player.Hp;
				}
			}
		}
	}
}

//function to draw the boss's laser beam
void draw_laser_beam() {
	if (laser_beam.active) {
		AEGfxTextureSet(laser_beam.picture, 0.0f, 0.0f);
		AEGfxSetTransform(laser_beam.final_matrix.m);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

//function to draw the boss's laser beam warning, before the actual laser beam attack
void draw_laser_beam_warning() {

	//update transformation matrix for laser beam warning picture
	AEMtx33Scale(&laser_beam.warning_pic_scale, laser_beam.warning_pic_width, laser_beam.warning_pic_height);
	AEMtx33Trans(&laser_beam.warning_pic_translate, boss.center.x, boss.center.y);
	AEMtx33Concat(&laser_beam.warning_pic_matrix, &laser_beam.warning_pic_translate, &laser_beam.warning_pic_scale);
	//if time elapsed since previous laser beam attack is more than the buffer duration and
	//laser beam attack is not active, draw the warning sign
	if (laser_beam.time_elapsed >= laser_beam.buffer_duration && laser_beam.active == false) {
		AEGfxTextureSet(laser_beam.warning_pic, 0.0f, 0.0f);
		AEGfxSetTransform(laser_beam.warning_pic_matrix.m);
		AEGfxMeshDraw(square_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

//draws the boss bullets
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

