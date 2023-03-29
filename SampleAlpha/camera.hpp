#pragma once

#include "AEEngine.h"

struct Camera {
	f32 x{}, y{};
	bool free_moving{};
	float buffer_range{ };

	//free moving speed is const
	const float free_moving_speed{30.0f};
};

void camera_init();
void camera_update();

