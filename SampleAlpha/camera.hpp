#pragma once

#include "AEEngine.h"

struct Camera {
	f32 x{}, y{};
	bool free_moving{};
	float buffer_range{}, speed{};
};

void camera_init();
void camera_update();

