#pragma once


void bullet_initialise(void);
void weapon_fire(f32 player_x, f32 player_y, int state);
bool isbullet_enemy_colliding(f32 bullet_x, f32 bullet_y);
