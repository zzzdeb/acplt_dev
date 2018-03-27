//OV_RESULT getNamedVariable(const OV_INSTPTR_ov_object pTargetObj, const OV_STRING targetVarname, OV_ANY *value){
//	OV_RESULT result = OV_ERR_OK;
//	OV_ELEMENT element;
//	OV_ELEMENT varElement;
//	OV_VTBLPTR_ov_object pVtblObj = NULL;
//
//	if(pTargetObj == NULL){
//		result = OV_ERR_BADPARAM;
//	}else if (Ov_CanCastTo(fb_functionchart, pTargetObj)&& !Ov_CanCastTo(ssc_SequentialStateChart, pTargetObj)){
//		//get variable in a functionchart
//		result = fb_functionchart_getport(Ov_StaticPtrCast(fb_functionchart, pTargetObj), targetVarname, value);
//	}else{
//		//get variable in a object
//		varElement.elemtype = OV_ET_NONE;
//		element.elemtype = OV_ET_OBJECT;
//		element.pobj = pTargetObj;
//
//		//search the variable for the get operation
//		ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, targetVarname);
//		if(varElement.elemtype == OV_ET_VARIABLE) {
//			//port found, use the getter to read the value
//			Ov_GetVTablePtr(ov_object, pVtblObj, pTargetObj);
//			result = pVtblObj->m_getvar(varElement.pobj, &varElement, value);
//		}else{
//			result = OV_ERR_BADPARAM;
//		}
//	}
//	if(result != OV_ERR_OK){
//		Ov_SetAnyValue(value,NULL);
//	}
//	return result;
//}

//OV_RESULT set_variable_values(CtreeUpload* pupload, cJSON* jsvariables, OV_INSTPTR_ov_object pobj) {
//	/*
//	*	parameter and result objects
//	*/
//
//	OV_RESULT res = OV_ERR_OK;
//
//	cJSON* jsvariable = NULL;
//	OV_STRING objpathwithpunct = NULL;
//
//	OV_INSTPTR_ksapi_setVar papiSet = &(pupload->pinst->p_apiSet);
//
//	ov_memstack_lock();
//	ov_string_setvalue(&objpathwithpunct, ov_path_getcanonicalpath(pobj, VERSION_FOR_CTREE));
//	ov_string_append(&objpathwithpunct, ".");
//	ov_memstack_unlock();
//
//	cJSON_ArrayForEach(jsvariable, jsvariables)
//	{
//		if (cJSON_GetArraySize(jsvariable) != VARIABLE_FORMAT_SIZE) {
//			print_log(pupload, ov_logfile_error, "%s error", cJSON_GetStringValue(jsvariable));
//			continue;
//		}
//		res = ov_string_setvalue(&papiSet->v_path, objpathwithpunct);
//		ov_string_append(&papiSet->v_path, jsvariable->string);
////		ov_string_append(&papiSet->v_path, "root");
//
//		papiSet->v_varValue = get_value_from_str(jsvariable);
//
//		//DEBUG
////		OV_ANY tmp;
////		tmp.value.vartype = OV_VT_STRING;
////		ov_string_setvalue(&tmp.value.valueunion.val_string, "testing");
////		ov_variable_setanyvalue(&papiSet->v_varValue, &tmp);
//
//		ksapi_setVar_submit(Ov_PtrUpCast(ksapi_KSApiCommon, papiSet));
//		print_log(pupload, ov_logfile_info, "%i %s", papiSet->v_status, papiSet->v_path);
//		ov_logfile_info("%i: %s",papiSet->v_status, papiSet->v_path);
//	}
//
//	return res;
//
//
////	OV_ELEMENT elparent;
////	elparent.elemtype = OV_ET_OBJECT;
////	elparent.pobj = pobj;
////
////	OV_VTBLPTR_ov_object pVtblObj = NULL;
////	Ov_GetVTablePtr(ov_object, pVtblObj, pobj);
////	OV_ELEMENT elvar;
////	elvar.elemtype = OV_ET_NONE;
////
////	cJSON_ArrayForEach(jsvariable, jsvariables)
////	{
////		if (cJSON_GetArraySize(jsvariable) != VARIABLE_FORMAT_SIZE) {
////			ov_logfile_alert("%s error", cJSON_GetStringValue(jsvariable));
////			continue;
////		}
////		res = ov_element_searchpart(&elparent, &elvar, OV_ET_VARIABLE, jsvariable->string);
////		if(Ov_Fail(res)){
////			ov_logfile_error("variable set error");
////			return res;
////		}
////		OV_ANY value = get_value(jsvariable);
//////		pVtblObj->m_setvar
////		pVtblObj->m_setvar(pobj, &elvar, &value);
////	}
//}
/*
cJSON* jschildren = NULL;
OV_ELEMENT objElement;
objElement.elemtype = OV_ET_OBJECT;
objElement.pobj = Ov_PtrUpCast(ov_object, pobj);

OV_ELEMENT elchild;
elchild.elemtype = OV_ET_NONE;
int findChild = 0;
int only_parts = 0;
while (TRUE) {
	if (only_parts == 0) {
		ov_element_getnextchild(&objElement, &elchild);
		if (elchild.elemtype != OV_ET_OBJECT) {
			elchild.elemtype = OV_ET_NONE;
			only_parts = 1;
		}
	}
	if (only_parts == 1) {
		ov_element_getnextpart(&objElement, &elchild, OV_ET_OBJECT);
		if (elchild.elemtype != OV_ET_OBJECT) {
			return res;
		}
	}
	if (findChild == 0) {
		jschildren = cJSON_AddObjectToObject(jsobj, "children");
		findChild++;
	}
	ov_logfile_info("%s", ov_element_getidentifier(&elchild));

	cJSON* jschild = cJSON_AddObjectToObject(jschildren,
			ov_element_getidentifier(&elchild));
	res = crawl_tree(pinst, Ov_StaticPtrCast(ov_domain, elchild.pobj),
			jschild);
	if (Ov_Fail(res))
	return res;*/

}

/*OV_RESULT CtreeUplaod_init(CtreeUpload* pupload, OV_INSTPTR_CTree_Upload pinst) {
 OV_RESULT res = OV_ERR_OK;
 pupload->root_path = "";
 ov_string_append(&pupload->root_path, pinst->v_root);
 pupload->proot = ov_path_getobjectpointer(pinst->v_root, VERSION_FOR_CTREE);

 //TODO: find out from object
 //	ov_string_setvalue(&pupload->pinst->p_apiSet.v_serverHost, "localhost");
 //	ov_string_setvalue(&pupload->pinst->p_apiSet.v_serverName, "MANAGER");

 return res;
 }*/




//
//
//OV_RESULT get_links(cJSON* jslinks, const OV_ELEMENT* const pelobj,
//		OV_ELEM_TYPE mask) {
//	OV_RESULT res = OV_ERR_OK;
//
//	OV_ELEMENT elvar;
//	elvar.elemtype = OV_ET_NONE;
//
//	while (TRUE) {
//		//get links
//		res = ov_element_getnextpart(pelobj, &elvar, mask);
//		if (Ov_Fail(res)) {
//			ov_logfile_error("%s", ov_result_getresulttext(res));
//			return res;
//		}
//		if (elvar.elemtype == OV_ET_NONE)
//			break;
//
//		res = write_link(jslinks, &elvar, mask);
//		if (res == OV_ERR_OPTYPEDEFMISMATCH) {
//			res = OV_ERR_OK;
//		}
//	}
//	return res;
//}
//
//OV_RESULT write_link(cJSON* jslinks, OV_ELEMENT* pelassoc, OV_ELEM_TYPE mask) {
//	OV_RESULT res = OV_ERR_OK;
//
//	OV_INSTPTR_ov_association passoc = pelassoc->elemunion.passoc;
//
//	cJSON* jsparent = NULL;
//	cJSON* jschild = NULL;
//	cJSON* current = cJSON_CreateObject();
//
//	OV_INSTPTR_ov_object pparent = NULL;
//	OV_INSTPTR_ov_object pchild = NULL;
//
//	//of_association
//	ov_memstack_lock();
//	OV_STRING factory = ov_path_getcanonicalpath(
//			Ov_PtrUpCast(ov_object, passoc), VERSION_FOR_CTREE);
//	ov_memstack_unlock();
//	if (ov_string_match(factory, "*/ov/containment")
//			|| ov_string_match(factory, "*/ov/instantiation"))
//		return OV_ERR_OPTYPEDEFMISMATCH;
//
//	cJSON_AddStringToObject(current, "of_association", factory);
//	cJSON_AddItemToArray(jslinks, current);
//
//	jsparent = cJSON_AddArrayToObject(current, LINKPARENTSNAME);
////	cJSON_AddItemToArray(jsparent, cJSON_CreateString(passoc->v_parentrolename));
//
//	jschild = cJSON_AddArrayToObject(current, CHILDRENNAME);
////	cJSON_AddItemToArray(jschild, cJSON_CreateString(passoc->v_childrolename));
//
//	switch (passoc->v_assoctype) {
//	case OV_AT_ONE_TO_MANY:
////		switch (mask) {
////		case OV_ET_CHILDLINK:
//		pparent = Ov_Association_GetParent(passoc, pelassoc->pobj);
//		ov_memstack_lock();
//		factory = ov_path_getcanonicalpath(pparent,
//		VERSION_FOR_CTREE);
//		ov_memstack_unlock();
//
////			OV_STRING match=NULL;
////			ov_string_setvalue(&match, "");
////			ov_string_append(&match, "*");
//		//TODO:
////			if (ov_string_match(factory, "/TechUnits/*"))
////					break;
//		cJSON_AddItemToArray(jsparent, cJSON_CreateString(factory));
//
//		ov_memstack_lock();
//		factory = ov_path_getcanonicalpath(pelassoc->pobj,
//		VERSION_FOR_CTREE);
//		cJSON_AddItemToArray(jschild, cJSON_CreateString(factory));
//		ov_memstack_unlock();
//		break;
////		case OV_ET_PARENTLINK:
////			ov_memstack_lock();
////			factory = ov_path_getcanonicalpath(pelassoc->pobj,
////			VERSION_FOR_CTREE);
////			ov_memstack_unlock();
////			cJSON_AddItemToArray(jsparent, cJSON_CreateString(factory));
//
//		Ov_Association_ForEachChild(passoc, pparent, pchild)
//		{
//			ov_memstack_lock();
//			OV_STRING factory = ov_path_getcanonicalpath(pchild,
//			VERSION_FOR_CTREE);
//			cJSON_AddItemToArray(jschild, cJSON_CreateString(factory));
//			ov_memstack_unlock();
//		}
////		}
//		break;
//	case OV_AT_MANY_TO_MANY:
//		Ov_Error("many to many")
//		;
//		break;
//	case OV_AT_ONE_TO_ONE:
//		pparent = Ov_Association_GetParent(passoc, pelassoc->pobj);
//		ov_memstack_lock();
//		factory = ov_path_getcanonicalpath(pparent,
//		VERSION_FOR_CTREE);
//		ov_memstack_unlock();
//
//		cJSON_AddItemToArray(jsparent, cJSON_CreateString(factory));
//
//		ov_memstack_lock();
//		factory = ov_path_getcanonicalpath(pelassoc->pobj,
//		VERSION_FOR_CTREE);
//		cJSON_AddItemToArray(jschild, cJSON_CreateString(factory));
//		ov_memstack_unlock();
//		break;
//	}
//
//	return res;
//}

/*

 OV_RESULT crawl_links(OV_INSTPTR_CTree_Download pinst, const OV_ELEMENT* pelobj) {

 cJSON* jslinks = pinst->v_cache.jslinks;
 OV_RESULT res = OV_ERR_OK;

 //	get_links(jslinks, pelobj, OV_ET_PARENTLINK);
 get_links(jslinks, pelobj, OV_ET_CHILDLINK);

 OV_ELEMENT elchild;
 elchild.elemtype = OV_ET_NONE;

 ov_element_getnextchild(pelobj, &elchild);
 while (elchild.elemtype == OV_ET_OBJECT) {

 res = crawl_links(pinst, &elchild);
 if (Ov_Fail(res))
 return res;
 ov_element_getnextchild(pelobj, &elchild);
 }
 return res;
 }

 */



/*

 OV_RESULT link_objects(const cJSON* jslinks) {
 OV_RESULT res = OV_ERR_OK;
 cJSON* jschild = NULL;
 cJSON* current = NULL;
 OV_INSTPTR_ov_association passoc = NULL;
 OV_INSTPTR_ov_object pparent = NULL;
 OV_INSTPTR_ov_object pchild = NULL;

 if (jslinks == NULL)
 return OV_ERR_BADPARAM;

 cJSON_ArrayForEach(jschild, jslinks)
 {
 current = cJSON_GetObjectItem(jschild, "of_association");
 passoc =
 Ov_StaticPtrCast(ov_association,
 ov_path_getobjectpointer(cJSON_GetStringValue(current), VERSION_FOR_CTREE));

 cJSON* jsasparent = NULL;
 cJSON* jsasparents = cJSON_GetObjectItem(jschild, "parents");

 cJSON_ArrayForEach(jsasparent, jsasparents)
 {
 pparent = ov_path_getobjectpointer(cJSON_GetStringValue(jsasparent),
 VERSION_FOR_CTREE);
 if (pparent == NULL) {
 ov_logfile_error("%s does not exist",
 cJSON_GetStringValue(jsasparent));
 continue;
 //			return OV_ERR_BADPARAM;
 }

 cJSON* jsaschild = NULL;
 cJSON* jsaschildren = cJSON_GetObjectItem(jschild, LINKCHILDRENNAME);
 cJSON_ArrayForEach(jsaschild, jsaschildren)
 {
 OV_STRING path = cJSON_GetStringValue(jsaschild);
 pchild = ov_path_getobjectpointer(path, VERSION_FOR_CTREE);
 if (pchild == NULL) {
 ov_logfile_error("%s does not exist", path);
 continue;
 //			return OV_ERR_BADPARAM;
 }

 res = ov_association_link(passoc, pparent, pchild,
 OV_PMH_DEFAULT, NULL, OV_PMH_DEFAULT, NULL);
 if (Ov_OK(res))
 ov_logfile_info("%s linked with %s through %s",
 pparent->v_identifier, pchild->v_identifier,
 passoc->v_identifier);
 else {
 if (res == OV_ERR_ALREADYEXISTS) {
 ov_logfile_warning(
 "%s is already linked with %s through %s",
 pparent->v_identifier, pchild->v_identifier,
 passoc->v_identifier);
 res = OV_ERR_OK;
 } else {
 ov_logfile_error(
 "%s can not be linked with %s through %s",
 pparent->v_identifier, pchild->v_identifier,
 passoc->v_identifier);
 res = OV_ERR_OK;
 //!!!
 }
 }
 }
 }

 }
 return res;
 }
 */
