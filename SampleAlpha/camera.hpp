/*==================================================================================
* All content © 2023 DigiPen Institute of Technology Singapore, all rights reserved.
* File:					camera.hpp
* Course:				CSD1451
* Group Name:			Apparate
* Primary Author:		Lim Jian Rong (jianrong.lim@digipen.edu)
* Secondary Authors:	-
*
* Brief:
  This header file implements the functions declarations used for updating the position of the camera
  that is used to follow the player, as well as for the free moving camera mode.
==================================================================================*/
#pragma once
#include "AEEngine.h"
#include "Player.hpp"
#include "draw_level.hpp"

struct Camera {
	//position of camera
	f32 x{}, y{};

	//flag to toggle between free moving camera and camera that follows the player
	bool free_moving{};

	//buffer range for the camera, when the distance between the player and edge of the screen
	//if lesser than the buffer range, camera will start following the player
	float buffer_range{};

	//speed of offsetting the camera in free moving mode is const
	const float free_moving_speed{30.0f};
};

//initializes the position of the camera and its mode, as well as its buffer range
void camera_init();

//updates position of the camera and whether or not it is free moving
void camera_update();

