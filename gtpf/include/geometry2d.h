#ifndef GEOMETRY2D_H_
#define GEOMETRY2D_H_

#include "libov/ov_macros.h"
#include <math.h>

# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.78539816339744830962	/* pi/4 */

typedef OV_SINGLE Radian_t;
typedef OV_SINGLE Degree_t;

typedef struct {
	OV_SINGLE x;
	OV_SINGLE y;
} Point_t;

typedef struct {
	Point_t pos;
	Radian_t dir;
} Position_t;

typedef struct {
	Position_t pos;
	OV_SINGLE b;
	OV_SINGLE h;
} Rectangular_t;

/*
 * Angle functions
 */

// Converts degrees to radians.
#define degToRad(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radToDeg(angleRadians) (angleRadians * 180.0 / M_PI)

/*
Point functions
*/
void pointInit(Point_t* pnt) {
	pnt->x = 0;
	pnt->y = 0;
}
;
Point_t* pointConstruct(){
	Point_t* pnt = ov_memstack_alloc(sizeof(Point_t));
	pointInit(pnt);
	return pnt;
};

Point_t* pointAdd(Point_t* p1, Point_t* p2) {
	Point_t* res = pointConstruct();
	res->x = p1->x + p2->x;
	res->y = p1->y + p2->y;
	return res;
}

Point_t* pointSubtract(Point_t* p1, Point_t* p2) {
	Point_t* res = pointConstruct();
	res->x = p1->x - p2->x;
	res->y = p1->y - p2->y;
	return res;
}

void pointRotate(Point_t* pnt, Radian_t alpha){
	OV_SINGLE x = pnt->x;
	OV_SINGLE y = pnt->y;
	pnt->x = x*cos(alpha) - y*sin(alpha);
	pnt->y = x*cos(alpha) + y*sin(alpha);
}
/*
 * Position functions
 */
void positionInit(Position_t* pos){
	pos->dir = 0;
	pointInit(&pos->pos);
}

/*
 * Rect functions
 */
void rectInit(Rectangular_t* rec){
	rec->b = 1;
	rec->h = 1;
	positionInit(&rec->pos);
}

Rectangular_t* rectConstruct(){
	Rectangular_t* rec = ov_memstack_alloc(sizeof(Rectangular_t));
	rectInit(rec);
	return rec;
}

void rectGetCorners(Rectangular_t* rect, Point_t* c1, Point_t* c2, Point_t* c3, Point_t* c4){
	OV_SINGLE cb = cos(rect->pos.dir) * rect->b / 2;
	OV_SINGLE ch = cos(rect->pos.dir + M_PI_2) * rect->h / 2;
	OV_SINGLE sb = sin(rect->pos.dir) * rect->b / 2;
	OV_SINGLE sh = sin(rect->pos.dir + M_PI_2) * rect->h / 2;
	c1->x = rect->pos.pos.x + cb - ch;
	c1->y	=	rect->pos.pos.y + sb + sh;

	c2->x = rect->pos.pos.x + cb + ch;
	c2->y = rect->pos.pos.y + sb - sh;

	c3->x = rect->pos.pos.x - cb + ch;
	c3->y = rect->pos.pos.y - sb - sh;

	c4->x = rect->pos.pos.x - cb - ch;
	c4->y = rect->pos.pos.y - sb + sh;
	return;
}

OV_RESULT isPointInRect(Rectangular_t* rect, Point_t* pnt) {
	Point_t* diff = pointSubtract(pnt, &rect->pos.pos);
	pointRotate(diff, -rect->pos.dir);
	return (rect->b/2)>= diff->x && (-rect->b/2)<= diff->x && (rect->h/2)>= diff->y && (-rect->h/2)<= diff->y;
}

#endif
