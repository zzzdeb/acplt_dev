/*
 * tgraph_geometry.h
 *
 *  Created on: Jul 13, 2018
 *      Author: zzz
 */

#ifndef TGRAPH_GEOMETRY_H_
#define TGRAPH_GEOMETRY_H_

#include "geometry2d_.h"

Position_t* positionFromNode(const OV_INSTPTR_TGraph_Node node) {
	Position_t* pos = positionConstruct();
	pos->pos.x = node->v_Position.value[0];
	pos->pos.y = node->v_Position.value[1];
	pos->dir = degToRad(node->v_Position.value[2]);
	return pos;
}

#endif /* TGRAPH_GEOMETRY_H_ */
