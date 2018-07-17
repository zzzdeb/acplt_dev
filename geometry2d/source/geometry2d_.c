/*
 * geometry2d.c
 *
 *  Created on: Jul 16, 2018
 *      Author: zzz
 */

#include "geometry2d_.h"

OV_DLLFNCEXPORT OV_BOOL radInRange(const Radian_t angle, const Radian_t min, const Radian_t max) {
	if(min <= max) return angle >= min && angle <= max;
	return angle <= max || angle >= min;
}


OV_DLLFNCEXPORT Radian_t radianSubstract(const Radian_t r1, const Radian_t r2){
	Radian_t res = r1-r2;
	if(res<-M_PI)
		res+=2*M_PI;
	if(res>M_PI)
		res-=2*M_PI;
	return res;
}

OV_DLLFNCEXPORT void pointInit(Point_t* pnt) {
	pnt->x = 0;
	pnt->y = 0;
}

OV_DLLFNCEXPORT Point_t* pointConstruct() {
	Point_t* pnt = ov_memstack_alloc(sizeof(Point_t));
	pointInit(pnt);
	return pnt;
}

OV_DLLFNCEXPORT Point_t* pointAdd(Point_t* p1, Point_t* p2) {
	Point_t* res = pointConstruct();
	res->x = p1->x + p2->x;
	res->y = p1->y + p2->y;
	return res;
}

OV_DLLFNCEXPORT Point_t* pointSubstract(const Point_t* p1, const Point_t* p2) {
	Point_t* res = pointConstruct();
	res->x = p1->x - p2->x;
	res->y = p1->y - p2->y;
	return res;
}

OV_DLLFNCEXPORT void pointRotate(Point_t* pnt, const Radian_t alpha) {
	OV_SINGLE x = pnt->x;
	OV_SINGLE y = pnt->y;
	pnt->x = x * cos(alpha) - y * sin(alpha);
	pnt->y = x * sin(alpha) + y * cos(alpha);
}

OV_DLLFNCEXPORT OV_SINGLE pointNorm(const Point_t* p) {
	return pow(pow(p->x, 2) + pow(p->y, 2), 0.5);
}
OV_DLLFNCEXPORT OV_SINGLE pointDist(const Point_t* p1, const Point_t* p2) {
	return pointNorm(pointSubstract(p1, p2));
}

OV_DLLFNCEXPORT Radian_t pointGetDir(const Point_t* p){
	return atan2(p->y, p->x);
}
/*
 * Position functions
 */
OV_DLLFNCEXPORT void positionInit(Position_t* pos) {
	pos->dir = 0;
	pointInit(&pos->pos);
}

OV_DLLFNCEXPORT Position_t* positionConstruct() {
	Position_t* pos = ov_memstack_alloc(sizeof(Position_t));
	positionInit(pos);
	return pos;
}

OV_DLLFNCEXPORT Position_p positionSubstract(const Position_p p1, const Position_p p2){
	Position_p res = positionConstruct();
	res->dir = radianSubstract(p1->dir,p2->dir);
	res->pos = *pointSubstract(&p1->pos, &p2->pos);
	return res;
}


/*
 * Rect functions
 */
OV_DLLFNCEXPORT void rectInit(Rectangular_t* rec) {
	rec->b = 1;
	rec->h = 1;
	positionInit(&rec->pos);
}

OV_DLLFNCEXPORT Rectangular_t* rectConstruct() {
	Rectangular_t* rec = ov_memstack_alloc(sizeof(Rectangular_t));
	rectInit(rec);
	return rec;
}

OV_DLLFNCEXPORT void rectGetCorners(const Rectangular_t* rect, Point_t* c1, Point_t* c2,
		Point_t* c3, Point_t* c4) {
	OV_SINGLE cb = cos(rect->pos.dir) * rect->b / 2;
	OV_SINGLE ch = cos(rect->pos.dir + M_PI_2) * rect->h / 2;
	OV_SINGLE sb = sin(rect->pos.dir) * rect->b / 2;
	OV_SINGLE sh = sin(rect->pos.dir + M_PI_2) * rect->h / 2;
	c1->x = rect->pos.pos.x + cb + ch;
	c1->y = rect->pos.pos.y + sb + sh;

	c2->x = rect->pos.pos.x + cb - ch;
	c2->y = rect->pos.pos.y + sb - sh;

	c3->x = rect->pos.pos.x - cb - ch;
	c3->y = rect->pos.pos.y - sb - sh;

	c4->x = rect->pos.pos.x - cb + ch;
	c4->y = rect->pos.pos.y - sb + sh;
	return;
}

#define EPSILON 1

OV_DLLFNCEXPORT OV_RESULT isPointInRect(const Rectangular_t* rect, const Point_t* pnt) {
	Point_t* diff = pointSubstract(pnt, &rect->pos.pos);
	pointRotate(diff, -rect->pos.dir);
	return (rect->b / 2) + EPSILON >= diff->x
			&& (-rect->b / 2) - EPSILON <= diff->x
			&& (rect->h / 2) + EPSILON >= diff->y
			&& (-rect->h / 2) - EPSILON <= diff->y;
}
