/*
 * tgraph_geometry.c
 *
 *  Created on: Jul 18, 2018
 *      Author: zzz
 */
#include "tgraph_geometry.h"
#include "TGraph.h"

OV_DLLFNCEXPORT Position_t*
                positionFromNode(const OV_INSTPTR_TGraph_Node node) {
  Position_t* pos = positionConstruct();
  pos->pos.x = node->v_Position.value[0];
  pos->pos.y = node->v_Position.value[1];
  pos->dir = degToRad(node->v_Position.value[2]);
  return pos;
}
