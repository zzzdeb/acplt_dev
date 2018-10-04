/*
 * gitter.c
 *
 *  Created on: Jul 17, 2018
 *      Author: zzz
 */

#include "gitter.h"
#include "tgraph_geometry.h"
#include "CException.h"

OV_DLLFNCEXPORT int gitter2png(Gitter_t* gitter, OV_STRING name) {
	FILE * fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	size_t x, y;
	png_byte ** row_pointers = NULL;
	/* "status" contains the return value of this function. At first
	 it is set to a value which means 'failure'. When the routine
	 has finished its work, it is set to a value which means
	 'success'. */
	int status = -1;
	/* The following number is set by trial and error only. I cannot
	 see where it it is documented in the libpng manual.
	 */
	int pixel_size = 1;
	int depth = 8;

	OV_STRING path = NULL;
	char* ahome = getenv("ACPLT_HOME");
	ov_string_print(&path, "%s/dev/%s.png", ahome, name);

	fp = fopen(path, "wb");
	if(!fp) {
		goto fopen_failed;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	/* Set up error handling. */

	if(setjmp(png_jmpbuf (png_ptr))) {
		goto png_failure;
	}

	/* Set image attributes. */

	png_set_IHDR(png_ptr, info_ptr, gitter->width, gitter->height, depth,
	PNG_COLOR_TYPE_GRAY,
	PNG_INTERLACE_NONE,
	PNG_COMPRESSION_TYPE_DEFAULT,
	PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */

	row_pointers = png_malloc(png_ptr, gitter->height * sizeof(png_byte *));
	for (y = 0; y < gitter->height; y++) {
		png_byte *row = png_malloc(png_ptr,
			sizeof(uint8_t) * gitter->width * pixel_size);
		row_pointers[gitter->height - y - 1] = row;
		for (x = 0; x < gitter->width; x++) {
			Cell_t * pixel = cell_at(gitter, x, y);
			*row++ = 255 * !pixel->abnehmbar;

			//todo: init all abgebbar to zero
		}
	}

	/* Write the image data to "fp". */

	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* The routine has successfully written the file, so we set
	 "status" to a value which indicates success. */

	status = 0;

	for (int y = 0; y < gitter->height; y++) {
		png_free(png_ptr, row_pointers[y]);
	}
	png_free(png_ptr, row_pointers);

	png_failure: png_create_info_struct_failed: png_destroy_write_struct(&png_ptr,
		&info_ptr);
	png_create_write_struct_failed: fclose(fp);
	fopen_failed: return status;
}

/*
 * Gitter Functions
 */
OV_DLLFNCEXPORT Gitter_t* gitterConstruct() {
	Gitter_t* w1Gitter = ov_memstack_alloc(sizeof(Gitter_t));
	w1Gitter->pWagon = NULL;
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

OV_DLLFNCEXPORT void gitterDestruct(Gitter_t* gitter) {
	Ov_HeapFree(gitter->A);
}

OV_DLLFNCEXPORT Cell_t* cell_at_rel2global(Gitter_t* gitter, OV_INT x, OV_INT y) {
	OV_INT cellx = (x - gitter->x) / gitter->step;
	OV_INT celly = (y - gitter->y) / gitter->step;
	if(cellx > gitter->width || celly > gitter->height) Throw(OV_ERR_BADPARAM);
	return gitter->A + celly * (gitter->width) + cellx;
}

OV_DLLFNCEXPORT Cell_t* cell_at(Gitter_t* gitter, OV_INT x, OV_INT y) {
	return gitter->A + y * (gitter->width) + x;
}

OV_DLLFNCEXPORT void reachable(Gitter_t* gitter, Point_t* pos, int dir, OV_SINGLE range) {
	Cell_t* cell = cell_at_rel2global(gitter, pos->x, pos->y);
	cell->abgebbar = 1;
}

OV_DLLFNCEXPORT void canTakeAtPoint(Gitter_t* gitter, OV_SINGLE x, OV_SINGLE y) {
	Cell_t* cell = cell_at_rel2global(gitter, x, y);
	cell->abnehmbar = 1;
}

OV_DLLFNCEXPORT void canTakeBetweenPoints(Gitter_t* gitter, Point_t* pnt1, Point_t* pnt2) {
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

OV_DLLFNCEXPORT void canTakeRect(Gitter_t* gitter, Rectangular_t* rect) {
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

OV_DLLFNCEXPORT void drawRect(Gitter_t* gitter, const Rectangular_t* rect) {
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

OV_DLLFNCEXPORT void drawAssoc(Gitter_t* gitter, const OV_INSTPTR_TGraph_Node wagon1,
		const OV_INSTPTR_TGraph_Node wagon2) {
	Position_t* pos1 = positionFromNode(wagon1);
	Position_t* pos2 = positionFromNode(wagon2);
	canTakeBetweenPoints(gitter, &pos1->pos, &pos2->pos);
}

OV_DLLFNCEXPORT void draw_top(Gitter_t* gitter, OV_INSTPTR_ov_domain ptop) {
	OV_INSTPTR_wandelbareTopologie_Node pchild = NULL;
	Ov_ForEachChildEx(ov_containment, ptop, pchild, wandelbareTopologie_Node)
	{
		Rectangular_t* rect = rectConstruct();
		rect->b = pchild->v_Xlength;
		rect->h = pchild->v_Ylength;

		rect->pos.pos.x = pchild->v_x;
		rect->pos.pos.y = pchild->v_y;
		rect->pos.dir = degToRad(pchild->v_ThetaZ);
		drawRect(gitter, rect);

		for (OV_INT x = pchild->v_TTPSVM.value[0]; x <= pchild->v_TTPSVP.value[0];
				x += 1) {
			for (OV_INT y = pchild->v_TTPSVM.value[1]; y <= pchild->v_TTPSVP.value[1];
					y += 1) {
				for (OV_INT v = pchild->v_TCSVM.value[2]; v <= pchild->v_TCSVP.value[2];
						v += 1) {
					//todo: type conversion correction
					rect->pos.dir = degToRad(pchild->v_ThetaZ + v);

					Point_t* schiebe = pointConstruct();
					schiebe->x = x;
					schiebe->y = y;
					pointRotate(schiebe, rect->pos.dir);
					rect->pos.pos.x = pchild->v_x + schiebe->x;
					rect->pos.pos.y = pchild->v_y + schiebe->y;

					drawRect(gitter, rect);
				}
			}
		}
	}
	pchild = NULL;
	OV_INSTPTR_TGraph_graph pgraph = NULL;
	Ov_ForEachChildEx(ov_containment, ptop, pgraph, TGraph_graph)
	{
		OV_INSTPTR_ov_domain Nodes = &pgraph->p_Nodes;
		OV_INSTPTR_TGraph_Node poi = NULL;
		Ov_ForEachChildEx(ov_containment, Nodes, poi, TGraph_Node)
		{
			OV_INSTPTR_TGraph_Edge poiEdgeOut = NULL;
			Ov_ForEachChildEx(TGraph_Start, poi, poiEdgeOut, TGraph_Edge)
			{
				OV_INSTPTR_TGraph_Node poiChild = Ov_StaticPtrCast(TGraph_Node, Ov_GetParent(TGraph_End, poiEdgeOut));
				drawAssoc(gitter, poi, poiChild);
			}
		}
	}
}

OV_DLLFNCEXPORT Gitter_t* createPics(OV_INSTPTR_wandelbareTopologie_Node w1) {


//		Gitter_t _
//		int Factory[i][y][x] = alloc(size(uint16) * x * y)

	Gitter_t* w1Gitter = gitterConstruct();

	w1Gitter->pWagon = w1;

	//todo: zur zeit keine verwendung von einzel bilder
//	return w1Gitter;

	int XRATE = 1;
	int YRATE = 1;
	int VRATE = 1;

	Rectangular_t* rect = rectConstruct();
	rect->b = w1->v_Xlength;
	rect->h = w1->v_Ylength;
	for (OV_INT x = w1->v_TTPSVM.value[0]; x <= w1->v_TTPSVP.value[0]; x +=
			XRATE) {
		for (OV_INT y = w1->v_TTPSVM.value[1]; y <= w1->v_TTPSVP.value[1]; y +=
				YRATE) {
			for (OV_INT v = w1->v_TCSVM.value[2]; v <= w1->v_TCSVP.value[2]; v +=
					VRATE) {
				//todo: type conversion correction
				rect->pos.dir = degToRad(w1->v_ThetaZ + v);
				Point_t* schiebe = pointConstruct();
				schiebe->x = x;
				schiebe->y = y;
				pointRotate(schiebe, rect->pos.dir);
				rect->pos.pos.x = w1->v_x + schiebe->x;
				rect->pos.pos.y = w1->v_y + schiebe->y;

				canTakeRect(w1Gitter, rect);
//				if(w1->v_CSV.value[0])
//					reachable(w1Gitter, &currentPos.pos, 0, w1->v_Xlength);
//				if(w1->v_CSV.value[1])
//					reachable(w1Gitter, &currentPos.pos, 90, w1->v_Ylength);
			}
		}
	}

//	gitter2png(w1Gitter, w1->v_identifier);

//		for (OV_UINT i = 0; i < numOfTops - 1; ++i) {
//			for (OV_UINT j = i + 1; j < numOfTops; ++j) {
//				OV_BOOL Filter[ynum][xnum] = filter(Factory[i], Factory[j], canTake)
//			for l in y:
//			for m in x:
//			if(Filter[l][m]):
//
//		}
	return w1Gitter;
}


