#pragma once

const int asset_count{ 4 };
const int asset2_count{ 4 };

void tutorial_load(void);
void tutorial_init(void);
void tutorial_update(void);
void tutorial_draw(void);
void tutorial_free(void);
void tutorial_unload(void);

static enum asset {
	_player,
	portals,
	range,
	enemy1
};

static enum asset2 {
	enemy2,
	enemy3,
	enemy3_warning,
	_boss
};


