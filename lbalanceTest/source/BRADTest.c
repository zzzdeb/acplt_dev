/******************************************************************************
 *
 *   FILE
 *   ----
 *   BRADTest.c
 *
 *   History
 *   -------
 *   Mi 02 Jan 2019 22:53:41 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lbalanceTest
#define OV_COMPILE_LIBRARY_lbalanceTest
#endif

#include "lbalance.h"
#include "lbalanceTest.h"

#include "libov/ov_macros.h"

#include "unity.h"
#include "unity_fixture.h"

#include "ovunity.h"
#include "ovunity_helper.h"

#include "CException.h"

OV_INSTPTR_ovunity_main gpinst;
OV_TIME*                gpltc;

TEST_GROUP(BRAD);

TEST_SETUP(BRAD) { ov_memstack_lock(); }

TEST_TEAR_DOWN(BRAD) { ov_memstack_unlock(); }

TEST(BRAD, BRAD_2serv_1gse) {
  OV_RESULT                result = OV_ERR_OK;
  OV_INSTPTR_lbalance_BRAD pobj = NULL;
  OV_STRING                case_name = "case_2serv_1gse";
  OV_STRING                case_path = ovunity_getCasePath(gpinst, case_name);

  /* create case */
  OV_INSTPTR_ovunity_ovCase pcase = ovunity_createCase(gpinst, case_name);
  if(!pcase)
    Throw(OV_ERR_GENERIC);

  ovunity_ovCase_build1(pcase);
  pobj = Ov_StaticPtrCast(lbalance_BRAD, ovunity_ovCase_getObjPath(pcase));

  lbalance_BRAD_typemethod(Ov_StaticPtrCast(fb_functionblock, pobj), gpltc);
}

TEST_GROUP_RUNNER(BRAD) { RUN_TEST_CASE(BRAD, BRAD_2serv_1gse); }

static void RunAllTests(void) { RUN_TEST_GROUP(BRAD); }

OV_DLLFNCEXPORT void
lbalanceTest_BRADTest_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                 OV_TIME*                    pltc) {
  /*
   *   local variables
   */
  OV_INSTPTR_lbalanceTest_BRADTest pinst =
      Ov_StaticPtrCast(lbalanceTest_BRADTest, pfb);

  gpinst = Ov_StaticPtrCast(ovunity_main, pfb);
  //	gpobj = &Ov_StaticPtrCast(lbalanceTest_BRAD,gpinst)->p_obj;
  gpltc = pltc;

  // todo
  char* ahome = getenv("ACPLT_HOME");
  ov_string_print(&pinst->v_sysPath, "%s/dev/lbalanceTest/test/BRADTest",
                  ahome);

  const char*  argv[] = {"BRAD", "-v"};
  CEXCEPTION_T e;
  Try { UnityMain(2, argv, RunAllTests); }
  Catch(e) {
    ov_logfile_error("BRADTest fail: %u: %s", e, ov_result_getresulttext(e));
  }
  return;
}
