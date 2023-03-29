#include "camera.hpp"
#include "Player.hpp"


extern float portal_max_range; //maximum range that a player can teleport
extern float moveSpeed; //for camera to follow player when player is on platform
extern Player_stats player;
Camera camera;
void camera_init() {
	// -------- Camera --------
	camera.x = 0;
	camera.y = 0; // Reset camera
	camera.free_moving = false;
	camera.buffer_range = portal_max_range;
}
void camera_update() {
	
	//check if player has upgraded portal range, if upgraded, then camera buffer range must be updated as well
	camera.buffer_range = portal_max_range;

	//check for player input to toggle free moving camera
	if (AEInputCheckTriggered(AEVK_B)) {
		camera.free_moving = !camera.free_moving;

		//if previous camera state was free_moving(for level-design), and B was pressed
		//switch back to camera state that is used for playing, now the camera goes back
		//to following the player
		if (camera.free_moving) {
			camera.x = player.x;
			camera.y = player.y;
		}
	}

	//if free moving camera is false, camera will follow the player's movement
	if (camera.free_moving == false) {
		if (AEGfxGetWinMaxX() - player.x <= camera.buffer_range && AEInputCheckCurr(AEVK_D)) {
			camera.x += 5 * player.Speed;
		}
		else if (AEGfxGetWinMaxX() - player.x <= camera.buffer_range) {
			camera.x += static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
		}

		if (player.x - AEGfxGetWinMinX() <= camera.buffer_range && AEInputCheckCurr(AEVK_A)) {
			camera.x -= 5 * player.Speed;
		}
		else if (player.x - AEGfxGetWinMinX() <= camera.buffer_range && camera.x >= 0) {
			camera.x -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * moveSpeed;
		}

		//if (player.x > AEGfxGetWinMaxX()) {
		//	camera.x = player.x;
		//}
		//else if (player.x < AEGfxGetWinMinX()) {
		//	if (player.x < 0) {
		//		camera.x = 0;
		//	}
		//	else {
		//		camera.x = player.x;
		//	}
		//}

		//camera will always follow player's y if player.y is +ve
		if (player.y > 0) {
			camera.y = player.y;
		}

		//if player.y is -ve, camera.y stays fixed at 0
		else if (player.y < 0) {
			camera.y = 0;
		}

	}
	//if free moving camera is true, can freely pan left/right to see the level design
	//mainly used for level designing/debugging purposes
	if (camera.free_moving == true) {
		if (AEInputCheckCurr(AEVK_I))
			camera.y += camera.free_moving_speed;

		if (AEInputCheckCurr(AEVK_K))
			camera.y -= camera.free_moving_speed;

		if (AEInputCheckCurr(AEVK_J))
			camera.x -= camera.free_moving_speed;

		if (AEInputCheckCurr(AEVK_L))
			camera.x += camera.free_moving_speed;
	}
}