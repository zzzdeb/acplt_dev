#include "libov/ov_memstack.h"
#include "libov/ov_ov.h"
#include "libov/ov_macros.h"
#include "acplt_simpleMsgHandling.h"

#include <string.h>
#include <stdlib.h>

static char c_DATA_start[] = "![CDATA[";
static char c_DATA_end[] = "]]>";

/*	If no element is found pStart is NULL, return value is OV_ERR_OK	*/
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_findElementBegin(char const* xml,
		const OV_STRING elemName, OV_STRING* pStart) {
	OV_UINT nameLength = 0;
	OV_UINT cDataCount = 0;

	if(!xml || !elemName || !pStart) return OV_ERR_BADPARAM;

	nameLength = strlen(elemName);

	*pStart = strchr(xml, '<');
	if(!(*pStart)) return OV_ERR_OK;

	while (*pStart) {
		/*	next char	*/
		(*pStart)++;
		/*	check for !CDATA and if found jump over it; care for nested CDATA-elements	*/
		if(!strncmp(*pStart, c_DATA_start, sizeof(c_DATA_start) - 1)) {
			cDataCount = 1;
			(*pStart) += sizeof(c_DATA_start) + 1 - 1;
			while (cDataCount >= 1) {
				if(!(**pStart)) return OV_ERR_BADVALUE;
				if(!strncmp(*pStart, c_DATA_end, sizeof(c_DATA_end) - 1)) {
					cDataCount--;
					(*pStart) += sizeof(c_DATA_end) - 1;
					continue;
				} else if(**pStart == '<') {
					(*pStart)++;
					if(!strncmp(*pStart, c_DATA_start, sizeof(c_DATA_start) - 1)) {
						cDataCount++;
						(*pStart) += sizeof(c_DATA_start) - 1;
						continue;
					}
				}
				(*pStart)++;
			}
		} else {/*	compare tag name	*/
			if(!strncmp(*pStart, elemName, nameLength)) { /*	found name but it may only be part of the element name in the xml stream	*/
				if((*pStart)[nameLength] == '>' || (*pStart)[nameLength] == ' '
						|| (*pStart)[nameLength] == '\t' || (*pStart)[nameLength] == '\r'
						|| (*pStart)[nameLength] == '\n') /*	element Name complete	*/
				break;
			}
			*pStart = strchr(*pStart, '<');
			if(!(*pStart)) return OV_ERR_OK;
		}
	}

	if(!(*pStart)) return OV_ERR_OK;

	(*pStart)--;
	return OV_ERR_OK;
}

static char esc_qout[] = "quot;"; /*	"	*/
static char esc_apos[] = "apos;"; /*	'	*/
static char esc_lt[] = "lt;"; /*	<	*/
static char esc_gt[] = "gt;"; /*	>	*/
static char esc_amp[] = "amp;"; /*	&	*/

OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_extractString(
		char const* xmlString, OV_UINT strLength, OV_STRING* targetString) {
	unsigned long i;
	unsigned long cDataCount = 0;

	if(!xmlString || !(*xmlString)) return OV_ERR_BADPARAM;
	if(!strLength) { /*	no length given	*/
		/*	check delimiter	*/
		if(*xmlString == '\'') { /*	single quote delimiter	*/
			xmlString++; /*	move behind first delimiter	*/
			for (i = 0; xmlString[i] && xmlString[i] != '\''; i++)
				;
			strLength = i;
		} else if(*xmlString == '\"') {/*	double quote delimiter	*/
			xmlString++; /*	move behind first delimiter	*/
			for (i = 0; xmlString[i] && xmlString[i] != '\"'; i++)
				;
			strLength = i;
		} else {
			return OV_ERR_BADVALUE;
		}
	}

	/*	length known (now), we are inside the delimiters	*/

	*targetString = ov_memstack_alloc(strLength + 1);
	if(!(*targetString)) return OV_ERR_HEAPOUTOFMEMORY;

	for (i = 0; i < strLength; i++) {
		switch (xmlString[i]) {
			case '&':
				/*	unescape String	*/
				/*	"	*/
				if(!strncmp(&(xmlString[i + 1]), esc_qout, sizeof(esc_qout) - 1)) {
					(*targetString)[i] = '"';
					xmlString += sizeof(esc_qout) - 1;
					strLength -= sizeof(esc_qout) - 1;
				}
				/*	'	*/
				else if(!strncmp(&(xmlString[i + 1]), esc_apos, sizeof(esc_apos) - 1)) {
					(*targetString)[i] = '\'';
					xmlString += sizeof(esc_apos) - 1;
					strLength -= sizeof(esc_apos) - 1;
				}
				/*	<	*/
				else if(!strncmp(&(xmlString[i + 1]), esc_lt, sizeof(esc_lt) - 1)) {
					(*targetString)[i] = '<';
					xmlString += sizeof(esc_lt) - 1;
					strLength -= sizeof(esc_lt) - 1;
				}
				/*	>	*/
				else if(!strncmp(&(xmlString[i + 1]), esc_gt, sizeof(esc_gt) - 1)) {
					(*targetString)[i] = '>';
					xmlString += sizeof(esc_gt) - 1;
					strLength -= sizeof(esc_gt) - 1;
				}
				/*	&	*/
				else if(!strncmp(&(xmlString[i + 1]), esc_amp, sizeof(esc_amp) - 1)) {
					(*targetString)[i] = '&';
					xmlString += sizeof(esc_amp) - 1;
					strLength -= sizeof(esc_amp) - 1;
				} else {
					return OV_ERR_BADVALUE;
				}
				break;

			case '<':
				if(!strncmp(&(xmlString[i + 1]), c_DATA_start,
					sizeof(c_DATA_start) - 1)) { /*	CDATA element found; copy content only but care for nested CDATA elements	*/
					cDataCount = 1;
					strLength -= sizeof(c_DATA_start) - 1;
					for (xmlString += sizeof(c_DATA_start);
							*xmlString && (cDataCount >= 1); i++) {
						(*targetString)[i] = xmlString[i];
						if(xmlString[i] == '<') {
							if(!strncmp(&(xmlString[i + 1]), c_DATA_start,
								sizeof(c_DATA_start) - 1)) cDataCount++;
						} else if(!strncmp(&(xmlString[i]), c_DATA_end,
							sizeof(c_DATA_end) - 1)) cDataCount--;
					}

					xmlString += sizeof(c_DATA_end) - 2; //one more from above
					strLength -= sizeof(c_DATA_end) - 1;
				} else
					return OV_ERR_BADVALUE;
				break;

			case '>':
				return OV_ERR_BADVALUE;

			default:
				(*targetString)[i] = xmlString[i];
				break;
		}
	}

	(*targetString)[i] = '\0';
	return OV_ERR_OK;

}

/*
 * returns OV_ERR_BANAME if element is not found
 * returns OV_ERR_BADVALUE if element is not valid
 */

OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_getElementData(char const* xml,
		const OV_STRING elemName, OV_STRING* pData) {
	OV_UINT dataLength;
	OV_RESULT result;
	OV_STRING beginElement = NULL;
	char const* tempPtr = NULL;
	OV_UINT cDataCount = 0;

	/*	find the begin of the element of concern; this function will also catch NULL-pointers for us	*/
	result = acplt_simpleMsg_xml_findElementBegin(xml, elemName, &beginElement);
	if(Ov_Fail(result)) return result;

	if(!beginElement) return OV_ERR_BADNAME;

	/*	jump to end of tag	*/
	tempPtr = strchr(beginElement, '>');
	if(!tempPtr) return OV_ERR_BADVALUE;

	if(*(tempPtr - 1) == '/') { /*	no Data	*/
		*pData = NULL;
		return OV_ERR_OK;
	}

	tempPtr++;
	/*	now it points to the begin of the data	*/
	for (dataLength = 0; TRUE; dataLength++) {
		if(!tempPtr[dataLength]) return OV_ERR_BADVALUE; /*	file ends before end of element --> invalid	*/

		if(tempPtr[dataLength] == '<') { /*	jump over CDATA	*/
			if(!strncmp(&(tempPtr[dataLength + 1]), c_DATA_start,
				sizeof(c_DATA_start) - 1)) { /*	CDATA element found; count content but care for nested CDATA-elements	*/
				cDataCount++;
				dataLength += sizeof(c_DATA_start) + 1 - 1;
				while (cDataCount >= 1) {
					if(!tempPtr[dataLength]) return OV_ERR_BADVALUE;

					if(!strncmp(&(tempPtr[dataLength]), c_DATA_end,
						sizeof(c_DATA_end) - 1)) {
						cDataCount--;
						dataLength += sizeof(c_DATA_end) - 1;
						continue;
					} else if(tempPtr[dataLength] == '<') {
						if(!strncmp(&(tempPtr[dataLength + 1]), c_DATA_start,
							sizeof(c_DATA_start) - 1)) {
							cDataCount++;
							dataLength += sizeof(c_DATA_start) + 1 - 1;
							continue;
						}
					}
					dataLength++;
				}
			} else
				break;
		}

	}
	/*	extract and unescape data	*/
	return acplt_simpleMsg_xml_extractString(tempPtr, dataLength, pData);
}

/*
 * returns OV_ERR_BANAME if element is not found
 * returns OV_ERR_BADVALUE if element is not valid
 * returns OV_ERR_OK if attribute is not found, pData is NULL then
 */
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_xml_getAttributeData(char const* xml,
		const OV_STRING elemName, const OV_STRING attributeName, OV_STRING* pData) {
	OV_UINT dataLength;
	char const* beginElement = NULL;
	char const* endPtr = NULL;
	char const* tempptr = NULL;
	OV_UINT i;
	OV_BOOL found = FALSE;
	char const* valueEnd = NULL;
	OV_RESULT result;

	if(!attributeName) return OV_ERR_BADPARAM;
	/*	find the begin of the element of concern; this function will also catch NULL-pointers for us	*/
	result = acplt_simpleMsg_xml_findElementBegin(xml, elemName,
		(OV_STRING*) &beginElement);
	if(Ov_Fail(result)) return result;
	if(!beginElement) return OV_ERR_BADNAME;

	/*	find end of tag	*/
	endPtr = strchr(beginElement, '>');
	if(!endPtr) return OV_ERR_BADVALUE;

	/*	find attribute name	*/
	tempptr = beginElement;
	for (; tempptr < endPtr; tempptr++) {
		if(*tempptr == *attributeName) {
			for (i = 0; (&(tempptr[i]) < endPtr) && (attributeName[i]); i++) {
				if(tempptr[i] == attributeName[i])
					found = TRUE;
				else {
					found = FALSE;
					break;
				}
			}
			if(found) break;
		}
	}

	if(!found) {
		pData = NULL;
		return OV_ERR_OK;
	} else {
		if(tempptr[i] == '=') {
			/*	set to begin of attribute data	*/
			beginElement = &(tempptr[i + 1]);
			if(*beginElement == '"' || *beginElement == '\'') {
				return acplt_simpleMsg_xml_extractString(beginElement, 0, pData);
			} else {
				valueEnd = strchr(beginElement, ' ');
				if(valueEnd)
					dataLength = valueEnd - beginElement;
				else
					dataLength = endPtr - beginElement;
				return acplt_simpleMsg_xml_extractString(beginElement, dataLength,
					pData);
			}
		} else {
			pData = NULL;
			return OV_ERR_OK;
		}
	}
}

/*
 * fills header structure with values
 * returns OV_ERR_BADNAME if required elements are not found
 *
 */
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_parseMessageHeader(char const* xml,
		ACPLT_MSGHEADER* header) {

	OV_STRING tempMsgText = NULL;
	OV_INT HeaderLength;
	OV_STRING tempPtr = NULL;
	OV_STRING endTemp = NULL;
	OV_RESULT result;

	if(!xml || !(*xml)) return OV_ERR_BADPARAM;

	if(Ov_Fail(acplt_simpleMsg_xml_findElementBegin(xml, msgHdr, &tempPtr))
			|| !tempPtr) {
		return OV_ERR_BADVALUE;
	}

	if(Ov_Fail(acplt_simpleMsg_xml_findElementBegin(tempPtr, "/hdr", &endTemp))
			|| !endTemp) {
		return OV_ERR_BADVALUE;
	}

	HeaderLength = (endTemp - tempPtr);
	if(HeaderLength <= 0) return OV_ERR_BADVALUE;

	tempMsgText = ov_memstack_alloc(HeaderLength + 1);
	if(!tempMsgText) return OV_ERR_HEAPOUTOFMEMORY;
	memcpy(tempMsgText, tempPtr, HeaderLength);
	tempMsgText[HeaderLength] = '\0';

//	todo tmp comment
	OV_STRING tmpStrBeg = NULL;
	// OV_STRING tmpStrBeg1 = NULL;
	OV_STRING tmpStrEnd = NULL;
	OV_STRING tmpStrTag = NULL;
	OV_STRING_VEC tmpStrVec = { 0 };
	result = acplt_simpleMsg_xml_findElementBegin(tempPtr, msgSysAdrPath,
		&tmpStrBeg);
	if(Ov_Fail(result))
		return result;
	else if(!tmpStrBeg) return OV_ERR_BADVALUE;
	ov_string_print(&tmpStrTag, "/%s", msgSysAdrPath);
	result = acplt_simpleMsg_xml_findElementBegin(tempPtr, tmpStrTag,
		&tmpStrEnd);
	ov_string_setvalue(&tmpStrTag, NULL);
	if(Ov_Fail(result))
		return result;
	else if(!tmpStrEnd) return OV_ERR_BADVALUE;

	OV_UINT pathLen = 0;
	OV_UINT currentLen = 5;
	Ov_SetDynamicVectorLength(&tmpStrVec, currentLen, STRING);
	acplt_simpleMsg_xml_findElementBegin(tmpStrBeg, msgPathVal,
		&tmpStrVec.value[0]);
	if(tmpStrVec.value[pathLen] && tmpStrVec.value[pathLen] < tmpStrEnd) {
		while (1) {
			acplt_simpleMsg_xml_findElementBegin(
				tmpStrVec.value[pathLen] + 1,
				msgPathVal, &tmpStrVec.value[pathLen + 1]);
			pathLen++;
			if(!tmpStrVec.value[pathLen] || tmpStrVec.value[pathLen] > tmpStrEnd)
				break;
			if(pathLen == currentLen - 1) {
				currentLen *= 2;
				Ov_SetDynamicVectorLength(&tmpStrVec, currentLen, STRING);
			}
		}
	}
	Ov_SetDynamicVectorLength(&header->sysAdrPath, pathLen, STRING);
	for (OV_UINT i = 0; i < pathLen; i++) {
		acplt_simpleMsg_xml_getElementData(tmpStrVec.value[i], msgPathVal,
			&header->sysAdrPath.value[i]);
	}

	/* loc vals */
	result = acplt_simpleMsg_xml_findElementBegin(tempMsgText, msgLocAdrPath,
		&tmpStrBeg);
	if(Ov_Fail(result))
		return result;
	else if(!tmpStrBeg) return OV_ERR_BADVALUE;
	ov_string_print(&tmpStrTag, "/%s", msgLocAdrPath);
	result = acplt_simpleMsg_xml_findElementBegin(tempMsgText, tmpStrTag,
		&tmpStrEnd);
	ov_string_setvalue(&tmpStrTag, NULL);
	if(Ov_Fail(result))
		return result;
	else if(!tmpStrEnd) return OV_ERR_BADVALUE;

	pathLen = 0;
	acplt_simpleMsg_xml_findElementBegin(tmpStrBeg, msgPathVal,
		&tmpStrVec.value[0]);
	if(tmpStrVec.value[pathLen] && tmpStrVec.value[pathLen] < tmpStrEnd) {
		while (1) {
			acplt_simpleMsg_xml_findElementBegin(
				tmpStrVec.value[pathLen] + 1,
				msgPathVal, &tmpStrVec.value[pathLen + 1]);
			pathLen++;
			if(!tmpStrVec.value[pathLen] || tmpStrVec.value[pathLen] > tmpStrEnd)
				break;
			if(pathLen == currentLen - 1) {
				currentLen *= 2;
				Ov_SetDynamicVectorLength(&tmpStrVec, currentLen, STRING);
			}
		}
	}
	Ov_SetDynamicVectorLength(&header->locAdrPath, pathLen, STRING);
	for (OV_UINT i = 0; i < pathLen; i++) {
		acplt_simpleMsg_xml_getElementData(tmpStrVec.value[i], msgPathVal,
			&header->locAdrPath.value[i]);
	}
	Ov_SetDynamicVectorLength(&tmpStrVec, 0,
		STRING);

	result = acplt_simpleMsg_xml_getElementData(tempPtr, msgCurInd,
		&(tmpStrBeg));
	if(Ov_Fail(result))
		return result;
	header->currentInd = atoi(tmpStrBeg);

	result = acplt_simpleMsg_xml_getElementData(tempPtr, msgId, &(header->msgId));
	if(Ov_Fail(result))
		return result;
	else if(!header->msgId) return OV_ERR_BADVALUE;

	result = acplt_simpleMsg_xml_getElementData(tempPtr, msgRefId,
		&(header->refMsgId));
	if(Ov_Fail(result) && result != OV_ERR_BADNAME) {
		return result;
	}

	result = acplt_simpleMsg_xml_getElementData(tempPtr, msgAuth,
		&(header->auth));
	if(Ov_Fail(result) && result != OV_ERR_BADNAME) {
		return result;
	}

	return OV_ERR_OK;

}

/*
 * fills the strings and vectors with the respective values
 * the vectors are all of the same length, they contain the "val"-Elements data in the order of the respective elements
 * uses the memstack. don't forget to call lock/unlock around it
 * return OV_ERR_BADVALUE if no <bdy> or </bdy> tag is found
 * or if an expected sd-element is not found or an unexpected sd-element is found
 */
OV_DLLFNCEXPORT OV_RESULT acplt_simpleMsg_parseFlatBody(char const* xml,
		OV_STRING* svc, OV_STRING* op, OV_BOOL expectContainingObject,
		OV_STRING* containerName, OV_STRING_VEC* ids, OV_STRING_VEC* values,
		OV_STRING_VEC* units, OV_STRING_VEC* types) {
	OV_STRING tempPtr = NULL;
	OV_STRING endPtr = NULL;
	OV_STRING tempSd = NULL;
	OV_RESULT result;
	OV_STRING* tempIds = NULL;
	OV_STRING* tempValues = NULL;
	OV_STRING* tempUnits = NULL;
	OV_STRING* tempTypes = NULL;
	OV_UINT i;

	if(!xml || !(*xml)) return OV_ERR_BADPARAM;

	if(Ov_Fail(acplt_simpleMsg_xml_findElementBegin(xml, "bdy", &tempPtr))
			|| !tempPtr) {
		return OV_ERR_BADVALUE;
	}

	if(Ov_Fail(acplt_simpleMsg_xml_findElementBegin(tempPtr, "/bdy", &endPtr))
			|| !endPtr) {
		return OV_ERR_BADVALUE;
	}

	if(Ov_Fail(acplt_simpleMsg_xml_findElementBegin(tempPtr, "sd", &tempSd))
			|| !tempSd) {
		if(expectContainingObject) return OV_ERR_BADVALUE;
	} else {
		if(!expectContainingObject)
			return OV_ERR_BADVALUE;
		else {
			result = acplt_simpleMsg_xml_getAttributeData(tempSd, "sd", "id",
				containerName);
			if(Ov_Fail(result)) return result;
		}
	}

	if(Ov_Fail(acplt_simpleMsg_xml_findElementBegin(tempSd, "/sd", &tempSd))
			|| !tempSd) {
		if(expectContainingObject) return OV_ERR_BADVALUE;
	} else {
		if(!expectContainingObject) return OV_ERR_BADVALUE;
	}

	result = acplt_simpleMsg_xml_getAttributeData(tempPtr, "bdy", "Svc", svc);
	if(Ov_Fail(result)) return result;

	result = acplt_simpleMsg_xml_getAttributeData(tempPtr, "bdy", "Op", op);
	if(Ov_Fail(result)) return result;

	ids->veclen = 0;
	ids->value = NULL;
	values->veclen = 0;
	values->value = NULL;
	units->veclen = 0;
	units->value = NULL;
	types->veclen = 0;
	types->value = NULL;

	while (tempPtr < endPtr) {
		result = acplt_simpleMsg_xml_findElementBegin(tempPtr, "val", &tempPtr);
		if(Ov_Fail(result)) return result;
		if(!tempPtr) { /*	no more val-elements	*/
			return OV_ERR_OK;
		} else { /*	new val-Element found; extract data	*/
			/*	reserve new memory	*/
			tempIds = ov_memstack_alloc((ids->veclen + 1) * sizeof(OV_STRING));
			if(!tempIds) return OV_ERR_HEAPOUTOFMEMORY;
			tempValues = ov_memstack_alloc((values->veclen + 1) * sizeof(OV_STRING));
			if(!tempValues) return OV_ERR_HEAPOUTOFMEMORY;
			tempUnits = ov_memstack_alloc((units->veclen + 1) * sizeof(OV_STRING));
			if(!tempUnits) return OV_ERR_HEAPOUTOFMEMORY;
			tempTypes = ov_memstack_alloc((types->veclen + 1) * sizeof(OV_STRING));
			if(!tempTypes) return OV_ERR_HEAPOUTOFMEMORY;
			/*	copy old values to temp vectors	*/
			for (i = 0; i < ids->veclen; i++) {
				tempIds[i] = ids->value[i];
				tempValues[i] = values->value[i];
				tempUnits[i] = units->value[i];
				tempTypes[i] = types->value[i];
			}
			/*	add new elements data	*/
			result = acplt_simpleMsg_xml_getAttributeData(tempPtr, "val", "id",
				&(tempIds[ids->veclen]));
			if(Ov_Fail(result)) return result;
			result = acplt_simpleMsg_xml_getElementData(tempPtr, "val",
				&(tempValues[values->veclen]));
			if(Ov_Fail(result)) return result;
			result = acplt_simpleMsg_xml_getAttributeData(tempPtr, "val", "unit",
				&(tempUnits[units->veclen]));
			if(Ov_Fail(result)) return result;
			result = acplt_simpleMsg_xml_getAttributeData(tempPtr, "val", "type",
				&(tempTypes[types->veclen]));
			if(Ov_Fail(result)) return result;
			/*	update output	*/
			ids->veclen++;
			values->veclen++;
			units->veclen++;
			types->veclen++;
			ids->value = tempIds;
			values->value = tempValues;
			units->value = tempUnits;
			types->value = tempTypes;
		}
		/*	get next element in next iteration	*/
		tempPtr++;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void acplt_simpleMsg_initHeader(ACPLT_MSGHEADER* header) {
	if(header) {
		header->sysAdrPath.veclen = 0;
		header->sysAdrPath.value = NULL;
		header->locAdrPath.veclen = 0;
		header->locAdrPath.value = NULL;
		header->currentInd = 0;
		header->msgId = NULL;
		header->refMsgId = NULL;
		header->auth = NULL;
	}
	return;
}

