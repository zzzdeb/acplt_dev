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
