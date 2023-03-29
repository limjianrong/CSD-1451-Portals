#pragma once
#include "AEEngine.h"


enum { UP, DOWN,STOP, BTM_RIGHT, BTM_LEFT, TOP_RIGHT, TOP_LEFT };

struct Boss{
	f32 width{}, height{}, x_pos{}, y_pos{}, velocity{};
	f32 range_x{}, range_y{}; //100+200, 100+500
	s32 Hp{};
	s32 direction{ UP };
	f32 charge_angle{};

	// ---- Texture ----
	AEMtx33 scale{}, translate{}, matrix{};
	AEGfxTexture* standTex, * deadTex;
	AEVec2 center{};
};


// ------ Attack #1 ------
struct Laser_beam{
	int status{};
	f32 x{}, y{};
	f32 width{}, height{}, cooldown{}, duration{}, buffer_duration{};
	f32 max_duration{};
	f32 time_elapsed{};
	bool damaged_player{};
	AEVec2 center{};

	// ---- Texture ----
	AEMtx33 scale{}, translate{}, rotate{}, matrix{};
	AEGfxTexture* picture{};
	AEGfxTexture* warning_pic{};
	f32 warning_pic_width{}, warning_pic_height{};
	AEMtx33 warning_pic_scale{}, warning_pic_translate{}, warning_pic_matrix{};

};

// ------ Attack #2 ------
#define BOSS_TIMER 0.5f
struct Bullet {
	f32 x{}, y{};
	AEVec2 center{};
	f32 width, height;
	f32 speed;
	f32 timer;
	bool isTimerActive, isTeleported, isShooting;

	// ----- Mesh & Texture -----
	AEGfxTexture* bulletTex;
};

// ------ Attack #3  -------
struct Boss_charge {
	bool status{}, return_to_position{}, player_damaged{};
	f32 cooldown{}, range{}, velocity{}, time_elapsed{};
	s32 previous_direction{};

	//distance travelled by boss during charge attack
	f32 distance_travelled{}, direction_magnitude{};

	//original position when boss first starts the charge attack
	AEVec2 original_position{};

	//direction of boss's charge
	AEVec2 direction{};
	AEVec2 normalized_direction{};

	//endpoint of boss's charge
	AEVec2 endpoint{};

};

struct Boss_teleport {
	f32 cooldown{}, time_elapsed{};
	AEVec2 location{};

};
// ---- Main Functions ----
void boss_load();
void boss_init();
void boss_draw();
void boss_update();
void boss_free();
void boss_unload();


void boss_movement();
void boss_movement_teleport();
void bullet_update();
void bullet_draw();
void boss_laser_beam_attack();
void boss_charge_attack();
void draw_laser_beam();
void draw_laser_beam_warning();