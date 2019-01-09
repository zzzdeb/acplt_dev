#ifndef GEOMETRY2D_H_
#define GEOMETRY2D_H_

//version 1.0

#include "libov/ov_ov.h"
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

typedef Point_t* Point_p;

typedef struct {
	Point_t pos;
	Radian_t dir;
} Position_t;

typedef Position_t* Position_p;

typedef struct {
	Position_t pos;
	OV_SINGLE b;
	OV_SINGLE h;
} Rectangular_t;

typedef Rectangular_t* Rectangular_p;

/*
 * Angle functions
 */

// Converts degrees to radians.
#define degToRad(angleDegrees) ((angleDegrees) * M_PI / 180.0)

// Converts radians to degrees.
#define radToDeg(angleRadians) ((angleRadians) * 180.0 / M_PI)

OV_BOOL radInRange(const Radian_t angle, const Radian_t min, const Radian_t max);

Radian_t radianSubstract(const Radian_t r1, const Radian_t r2);
/*
 Point functions
 */
void pointInit(Point_t* pnt);

Point_t* pointConstruct();


Point_t* pointAdd(Point_t* p1, Point_t* p2);


Point_t* pointSubstract(const Point_t* p1, const Point_t* p2);


void pointRotate(Point_t* pnt, const Radian_t alpha);


OV_SINGLE pointNorm(const Point_t* p) ;

OV_SINGLE pointDist(const Point_t* p1, const Point_t* p2);

Radian_t pointGetDir(const Point_t* p);
/*
 * Position functions
 */
void positionInit(Position_t* pos) ;

Position_t* positionConstruct() ;

Position_p positionSubstract(const Position_p p1, const Position_p p2);

/*
 * Rect functions
 */
void rectInit(Rectangular_t* rec) ;

Rectangular_t* rectConstruct();

void rectGetCorners(const Rectangular_t* rect, Point_t* c1, Point_t* c2,
		Point_t* c3, Point_t* c4);

#define EPSILON 1

OV_RESULT isPointInRect(const Rectangular_t* rect, const Point_t* pnt) ;

#endif
