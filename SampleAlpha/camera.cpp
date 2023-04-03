/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					Camera.cpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors:	-
*
* Brief:
  This source file implements the functions used for updating the position of the camera
  that is used to follow the player, as well as for the free moving camera mode.
==================================================================================*/

#include "Camera.hpp"

extern float portal_max_range; //maximum range that a player can place a portal
//extern float moveSpeed; //for camera to follow player when player is on platform
extern Player_stats player;
Camera camera;
std::ifstream camera_ifs{};

void camera_load() {

	camera_ifs.open("Assets/textFiles/camera.txt");

#ifdef debug
	if (!camera_ifs) {
		std::cout << "\nFailed to open camera.txt";
	}
#endif
	std::string str{};
	camera_ifs >> str >> camera.buffer_range_multiplier;
	camera_ifs.close();
	



}
//sets the position of the camera and its mode, as well as its buffer range
void camera_init() {
	// -------- Camera --------

	camera.free_moving = false;
	//set buffer range multiplier value so that camera buffer range will be slightly larger than
	//the maximum range of the portal, so that player can always see slightly ahead
	//of the portal's maximum range
	camera.buffer_range = camera.buffer_range_multiplier * portal_max_range;

	//camera position will be set to player's initial starting
	//position, but camera position cannot be below (0,0) as there are no longer any platforms
	//below the screen when the camera is at (0,0)
	camera.x = static_cast<f32>(player.initial_pos_x);
	camera.y = static_cast<f32>(player.initial_pos_y);
}

//updates position of the camera and whether or not it is free moving
void camera_update() {
	
	//check if player has upgraded the maximum portal range, if upgraded, then camera buffer range must be updated as well
	camera.buffer_range = camera.buffer_range_multiplier * portal_max_range;

	//check for player input to toggle free moving camera
	if (AEInputCheckTriggered(AEVK_B)) {
		camera.free_moving = !camera.free_moving;

		//camera will be set to player's position once whenever switching between camera modes

		camera.x = player.center.x;
		camera.y = player.center.y;
		
	}

	//if free moving camera is false, camera will follow the player's movement
	if (camera.free_moving == false) {

		//player is moving and has reached the right buffer range of the camera
		if (AEGfxGetWinMaxX() - player.center.x <= camera.buffer_range && AEInputCheckCurr(AEVK_D)) {
			camera.x += FIXED_MOVEMENT * player.Speed;
		}

		//player is not moving but still reached the right buffer range of the camera
		//this is because the moving platform is also moving the player
		else if (AEGfxGetWinMaxX() - player.center.x <= camera.buffer_range) {
			camera.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
		}

		//player is moving and has reached the left buffer range of the camera
		if (player.center.x - AEGfxGetWinMinX() <= camera.buffer_range && AEInputCheckCurr(AEVK_A)) {
			camera.x -= FIXED_MOVEMENT * player.Speed;
		}

		//player is not moving but still reached the left buffer range of the camera
		//this is because the moving platform is also moving the player
		else if (player.center.x - AEGfxGetWinMinX() <= camera.buffer_range && camera.x >= 0) {
			camera.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * MOVESPEED;
		}

		//coordinate of camera should not go below (0,0) as there is no level design implemented
		//below AEGfxGetWinMinX and AEGfxGetWinMinY when camera.x and camera.y = 0.
		camera.y = player.center.y;
	}
	//if free moving camera is true, can freely pan left/right to see the level design
	//mainly used for level designing/debugging purposes
	if (camera.free_moving == true) {

		//if key 'I' is pressed, camera pans upwards
		if (AEInputCheckCurr(AEVK_I))
			camera.y += camera.free_moving_speed;

		//if key 'K' is pressed, camera pans downwards
		if (AEInputCheckCurr(AEVK_K))
			camera.y -= camera.free_moving_speed;

		//if key 'J' is pressed, camera to the left
		if (AEInputCheckCurr(AEVK_J))
			camera.x -= camera.free_moving_speed;

		//if key 'L' is pressed, camera to the right
		if (AEInputCheckCurr(AEVK_L))
			camera.x += camera.free_moving_speed;
	}
	camera.x = AEClamp(camera.x, 0, static_cast<f32>(INT_MAX));
	camera.y = AEClamp(camera.y, 0, static_cast<f32>(INT_MAX));
}