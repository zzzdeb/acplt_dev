/*
 * tgraph_geometry.h
 *
 *  Created on: Jul 13, 2018
 *      Author: zzz
 */

#ifndef TGRAPH_GEOMETRY_H_
#define TGRAPH_GEOMETRY_H_

#include "geometry2d_.h"

/**
 * @brief gets position of TGraph_Node in Position_t type
 *
 * @param node
 *
 * @return
 */
Position_t* positionFromNode(const OV_INSTPTR_TGraph_Node node);

#endif /* TGRAPH_GEOMETRY_H_ */
