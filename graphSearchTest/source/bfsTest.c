
/******************************************************************************
*
*   FILE
*   ----
*   bfsTest.c
*
*   History
*   -------
*   2018-06-04   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_graphSearchTest
#define OV_COMPILE_LIBRARY_graphSearchTest
#endif

#include <stdarg.h>

#include "graphSearchTest.h"
#include "libov/ov_macros.h"
//#include "libov/ov_ov.h"

#include "unity.h"
#include "unity_fixture.h"

OV_INSTPTR_graphSearchTest_bfsTest gpinst;
OV_TIME						*gpltc;



OV_RESULT ov_string_set_vecvalue_one(OV_STRING_VEC* vec, OV_UINT len, ...) {
	va_list args;
	va_start(args, len);
	OV_RESULT result = Ov_SetDynamicVectorLength(vec, len, STRING);
	for (OV_UINT i = 0; i < len; ++i) {
		result = ov_string_setvalue(&vec->value[i], va_arg(args, OV_STRING));
		if(Ov_Fail(result))
			return result;
	}
	va_end(args);
	return 0;
}

TEST_GROUP(bfs);

TEST_SETUP(bfs) {
	ov_memstack_lock();
	//This is run before EACH TEST
	ov_string_setvalue(&gpinst->v_start, "PE004");
	ov_string_setvalue(&gpinst->v_topologie, "/TechUnits/Topology");
	ov_string_set_vecvalue_one(&gpinst->v_recipe, 1, "PE005");
	gpinst->v_EN = 0;
}

TEST_TEAR_DOWN(bfs) {
	ov_memstack_unlock();
}

TEST(bfs, bfs_default) {
	graphSearch_bfs_typemethod(Ov_StaticPtrCast(fb_functionblock, gpinst), gpltc);

	TEST_ASSERT_EQUAL(gpinst->v_result, 0);

	OV_UINT pathDirLen = 2;
	OV_STRING pathDirValue[] = { "right", "right" };
	TEST_ASSERT_EQUAL(gpinst->v_pathDir.veclen, pathDirLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathDirValue, gpinst->v_pathDir.value,
		pathDirLen);

	OV_UINT pathNodeLen = 2;
	OV_STRING pathNodeValue[] = { "PE034", "PE005" };
	TEST_ASSERT_EQUAL(gpinst->v_pathNode.veclen, pathNodeLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathNodeValue, gpinst->v_pathNode.value,
		pathNodeLen);

	OV_UINT paramLen = 2;
	OV_STRING paramValue[] = { NULL, NULL };
	TEST_ASSERT_EQUAL(gpinst->v_parameter.veclen, paramLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(paramValue, gpinst->v_parameter.value, paramLen);
}

TEST(bfs, bfs_badstart){
	ov_string_setvalue(&gpinst->v_start, "PE0033");
	graphSearch_bfs_typemethod(Ov_StaticPtrCast(fb_functionblock, gpinst), gpltc);
	TEST_ASSERT_EQUAL(gpinst->v_result, 4);
}
	//test2 bad top
TEST(bfs, bfs_badtopo){
//	ov_logfile_info("test: bad topo");
	ov_string_setvalue(&gpinst->v_topologie, "PE03");
	graphSearch_bfs_typemethod(Ov_StaticPtrCast(fb_functionblock, gpinst), gpltc);
	TEST_ASSERT_EQUAL(gpinst->v_result, 4);
}

//test3 start==end
TEST(bfs, bfs_start_same_end){
//	ov_logfile_info("test: start==end");
	ov_string_set_vecvalue_one(&gpinst->v_recipe, 1, "PE004");
	graphSearch_bfs_typemethod(Ov_StaticPtrCast(fb_functionblock, gpinst), gpltc);
	TEST_ASSERT_EQUAL(gpinst->v_result,0);
	OV_UINT pathDirLen = 0;
	TEST_ASSERT_EQUAL(gpinst->v_pathDir.veclen, pathDirLen);

	OV_UINT pathNodeLen = 0;
	TEST_ASSERT_EQUAL(gpinst->v_pathNode.veclen, pathNodeLen);

	OV_UINT paramLen = 0;
	TEST_ASSERT_EQUAL(gpinst->v_parameter.veclen, paramLen);
}

TEST(bfs, bfs_heat_turn){
	//test4
	ov_logfile_info("test: heat & turn");
	ov_string_set_vecvalue_one(&gpinst->v_recipe, 3, "PE009 Heat", "PE025 Turn",
		"PE033");
	graphSearch_bfs_typemethod(Ov_StaticPtrCast(fb_functionblock, gpinst), gpltc);
	TEST_ASSERT_EQUAL(gpinst->v_result, 0);

	OV_UINT pathDirLen = 16;
	OV_STRING pathDirValue[] = { "right", "right", "right", "right",
			"heat", "left", "left", "left", "down", "right", "right", "right", "turn",
			"right", "right", "right" };
	TEST_ASSERT_EQUAL(gpinst->v_pathDir.veclen, pathDirLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathDirValue, gpinst->v_pathDir.value,
		pathDirLen);

	OV_UINT pathNodeLen = 16;
	OV_STRING pathNodeValue[] = { "PE034", "PE005", "PE008", "PE009",
			"PE009", "PE008", "PE005", "PE034", "PE034", "PE031", "PE030", "PE025",
			"PE025", "PE028", "PE027", "PE033" };
	TEST_ASSERT_EQUAL(gpinst->v_pathNode.veclen, pathNodeLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathNodeValue, gpinst->v_pathNode.value,
		pathNodeLen);

	OV_UINT paramLen = 16;
	OV_STRING paramValue[] = {NULL, NULL, NULL, NULL, "150", NULL, NULL, NULL,
			NULL, NULL, NULL, NULL, "180", NULL, NULL, NULL };
	TEST_ASSERT_EQUAL(gpinst->v_parameter.veclen, paramLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(paramValue, gpinst->v_parameter.value, paramLen);
}

TEST(bfs, bfs_heat2_turn2){
	//test5
//	ov_logfile_info("test: (heat&heat) & (turn&turn)");
	ov_string_set_vecvalue_one(&gpinst->v_recipe, 3, "PE009 Heat", "PE009 Heat",
		"PE025 Turn", "PE025 Turn");
	graphSearch_bfs_typemethod(Ov_StaticPtrCast(fb_functionblock, gpinst), gpltc);

	OV_UINT pathDirLen = 13;
	OV_STRING pathDirValue[] = { "right", "right", "right", "right",
			"heat", "left", "left", "left", "down", "right", "right", "right",
			"turn" };
	TEST_ASSERT_EQUAL(gpinst->v_pathDir.veclen, pathDirLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathDirValue, gpinst->v_pathDir.value,
		pathDirLen);

	OV_UINT pathNodeLen = 13;
	OV_STRING pathNodeValue[] = { "PE034", "PE005", "PE008", "PE009",
			"PE009", "PE008", "PE005", "PE034", "PE034", "PE031", "PE030", "PE025",
			"PE025" };
	TEST_ASSERT_EQUAL(gpinst->v_pathNode.veclen, pathNodeLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(pathNodeValue, gpinst->v_pathNode.value,
		pathNodeLen);

	OV_UINT paramLen = 13;
	OV_STRING paramValue[] = {NULL, NULL, NULL, NULL, "150", NULL, NULL, NULL,
			NULL, NULL, NULL, NULL, "180" };
	TEST_ASSERT_EQUAL(gpinst->v_parameter.veclen, paramLen);
	TEST_ASSERT_EQUAL_STRING_ARRAY(paramValue, gpinst->v_parameter.value, paramLen);
}

TEST_GROUP_RUNNER(bfs)
{
  RUN_TEST_CASE(bfs, bfs_default);
  RUN_TEST_CASE(bfs, bfs_badstart);
  RUN_TEST_CASE(bfs, bfs_badtopo);
  RUN_TEST_CASE(bfs, bfs_start_same_end);
  RUN_TEST_CASE(bfs, bfs_heat_turn);
  RUN_TEST_CASE(bfs, bfs_heat2_turn2);
//  RUN_TEST_CASE(bfs, AnotherIgnoredTest);
//  RUN_TEST_CASE(bfs, ThisFunctionHasNotBeenTested_NeedsToBeImplemented);
}

static void RunAllTests(void)
{
	RUN_TEST_GROUP(bfs);
}
OV_DLLFNCEXPORT void graphSearchTest_bfsTest_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    gpinst = Ov_StaticPtrCast(graphSearchTest_bfsTest, pfb);
    gpltc = pltc;

    OV_STRING argv[] = {"BFS", "-v"};
    UnityMain(2, argv, RunAllTests);

    return;
}

