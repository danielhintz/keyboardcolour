#include <libcmmk/libcmmk.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void printusage();

void
setkeyboardcols(struct cmmk *dev, long int *cols, int colSize) {
	struct cmmk_color_matrix map = {0};

	for(int i=0;i<colSize;i++) {
		for(int j=0;j<CMMK_COLS_MAX/colSize;j++) {
			int index = i*CMMK_COLS_MAX/colSize+j;
			for(int y=0;y<CMMK_ROWS_MAX;y++) {
				map.data[y][index] = MKRGB(cols[i]);
			}
		}
	}

	cmmk_set_customized_leds(dev, &map);
}

void
detach(struct cmmk *dev) {
	cmmk_set_control_mode(dev, CMMK_FIRMWARE);
	cmmk_detach(dev);
}

void
saveprofile(struct cmmk *dev) {
	cmmk_set_control_mode(dev, CMMK_PROFILE_CUSTOMIZATION);
	cmmk_save_active_profile(dev);
}

void
brighten(long int *col) {
	uint8_t r = (*col >> 16)&0xff;
	uint8_t g = (*col >> 8)&0xff;
	uint8_t b = *col & 0xff;

	uint8_t max = fmax(fmax(r, g), b);
	float amt = 255.0 / max;


	r=fmax(fmin(r*amt, 255),0);
	g=fmax(fmin(g*amt, 255),0);
	b=fmax(fmin(b*amt, 255),0);

	*col = (r<<16)|(g<<8)|b;
}

int
main(int argc, char** argv) {
	struct cmmk dev;
	int product;

	if (argc == 1) {
		printusage();
		return 1;
	}

	if (cmmk_find_device(&product) != 0) {
		fprintf(stderr, "Can't find device\n");
		return 1;
	}

	if (cmmk_attach(&dev, product, CMMK_LAYOUT_US_S) != 0) {
		fprintf(stderr, "Can't attach device\n");
		return 1;
	}

	long int *cols = malloc(sizeof(long int) * (argc-1));

	for(int i = 0;i<argc-1;i++) {
		cols[i] = strtol(argv[i+1], 0, 0);
		brighten(cols+i);
	}

	cmmk_set_control_mode(&dev, CMMK_EFFECT);
	cmmk_set_active_effect(&dev, CMMK_EFFECT_CUSTOMIZED);

	setkeyboardcols(&dev, cols, argc-1);

	saveprofile(&dev);
	detach(&dev);

	free(cols);

	return 0;
}

void
printusage() {
	fprintf(stderr, "keyboardcolour <COLOURS...>\n");
}
