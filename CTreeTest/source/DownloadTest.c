/******************************************************************************
 *
 *   FILE
 *   ----
 *   DownloadTest.c
 *
 *   History
 *   -------
 *   2018-10-15   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_CTreeTest
#define OV_COMPILE_LIBRARY_CTreeTest
#endif

#include "CTreeTest.h"
#include "libov/ov_macros.h"
#include "CTree.h"

#include "unity.h"
#include "unity_fixture.h"

#include "ovunity.h"
#include "ovunity_helper.h"

#include "CException.h"

OV_INSTPTR_ovunity_main gpinst;
OV_TIME *gpltc;

TEST_GROUP(Download);

TEST_SETUP( Download) {
	ov_memstack_lock();
	//This is run before EACH TEST
//	ov_string_setvalue(&gpinst->v_start, "PE004");
//	ov_string_set_vecvalue_one(&gpinst->v_recipe, 1, "PE005");
//	gpinst->v_EN = 0;
}

TEST_TEAR_DOWN( Download) {
	ov_memstack_unlock();
}

TEST( Download, Download_default) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_CTree_Download pobj = NULL;
	OV_STRING case_name = "case_testobj";
	OV_STRING case_path = ovunity_getCasePath(gpinst, case_name);

	/* create case */
	OV_INSTPTR_ovunity_ovCase pcase = ovunity_createCase(gpinst, case_name);
	if(!pcase) Throw(OV_ERR_GENERIC);

	ovunity_ovCase_build1(pcase);
	pobj = ovunity_ovCase_getObjPath(pcase);

//	OV_STRING dataPath = NULL;
//	char* ahome = getenv("ACPLT_HOME");
//	ov_string_print(&dataPath, "%s/dev/CTreeTest/test/Download/default.json",
//		ahome);
//	OV_STRING treestr = ovunity_helper_data2str(dataPath);
//	ov_string_setvalue(&pobj->v_json, treestr);

//	ovunity_compareIstSoll();

	ov_string_setvalue(&pobj->v_path, pcase->v_envPath);
	CTree_Download_typemethod(Ov_StaticPtrCast(fb_functionblock, pobj), gpltc);

	result = ovunity_compareIstSoll(pcase);

	TEST_ASSERT_EQUAL(result, 0);
}

TEST_GROUP_RUNNER( Download) {
	RUN_TEST_CASE(Download, Download_default);
}

static void RunAllTests(void) {
	RUN_TEST_GROUP(Download);
}

OV_DLLFNCEXPORT void CTreeTest_DownloadTest_typemethod(
		OV_INSTPTR_fb_functionblock pfb, OV_TIME *pltc) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_CTreeTest_DownloadTest pinst = Ov_StaticPtrCast(
		CTreeTest_DownloadTest, pfb);
	gpinst = Ov_StaticPtrCast(ovunity_main, pfb);
//	gpobj = &Ov_StaticPtrCast(CTreeTest_DownloadTest,gpinst)->p_obj;
	gpltc = pltc;

	//todo
	char* ahome = getenv("ACPLT_HOME");
	ov_string_print(&pinst->v_sysPath, "%s/dev/CTreeTest/test/DownloadTest",
		ahome);

	const char* argv[] = { "Download", "-v" };
	CEXCEPTION_T e;
	Try
			{
				UnityMain(2, argv, RunAllTests);
			}
				Catch(e)
	{
		ov_logfile_error("DownloadTest fail");
	}
	return;
}
