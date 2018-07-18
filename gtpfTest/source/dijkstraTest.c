/******************************************************************************
 *
 *   FILE
 *   ----
 *   dijkstraTest.c
 *
 *   History
 *   -------
 *   2018-07-18   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_gtpfTest
#define OV_COMPILE_LIBRARY_gtpfTest
#endif

#include "gtpfTest.h"
#include "gtpf.h"
#include "libov/ov_macros.h"

#include "CException.h"

#include "unity.h"
#include "unity_fixture.h"

#include "ovunity.h"
#include "ovunity_helper.h"

OV_INSTPTR_gtpfTest_dijkstraTest gpinst;
OV_INSTPTR_gtpf_dijkstra gpobj;
OV_TIME *gpltc;

TEST_GROUP(dijkstra);

TEST_SETUP(dijkstra) {
	ov_memstack_lock();
	//This is run before EACH TEST
	ov_string_setvalue(&gpobj->v_start, "PE004");
	ov_string_setvalue(&gpobj->v_topologie, NULL);

	Ov_SetDynamicVectorLength(&gpobj->v_recipe, 0, STRING);
	gpobj->v_recipe.value = NULL;

	gpobj->v_EN = 0;
}

TEST_TEAR_DOWN(dijkstra) {
	ov_memstack_unlock();
}

TEST(dijkstra, dijkstra_default) {
	//loading env
	OV_STRING case_path = ovunity_getCasePath(Ov_StaticPtrCast(ovunity_main, gpinst),
		"case_default");
	ovunity_loadEnv(Ov_StaticPtrCast(ovunity_main, gpinst), "default.json",
		case_path);
	//setting param
	ov_string_setvalue(&gpobj->v_start, "PE004");
	ov_string_setvalue(&gpobj->v_topologie, case_path);
	OV_STRING recipe[1] = { "PE010" };
	Ov_SetDynamicVectorValue(&gpobj->v_recipe, recipe, 1, STRING);

	//executing
	gtpf_dijkstra_typemethod(Ov_StaticPtrCast(fb_functionblock, gpobj), gpltc);

	//checking
	TEST_ASSERT_EQUAL(gpobj->v_result, 0);

	OV_UINT pathDirLen = 2;
	OV_STRING pathDirValue[] = { "right", "right" };
	TEST_ASSERT_EQUAL(gpobj->v_pathDir.veclen, pathDirLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathDirValue, gpobj->v_pathDir.value,
		pathDirLen);

	OV_UINT pathNodeLen = 2;
	OV_STRING pathNodeValue[] = { "PE034", "PE005" };
	TEST_ASSERT_EQUAL(gpobj->v_pathNode.veclen, pathNodeLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathNodeValue, gpobj->v_pathNode.value,
		pathNodeLen);

	OV_UINT paramLen = 2;
	OV_STRING paramValue[] = { NULL, NULL };
	TEST_ASSERT_EQUAL(gpobj->v_parameter.veclen, paramLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(paramValue, gpobj->v_parameter.value,
		paramLen);
}

TEST(dijkstra, dijkstra_badstart) {
	OV_STRING case_path = ovunity_getCasePath(Ov_StaticPtrCast(ovunity_main, gpinst),
			"case_badstart");
		ovunity_loadEnv(Ov_StaticPtrCast(ovunity_main, gpinst), "default.json",
			case_path);

	ov_string_setvalue(&gpobj->v_start, "PE0033");
	gtpf_dijkstra_typemethod(Ov_StaticPtrCast(fb_functionblock, gpobj), gpltc);
	TEST_ASSERT_EQUAL(gpobj->v_result, 4);
}

//test2 bad top
TEST(dijkstra, dijkstra_badtopo) {
//	OV_STRING case_path = ovunity_getCasePath(Ov_StaticPtrCast(ovunity_main, gpinst),
//			"case_badtopo");
//		ovunity_loadEnv(Ov_StaticPtrCast(ovunity_main, gpinst), "default.json",
//			case_path);
//	ov_logfile_info("test: bad topo");
	ov_string_setvalue(&gpobj->v_topologie, "PE03");
	gtpf_dijkstra_typemethod(Ov_StaticPtrCast(fb_functionblock, gpobj), gpltc);
	TEST_ASSERT_EQUAL(gpobj->v_result, 4);
}

//test3 start==end
TEST(dijkstra, dijkstra_start_same_end) {
	/* env */
	OV_STRING case_path = ovunity_getCasePath(Ov_StaticPtrCast(ovunity_main, gpinst),
			"case_start_same_end");
		ovunity_loadEnv(Ov_StaticPtrCast(ovunity_main, gpinst), "default.json",
			case_path);
//	ov_logfile_info("test: start==end");
	ov_string_setvalue(&gpobj->v_topologie, case_path);
	OV_STRING recipe[1] = { "PE004" };
	Ov_SetDynamicVectorValue(&gpobj->v_recipe, recipe, 1, STRING);

	gtpf_dijkstra_typemethod(Ov_StaticPtrCast(fb_functionblock, gpobj), gpltc);
	TEST_ASSERT_EQUAL(gpobj->v_result, 0);
	OV_UINT pathDirLen = 0;
	TEST_ASSERT_EQUAL(gpobj->v_pathDir.veclen, pathDirLen);

	OV_UINT pathNodeLen = 0;
	TEST_ASSERT_EQUAL(gpobj->v_pathNode.veclen, pathNodeLen);

	OV_UINT paramLen = 0;
	TEST_ASSERT_EQUAL(gpobj->v_parameter.veclen, paramLen);
}

TEST(dijkstra, dijkstra_heat_turn) {
	/* env */
	OV_STRING case_path = ovunity_getCasePath(Ov_StaticPtrCast(ovunity_main, gpinst),
			"case_heat_turn");
		ovunity_loadEnv(Ov_StaticPtrCast(ovunity_main, gpinst), "default.json",
			case_path);

	//test4
	ov_logfile_info("test: heat & turn");
	ov_string_setvalue(&gpobj->v_topologie, case_path);
	OV_STRING recipe[3] = { "PE009 HEAT", "PE025 TURN", "PE033" };
	Ov_SetDynamicVectorValue(&gpobj->v_recipe, recipe, 3, STRING);

	gtpf_dijkstra_typemethod(Ov_StaticPtrCast(fb_functionblock, gpobj), gpltc);
	TEST_ASSERT_EQUAL(gpobj->v_result, 0);

	OV_UINT pathDirLen = 16;
	OV_STRING pathDirValue[] = { "right", "right", "right", "right", "heat",
			"left", "left", "left", "down", "right", "right", "right", "turn",
			"right", "right", "right" };
	TEST_ASSERT_EQUAL(gpobj->v_pathDir.veclen, pathDirLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathDirValue, gpobj->v_pathDir.value,
		pathDirLen);

	OV_UINT pathNodeLen = 16;
	OV_STRING pathNodeValue[] = { "PE034", "PE005", "PE008", "PE009", "PE009",
			"PE008", "PE005", "PE034", "PE034", "PE031", "PE030", "PE025", "PE025",
			"PE028", "PE027", "PE033" };
	TEST_ASSERT_EQUAL(gpobj->v_pathNode.veclen, pathNodeLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathNodeValue, gpobj->v_pathNode.value,
		pathNodeLen);

	OV_UINT paramLen = 16;
	OV_STRING paramValue[] = { NULL, NULL, NULL, NULL, "150", NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, "180", NULL, NULL, NULL };
	TEST_ASSERT_EQUAL(gpobj->v_parameter.veclen, paramLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(paramValue, gpobj->v_parameter.value,
		paramLen);
}

TEST(dijkstra, dijkstra_heat2_turn2) {
	/* env */
	OV_STRING case_path = ovunity_getCasePath(Ov_StaticPtrCast(ovunity_main, gpinst),
			"case_heat2_turn2");
		ovunity_loadEnv(Ov_StaticPtrCast(ovunity_main, gpinst), "default.json",
			case_path);
	//test5
	ov_logfile_info("test: (heat&heat) & (turn&turn)");
	ov_string_setvalue(&gpobj->v_topologie, case_path);
	OV_STRING recipe[4] =
			{ "PE009 Heat", "PE009 Heat", "PE025 Turn", "PE025 Turn" };
	Ov_SetDynamicVectorValue(&gpobj->v_recipe, recipe, 4, STRING);

	gtpf_dijkstra_typemethod(Ov_StaticPtrCast(fb_functionblock, gpobj), gpltc);

	OV_UINT pathDirLen = 13;
	OV_STRING pathDirValue[] = { "right", "right", "right", "right", "heat",
			"left", "left", "left", "down", "right", "right", "right", "turn" };
	TEST_ASSERT_EQUAL(gpobj->v_pathDir.veclen, pathDirLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathDirValue, gpobj->v_pathDir.value,
		pathDirLen);

	OV_UINT pathNodeLen = 13;
	OV_STRING pathNodeValue[] = { "PE034", "PE005", "PE008", "PE009", "PE009",
			"PE008", "PE005", "PE034", "PE034", "PE031", "PE030", "PE025", "PE025" };
	TEST_ASSERT_EQUAL(gpobj->v_pathNode.veclen, pathNodeLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathNodeValue, gpobj->v_pathNode.value,
		pathNodeLen);

	OV_UINT paramLen = 13;
	OV_STRING paramValue[] = { NULL, NULL, NULL, NULL, "150", NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, "180" };
	TEST_ASSERT_EQUAL(gpobj->v_parameter.veclen, paramLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(paramValue, gpobj->v_parameter.value,
		paramLen);
}

TEST_GROUP_RUNNER(dijkstra) {
//	RUN_TEST_CASE(dijkstra, dijkstra_default);
	RUN_TEST_CASE(dijkstra, dijkstra_badstart);
	RUN_TEST_CASE(dijkstra, dijkstra_badtopo);
	RUN_TEST_CASE(dijkstra, dijkstra_start_same_end);
//	RUN_TEST_CASE(dijkstra, dijkstra_heat_turn);
//	RUN_TEST_CASE(dijkstra, dijkstra_heat2_turn2);
//  RUN_TEST_CASE(dijkstra, AnotherIgnoredTest);
//  RUN_TEST_CASE(dijkstra, ThisFunctionHasNotBeenTested_NeedsToBeImplemented);
}

static void RunAllTests(void) {
	RUN_TEST_GROUP(dijkstra);
}

OV_DLLFNCEXPORT void gtpfTest_dijkstraTest_typemethod(
		OV_INSTPTR_fb_functionblock pfb, OV_TIME *pltc) {
	/*
	 *   local variables
	 */

	gpinst = Ov_StaticPtrCast(gtpfTest_dijkstraTest, pfb);
	gpobj = &gpinst->p_obj;
	gpltc = pltc;

	const char* argv[] = { "dijkstra", "-v" };
	CEXCEPTION_T e;
	Try
			{
				UnityMain(2, argv, RunAllTests);
			}
				Catch(e)
	{
		ov_logfile_error("dijkstraTest cexception: %s", ov_result_getresulttext(e));
	}

	return;
}

