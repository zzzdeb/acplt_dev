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
