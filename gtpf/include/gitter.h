/*
 * gitter.h
 *
 *  Created on: Jul 17, 2018
 *      Author: zzz
 */

#ifndef GITTER_H_
#define GITTER_H_

#include <png.h>
#include "libov/ov_ov.h"
#include "geometry2d_.h"
#include "gtpf.h"


typedef struct Cell {
	OV_BOOL abnehmbar;
	OV_BOOL abgebbar;
	OV_BOOL dir[1];
} Cell_t;

typedef struct Gitter {
	int x;
	int y;
	int height;
	int width;
	OV_SINGLE step;
	Cell_t* A;
	OV_INSTPTR_wandelbareTopologie_Node pWagon;
} Gitter_t;

enum DRAWINGFORMAT {
	DRAWDEFAULT = 0, DRAWNODES = 1, DRAWREACHABLE = 2, DRAWASSOC = 4, DRAWPOI = 8
};

/*
 * Gitter Functions
 */
Gitter_t* gitterConstruct();

void gitterDestruct(Gitter_t* gitter);

Cell_t* cell_at_rel2global(Gitter_t* gitter, OV_INT x, OV_INT y);

Cell_t* cell_at(Gitter_t* gitter, OV_INT x, OV_INT y);

void reachable(Gitter_t* gitter, Point_t* pos, int dir, OV_SINGLE range);

void canTakeAtPoint(Gitter_t* gitter, OV_SINGLE x, OV_SINGLE y);

void canTakeBetweenPoints(Gitter_t* gitter, Point_t* pnt1, Point_t* pnt2);

void canTakeRect(Gitter_t* gitter, Rectangular_t* rect);

int gitter2png(Gitter_t* gitter, OV_STRING name);

void draw_top(Gitter_t* gitter, OV_INSTPTR_ov_domain ptop,
		enum DRAWINGFORMAT format);

Gitter_t* createPics(OV_INSTPTR_wandelbareTopologie_Node w1);

#endif /* GITTER_H_ */
