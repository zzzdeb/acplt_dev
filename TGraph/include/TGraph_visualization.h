/*
 * TGraph_visualization.h
 *
 *  Created on: Jul 13, 2018
 *      Author: zzz
 */

#ifndef TGRAPH_VISUALIZATION_H_
#define TGRAPH_VISUALIZATION_H_

//#include <png.h>

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
	OV_INSTPTR_TGraph_Node Node;
} Gitter_t;
/*
 * Gitter Functions
 */
Gitter_t* gitterConstruct() {
	Gitter_t* w1Gitter = ov_memstack_alloc(sizeof(Gitter_t));
//	w1Gitter->pWagon = NULL;
	w1Gitter->x = 0;
	w1Gitter->y = 0;
	w1Gitter->height = 1000;
	w1Gitter->width = 1000;
	w1Gitter->step = 1;
	w1Gitter->A = Ov_HeapMalloc(
		sizeof(Cell_t) * w1Gitter->height * w1Gitter->width);
	for (OV_UINT i = 0; i < w1Gitter->height * w1Gitter->width; ++i) {
		(w1Gitter->A + i)->abgebbar = 0;
		(w1Gitter->A + i)->abnehmbar = 0;
	}
	return w1Gitter;
}

Cell_t* cell_at_rel2global(Gitter_t* gitter, OV_INT x, OV_INT y) {
	OV_INT cellx = (x - gitter->x) / gitter->step;
	OV_INT celly = (y - gitter->y) / gitter->step;
	if(cellx > gitter->width || celly > gitter->height) Throw(OV_ERR_BADPARAM);
	return gitter->A + celly * (gitter->width) + cellx;
}

Cell_t* cell_at(Gitter_t* gitter, OV_INT x, OV_INT y) {
	return gitter->A + y * (gitter->width) + x;
}

void reachable(Gitter_t* gitter, Point_t* pos, int dir, OV_SINGLE range) {
	Cell_t* cell = cell_at_rel2global(gitter, pos->x, pos->y);
	cell->abgebbar = 1;
}

void canTakeAtPoint(Gitter_t* gitter, OV_SINGLE x, OV_SINGLE y) {
	Cell_t* cell = cell_at_rel2global(gitter, x, y);
	cell->abnehmbar = 1;
}

void canTakeBetweenPoints(Gitter_t* gitter, Point_t* pnt1, Point_t* pnt2) {
	OV_SINGLE dist = sqrt(
		pow((pnt2->x - pnt1->x), 2) + pow((pnt2->y - pnt1->y), 2));
	OV_INT numOfSteps = dist / gitter->step + 1;
	OV_SINGLE xrate = (pnt2->x - pnt1->x) / numOfSteps;
	OV_SINGLE yrate = (pnt2->y - pnt1->y) / numOfSteps;
	for (OV_UINT i = 0; i < numOfSteps; ++i) {
		canTakeAtPoint(gitter, pnt1->x + i * xrate, pnt1->y + i * yrate);
	}
	canTakeAtPoint(gitter, pnt2->x, pnt2->y);
}

void canTakeRect(Gitter_t* gitter, Rectangular_t* rect) {
	Point_t* c1 = pointConstruct();
	Point_t* c2 = pointConstruct();
	Point_t* c3 = pointConstruct();
	Point_t* c4 = pointConstruct();
	rectGetCorners(rect, c1, c2, c3, c4);
	canTakeBetweenPoints(gitter, c1, c2);
	canTakeBetweenPoints(gitter, c2, c3);
	canTakeBetweenPoints(gitter, c3, c4);
	canTakeBetweenPoints(gitter, c4, c1);
}

//Rectangular_t* createRectFromNode(OV_INSTPTR_TGraph_Node w1) {
//	Rectangular_t* rect = rectConstruct();
//	rect->b = w1->v_Xlength;
//	rect->h = w1->v_Ylength;
//	rect->pos.pos.x = w1->v_x;
//	rect->pos.pos.y = w1->v_y;
//	rect->pos.dir = degToRad(w1->v_ThetaZ);
//	return rect;
//}



//void visualize_graph(OV_INSTPTR_ov_domain ptop) {
//	Gitter_t* gitter = gitterConstruct();
//
//	OV_INSTPTR_wandelbareTopologie_Node pchild = NULL;
//	Ov_ForEachChildEx(ov_containment, ptop, pchild, wandelbareTopologie_Node)
//	{
//		Rectangular_t* rect = rectConstruct();
//		rect->b = pchild->v_Xlength;
//		rect->h = pchild->v_Ylength;
//
//		rect->pos.pos.x = pchild->v_x;
//		rect->pos.pos.y = pchild->v_y;
//		rect->pos.dir = degToRad(pchild->v_ThetaZ);
//		drawRect(gitter, rect);
//
//		for (OV_INT x = pchild->v_TTPSVM.value[0]; x <= pchild->v_TTPSVP.value[0];
//				x += 1) {
//			for (OV_INT y = pchild->v_TTPSVM.value[1]; y <= pchild->v_TTPSVP.value[1];
//					y += 1) {
//				for (OV_INT v = pchild->v_TCSVM.value[2]; v <= pchild->v_TCSVP.value[2];
//						v += 1) {
//					//todo: type conversion correction
//					rect->pos.dir = degToRad(pchild->v_ThetaZ + v);
//
//					Point_t* schiebe = pointConstruct();
//					schiebe->x = x;
//					schiebe->y = y;
//					pointRotate(schiebe, rect->pos.dir);
//					rect->pos.pos.x = pchild->v_x + schiebe->x;
//					rect->pos.pos.y = pchild->v_y + schiebe->y;
//
//					drawRect(gitter, rect);
//				}
//			}
//		}
//	}
//	pchild = NULL;
//	Ov_ForEachChildEx(ov_containment, ptop, pchild, wandelbareTopologie_Node)
//	{
//		OV_INSTPTR_TGraph_Node poi = NULL;
//		Ov_ForEachChildEx(ov_containment, pchild, poi, TGraph_Node)
//		{
//			OV_INSTPTR_TGraph_Edge poiEdgeOut = NULL;
//			Ov_ForEachChildEx(ov_containment, poi, poiEdgeOut, TGraph_Edge)
//			{
//				OV_INSTPTR_TGraph_Node poiChild = NULL;
////			Ov_ForEachChildEx(TGraph_End, poi, poiChild, TGraph_Node)
////				if(neighbour) drawAssoc(gitter, pchild, neighbour);
//			}
//
//		}
//	}
//	gitter2png(gitter, "visualization");
//}

#endif /* TGRAPH_VISUALIZATION_H_ */
