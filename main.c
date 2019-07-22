#include <libcmmk/libcmmk.h>

#include <unistd.h> /* usleep() */
#include <signal.h>
#include <time.h> /* do we need this? */

#include <string.h> /* memset() */
#include <stdio.h>
#include <stdlib.h>

void
multilayer(struct cmmk *dev) {
	struct cmmk_effect_matrix map = {0};

	struct cmmk_effect_fully_lit f = {
		.color = MKRGB(0x404040)
	};

	struct cmmk_effect_wave w = {
		.speed = 0x50,
		.direction = CMMK_LEFT_TO_RIGHT,
		.start = MKRGB(0xffffff)
	};

	struct cmmk_effect_raindrops r = {
		.speed = 0x20,
		.interval = 0x30,
		.active = MKRGB(0xffffff),
		.rest = MKRGB(0x000000)
	};

	map.data[0][0] = CMMK_EFFECT_WAVE;
	map.data[4][16] = CMMK_EFFECT_WAVE;
	map.data[5][15] = CMMK_EFFECT_WAVE;
	map.data[5][16] = CMMK_EFFECT_WAVE;
	map.data[5][17] = CMMK_EFFECT_WAVE;

	memset(&map.data[1], CMMK_EFFECT_RAINDROPS, 15);
	memset(&map.data[2], CMMK_EFFECT_RAINDROPS, 15);
	memset(&map.data[3], CMMK_EFFECT_RAINDROPS, 15);
	memset(&map.data[4], CMMK_EFFECT_RAINDROPS, 15);
	memset(&map.data[5], CMMK_EFFECT_RAINDROPS, 15);

	cmmk_set_control_mode(dev, CMMK_EFFECT);

	cmmk_switch_multilayer(dev, 1);
	cmmk_set_effect_fully_lit(dev, &f);
	cmmk_set_effect_wave(dev, &w);
	cmmk_set_effect_raindrops(dev, &r);
	cmmk_switch_multilayer(dev, 0);

	cmmk_set_multilayer_map(dev, &map);
	cmmk_set_active_effect(dev, CMMK_EFFECT_MULTILAYER);
}


//TODO: commandline arguments
int main(int argc, char** argv) {
	struct cmmk state;

	int product;

	if (cmmk_find_device(&product) != 0) {
		return 1;
	}

	if (cmmk_attach(&state, product, CMMK_LAYOUT_US_S) != 0) {
		return 1;
	}

	struct cmmk_effect_stars r = {
		.speed = 0x30,
		.interval = 0x10,

		.active = MKRGB(0xffffff),
		.rest = MKRGB(0x0)
	};


	cmmk_set_control_mode(&state, CMMK_EFFECT);
	cmmk_set_effect_stars(&state, &r);

	cmmk_set_active_effect(&state, CMMK_EFFECT_STARS);

	cmmk_set_control_mode(&state, CMMK_PROFILE_CUSTOMIZATION);
	cmmk_save_active_profile(&state);

	cmmk_set_control_mode(&state, CMMK_FIRMWARE);

	cmmk_detach(&state);

	return 0;
}
