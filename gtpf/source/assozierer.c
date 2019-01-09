/******************************************************************************
 *
 *   FILE
 *   ----
 *   assozierer.c
 *
 *   History
 *   -------
 *   2018-06-17   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_gtpf
#define OV_COMPILE_LIBRARY_gtpf
#endif

#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gtpf.h"

#include "libov/ov_association.h"
#include "libov/ov_class.h"
#include "libov/ov_macros.h"
#include "libov/ov_ov.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"

#include "fb.h"

#include "CTree.h"
#include "TGraph.h"
#include "tgraph_geometry.h"
#include "wandelbareTopologie.h"

#include "../../geometry2d/include/geometry2d_.h"
#include "list.h"

#include "gitter.h"

#include "CException.h"

OV_DLLFNCEXPORT OV_RESULT gtpf_assozierer_MAXGAP_set(
    OV_INSTPTR_gtpf_assozierer pobj, const OV_SINGLE value) {
  pobj->v_MAXGAP = value;
  return OV_ERR_OK;
}

#define DISCRETFACTOR 10
#define M_PI 3.14159265358979323846

#define MAX(a, b) (((a) < (b)) ? (b) : (a))

OV_INSTPTR_gtpf_assozierer gpinst = NULL;
OV_INSTPTR_TGraph_graph    ggraph = NULL;

Rectangular_t* createRectFromNode(OV_INSTPTR_wandelbareTopologie_Node w1) {
  Rectangular_t* rect = rectConstruct();
  rect->b = w1->v_Xlength;
  rect->h = w1->v_Ylength;
  rect->pos.pos.x = w1->v_x;
  rect->pos.pos.y = w1->v_y;
  rect->pos.dir = degToRad(w1->v_ThetaZ);
  return rect;
}

OV_BOOL canAssosiate(Rectangular_t* rect1, Rectangular_t* rect2) {
  // ecken der abgabe rechteck
  Point_t* c1 = pointConstruct();
  Point_t* c2 = pointConstruct();
  Point_t* c3 = pointConstruct();
  Point_t* c4 = pointConstruct();
  rectGetCorners(rect1, c1, c2, c3, c4);

  // check
  Degree_t  dirs[4] = {0, 90, 180, 270};
  OV_SINGLE dist[4] = {rect2->b, rect2->h};
  Point_t*  corners[4] = {c1, c2, c3, c4};

  Rectangular_t* abnehmRect = rectConstruct();

  for(OV_UINT i = 0; i < 4; ++i) {
    abnehmRect->h = dist[(i + 1) % 2];
    abnehmRect->b = gpinst->v_MAXGAP;
    abnehmRect->pos.dir = rect2->pos.dir + degToRad(dirs[i]);

    Point_t* gapVector = pointConstruct();
    gapVector->x = gpinst->v_MAXGAP / 2 + dist[i % 2] / 2;
    pointRotate(gapVector, abnehmRect->pos.dir);
    abnehmRect->pos.pos = *pointAdd(&rect2->pos.pos, gapVector);

    if(isPointInRect(abnehmRect, corners[i])) {
      if(isPointInRect(abnehmRect, corners[(i + 1) % 4])) {
        // seite1
        return 1;
        break;
      } else if(isPointInRect(abnehmRect, corners[(i + 3) % 4])) {
        // seite4
        return 1;
        break;
      } else {
        continue;
      }
    } else {
      if(isPointInRect(abnehmRect, corners[(i + 2) % 4]))
        if(isPointInRect(abnehmRect, corners[(i + 1) % 4])) {
          // seite2
          return 1;
          break;
        } else if(isPointInRect(abnehmRect, corners[(i + 3) % 4])) {
          // seite3
          return 1;
          break;
        } else {
          continue;
        }
      else {
        continue;
      }
    }
  }
  return 0;
}

OV_INSTPTR_TGraph_Node
createPOI(const OV_INSTPTR_wandelbareTopologie_Node wagen,
          const Rectangular_t* rect, const OV_STRING name) {
  OV_INSTPTR_TGraph_Node poi = NULL;
  OV_RESULT              result =
      Ov_CreateObject(TGraph_Node, poi, &(ggraph->p_Nodes), name);
  if(result) {
    Throw(result);
  }
  // set var
  poi->v_Position.value[0] = rect->pos.pos.x;
  poi->v_Position.value[1] = rect->pos.pos.y;
  poi->v_Position.value[2] = radToDeg(rect->pos.dir);

  // linking with wagen
  Ov_Link(wandelbareTopologie_POI, wagen, poi);
  if(result) {
    Throw(result);
  }
  // linking with other pois
  OV_INSTPTR_TGraph_Node poiChild = NULL;
  Ov_ForEachChildEx(wandelbareTopologie_POI, wagen, poiChild, TGraph_Node) {
    if(poi != poiChild) {
      TGraph_graph_linkNodes(ggraph, poi, poiChild);
      TGraph_graph_linkNodes(ggraph, poiChild, poi);
    }
  }
  return poi;
}

void createAssoc(Gitter_t* g1, Gitter_t* g2) {
  int XRATE = 1;
  int YRATE = 1;
  int VRATE = 1;

  OV_INSTPTR_wandelbareTopologie_Node w1 = g1->pWagon;
  OV_INSTPTR_wandelbareTopologie_Node w2 = g2->pWagon;

  Rectangular_t* rect1 = createRectFromNode(w1);
  Rectangular_t* rect2 = createRectFromNode(w2);

  OV_BOOL found = canAssosiate(rect1, rect2);

  for(OV_INT x1 = w1->v_TTPSVM.value[0]; x1 <= w1->v_TTPSVP.value[0] && !found;
      x1 += XRATE) {
    for(OV_INT y1 = w1->v_TTPSVM.value[1];
        y1 <= w1->v_TTPSVP.value[1] && !found; y1 += YRATE) {
      for(OV_INT x2 = w2->v_TTPSVM.value[0];
          x2 <= w2->v_TTPSVP.value[0] && !found; x2 += XRATE) {
        for(OV_INT y2 = w2->v_TTPSVM.value[1];
            y2 <= w2->v_TTPSVP.value[1] && !found; y2 += YRATE) {
          // rect1
          Point_t* schiebe = pointConstruct();
          schiebe->x = x1;
          schiebe->y = y1;
          pointRotate(schiebe, degToRad(w1->v_ThetaZ));
          rect1->pos.pos.x = w1->v_x + schiebe->x;
          rect1->pos.pos.y = w1->v_y + schiebe->y;

          // rect2
          schiebe->x = x2;
          schiebe->y = y2;
          pointRotate(schiebe, degToRad(w2->v_ThetaZ));
          rect2->pos.pos.x = w2->v_x + schiebe->x;
          rect2->pos.pos.y = w2->v_y + schiebe->y;

          // check distanz
          if(pointDist(&rect1->pos.pos, &rect2->pos.pos) >
             (gpinst->v_MAXGAP + MAX(rect1->b, rect1->h) / 2 +
              MAX(rect2->b, rect2->h) / 2)) {
            continue;
          }

          for(OV_INT v = w1->v_TCSVM.value[2];
              v <= w1->v_TCSVP.value[2] && !found; v += VRATE) {
            for(OV_INT v2 = w2->v_TCSVM.value[2];
                v2 <= w2->v_TCSVP.value[2] && !found; v2 += VRATE) {
              // todo: type conversion correction
              rect1->pos.dir = degToRad(w1->v_ThetaZ + v);
              rect2->pos.dir = degToRad(w2->v_ThetaZ + v2);

              found = canAssosiate(rect1, rect2);
            }
          }
        }
      }
    }
  }

  if(found) {
    OV_INSTPTR_wandelbareTopologie_Node wagens[] = {w1, w2};
    OV_INSTPTR_TGraph_Node              poi[] = {NULL, NULL};
    Rectangular_t*                      rects[] = {rect1, rect2};

    Radian_t piRotatedDir = rects[0]->pos.dir >= 0 ? rects[0]->pos.dir - M_PI
                                                   : rects[0]->pos.dir + M_PI;
    OV_BOOL angleCheck =
        radInRange(piRotatedDir,
                   degToRad(wagens[0]->v_ThetaZ + wagens[0]->v_TCSVM.value[2]),
                   degToRad(wagens[0]->v_TCSVP.value[2] + wagens[0]->v_ThetaZ));
    do {
      OV_SINGLE eps = 0.01;
      for(OV_UINT i = 0; i < 2; ++i) {
        // create
        OV_UINT j = 0;
        poi[i] = NULL;
        Ov_ForEachChildEx(wandelbareTopologie_POI, wagens[i], poi[i],
                          TGraph_Node) {
          if(abs(rects[i]->pos.pos.x - poi[i]->v_Position.value[0]) < eps &&
             abs(rects[i]->pos.pos.y - poi[i]->v_Position.value[1]) < eps &&
             abs(radToDeg(rects[i]->pos.dir) - poi[i]->v_Position.value[2]) <
                 eps)
            break;
          j++;
        }
        if(!poi[i]) {
          OV_STRING poiName = NULL;
          ov_string_print(&poiName, "%s_%d", wagens[i]->v_identifier, j);
          poi[i] = createPOI(wagens[i], rects[i], poiName);
        }
      }
      // linking
      if(!TGraph_graph_areNodesLinked(poi[0], poi[1])) {
        OV_INSTPTR_TGraph_Edge pedge =
            TGraph_graph_linkNodes(ggraph, poi[0], poi[1]);
        /*if wagons r different, set rotation to 0*/
        if(wagens[0] != wagens[1]) pedge->v_Direction.value[2] = 0;

        ov_logfile_info(
            "linking %s %s at position w1: [%f,%f,%f] w2: [%f,%f,%f]",
            poi[0]->v_identifier, poi[1]->v_identifier, rect1->pos.pos.x,
            rect1->pos.pos.y, radToDeg(rect1->pos.dir), rect2->pos.pos.x,
            rect2->pos.pos.y, radToDeg(rect2->pos.dir));
      }
      rects[0]->pos.dir = piRotatedDir;
    } while(0 < angleCheck--);
  }
}

OV_DLLFNCEXPORT OV_RESULT
                gtpf_assozierer_execute(OV_INSTPTR_gtpf_assozierer pinst) {
  // variables
  OV_RESULT result = OV_ERR_OK;
  //	Data_t* recipes = NULL;

  //	int xmax = calculateXmax();
  //	int ymax = calculateYmax();
  //	int xnum = xmax / DISCRETFACTOR;
  //	int ynum = ymax / DISCRETFACTOR;
  //
  // param check
  ov_memstack_lock();
  OV_INSTPTR_ov_domain ptop =
      Ov_StaticPtrCast(ov_domain, ov_path_getobjectpointer(pinst->v_Path, 2));
  OV_INSTPTR_wandelbareTopologie_Node pchild = NULL;
  if(!ptop) {
    ov_logfile_error("topology could not be found");
    return OV_ERR_BADPARAM;
  }
  ggraph = NULL;
  result = Ov_CreateObject(TGraph_graph, ggraph, ptop, "Graph");
  if(result) {
    if(result == OV_ERR_ALREADYEXISTS) {
      OV_STRING pathToGraph = NULL;
      ov_string_print(&pathToGraph, "%s/%s", pinst->v_Path, "Graph");
      ggraph = Ov_StaticPtrCast(TGraph_graph,
                                ov_path_getobjectpointer(pathToGraph, 2));
      Ov_DeleteObject(ggraph);
      result = Ov_CreateObject(TGraph_graph, ggraph, ptop, "Graph");
      if(result) Throw(result);
    } else {
      Throw(result);
    }
  }

  // safe
  if(!ggraph) Throw(OV_ERR_GENERIC);

  // creating list ov objects (no '_' in identifier)
  list_t* picList = constructList(sizeof(Gitter_t));
  Ov_ForEachChildEx(ov_containment, ptop, pchild, wandelbareTopologie_Node) {
    insertFirst(picList, createPics(pchild));
    // creating poi of current position
    OV_STRING poiName = NULL;
    ov_string_print(&poiName, "%s_0", pchild->v_identifier);
    Rectangular_p          rect = createRectFromNode(pchild);
    OV_INSTPTR_TGraph_Node poi = createPOI(pchild, rect, poiName);
    // Product skill
    for(OV_UINT i = 0; i < pchild->v_PSkills.veclen; ++i) {
      OV_STRING              psname = NULL;
      OV_INSTPTR_TGraph_Edge edge = TGraph_graph_linkNodes(ggraph, poi, poi);
      ov_string_print(&psname, "%s_%s", pchild->v_identifier,
                      pchild->v_PSkills.value[i]);
      ov_class_renameobject(Ov_StaticPtrCast(ov_object, edge), &ggraph->p_Edges,
                            psname, OV_PMH_DEFAULT, NULL);
    }
  }
  //	listSort(picList, relation);
  listNode_t* elem1 = NULL;
  listNode_t* elem2 = NULL;
  listIterate(picList, elem1) {
    listIterate(picList, elem2) {
      //			canGive(elem1, elem2){
      if(((Gitter_t*)elem1->data)->pWagon != ((Gitter_t*)elem2->data)->pWagon)
        createAssoc((Gitter_t*)elem1->data, (Gitter_t*)elem2->data);
    }
  }

  listIterate(picList, elem1) { gitterDestruct(elem1->data); }
  destructList(picList);

  // image
  Gitter_t* gitter = gitterConstruct();
  draw_top(gitter, ptop, pinst->v_drawingFormat);
  gitter2png(gitter, "gtpf/assozierer_viz");

  ov_memstack_unlock();
  return result;
}

OV_DLLFNCEXPORT void gtpf_assozierer_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                                OV_TIME* pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_gtpf_assozierer pinst = Ov_StaticPtrCast(gtpf_assozierer, pfb);
  gpinst = pinst;

  OV_RESULT    result = OV_ERR_OK;
  CEXCEPTION_T err;
  Try { result = gtpf_assozierer_execute(pinst); }
  Catch(err) { ov_logfile_error("%s", ov_result_getresulttext(err)); }
  ov_memstack_lock();
  switch(result) {
    case OV_ERR_OK:
      ov_logfile_info("gtpf: done");
      pinst->v_result = result;
      break;
    case OV_ERR_BADPARAM:
      ov_logfile_error("gtpf: bad param");
      pinst->v_result = result;
      break;
    default:
      ov_logfile_error("gtpf: failed \n error: %s",
                       ov_result_getresulttext(result));
      pinst->v_result = 1;
  }

  ov_memstack_unlock();
  return;
}

