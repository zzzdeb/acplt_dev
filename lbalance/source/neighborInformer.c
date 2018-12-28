/******************************************************************************
 *
 *   FILE
 *   ----
 *   neighborInformer.c
 *
 *   History
 *   -------
 *   Fr 28 Dez 2018 12:58:11 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalance
#define OV_COMPILE_LIBRARY_lbalance
#endif

#include "lbalance.h"

#include "libov/ov_macros.h"


OV_DLLFNCEXPORT void lbalance_neighborInformer_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                           OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalance_neighborInformer pinst = Ov_StaticPtrCast(lbalance_neighborInformer, pfb);

  /* param check  */
  

  

  return;
}

