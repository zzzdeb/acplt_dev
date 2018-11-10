/*
 * acplt_simpleMsgCreator.c
 *
 *  Created on: 24.09.2013
 *      Author: lars
 */
#include "libov/ov_memstack.h"
#include "libov/ov_ov.h"
#include "libov/ov_string.h"
#include "libov/ov_logfile.h"
#include "acplt_simpleMsgHandling.h"

static OV_INT msgID = 0x48ad3548;


#define GEN_ADD_TAG(tagname)	*tempinside = '<';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1;	\
		*tempinside = '>';	\
		tempinside++

#define GEN_ADD_TAGBEGIN(tagname)	*tempinside = '<';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1

#define GEN_ADD_CLOSETAG(tagname)	*tempinside = '<';	\
		tempinside++;	\
		*tempinside = '/';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1;	\
		*tempinside = '>';	\
		tempinside++

#define GEN_ADD_EMPTYTAG(tagname)	*tempinside = '<';	\
		tempinside++;	\
		memcpy(tempinside,tagname, sizeof(tagname)-1);	\
		tempinside += sizeof(tagname)-1;	\
		*tempinside = '/';	\
		tempinside++;	\
		*tempinside = '>';	\
		tempinside++

/*
 * generates an xml-MessageHeader from the header-structure
 * if msdId is NULL in header structure an internal counter will be used
 */
OV_DLLFNCEXPORT OV_STRING acplt_simpleMsg_generateMsgHeader(
		ACPLT_MSGHEADER const * header) {
	OV_STRING temp = NULL;
	OV_STRING tempinside = NULL;
	OV_UINT length = 0;
	OV_UINT sysAdrPathLength = 0;
	OV_UINT locAdrPathLength = 0;
	OV_UINT pathValueLen = 0;
	OV_STRING curIndStr = NULL;
	OV_UINT msgIdLength = 0;
	OV_UINT refMsgIdLength = 0;
	OV_UINT authLength = 0;
	OV_UINT pathLen = 0;

	/*	hdr element (<>+</> + 2*$msgHdr)	*/
	length = 2 + 3 + 2 * sizeof(msgHdr);
	/*	System Address 	*/
	pathLen = header->sysAdrPath.veclen;
	sysAdrPathLength = 2 + 3 + 2 * sizeof(msgSysAdrPath);
	for (OV_UINT i = 0; i < pathLen; i++) {
		/*	<> + </> + 	2*$msgRcvSysAdr + $rvcSysAdrLength	*/
		pathValueLen = strlen(header->sysAdrPath.value[i]);
		sysAdrPathLength += 2 + 3 + 2 * (sizeof(msgPathVal) - 1) + pathValueLen; /*	no terminating \0	*/
	}
	length += sysAdrPathLength;

	/*	local Address 	*/
	pathLen = header->locAdrPath.veclen;
	locAdrPathLength = 2 + 3 + 2 * sizeof(msgLocAdrPath);
	for (OV_UINT i = 0; i < pathLen; i++) {
		/*	<> + </> + 	2*$msgRcvLocAdr + $rvcLocAdrLength	*/
		pathValueLen = strlen(header->locAdrPath.value[i]);
		locAdrPathLength += 2 + 3 + 2 * (sizeof(msgPathVal) - 1) + pathValueLen; /*	no terminating \0	*/
	}
	length += locAdrPathLength;

	/* curInd */
	ov_string_print(&curIndStr, "%u", header->currentInd);
	length += 2 + 3 + 2 * strlen(msgCurInd)
			+ ov_string_getlength(curIndStr);
	

	/*	message ID	*/
	if(header->msgId) { /*	<> + </> + 	2*$msgId + $msgIdLength	*/
		msgIdLength = strlen(header->msgId);
		length += 2 + 3 + 2 * (sizeof(msgId) - 1) + msgIdLength;
	} else { /*	<> + </> + 2*$msgId + 10 (max length od UINT)	*/
		msgIdLength = 10;
		length += 2 + 3 + 2 * (sizeof(msgId) - 1) + 10;
	}

	/*	reference message ID	*/
	if(header->refMsgId) { /*	<> + </> + 	2*$msgRefId + $refMsgIdLength	*/
		refMsgIdLength = strlen(header->refMsgId);
		length += 2 + 3 + 2 * (sizeof(msgRefId) - 1) + refMsgIdLength;
	}

	/*	auth	*/
	if(header->auth) { /*	<> + </> + 	2*$msgAuth + $authLength 	*/
		authLength = strlen((const char*) header->auth);
		length += 2 + 3 + 2 * (sizeof(msgAuth) - 1) + authLength;
	}

	temp = ov_memstack_alloc((length + 1) * sizeof(char));
	if(!temp) {
		ov_logfile_error("generateMsgHeader: heap out of memory");
		return NULL;
	}

	tempinside = temp;

	/*	begin header	*/
	GEN_ADD_TAG(msgHdr);

	/*	Sys Address	*/
	GEN_ADD_TAG(msgSysAdrPath);
	for (OV_UINT i = 0; i < pathLen; i++) {
		GEN_ADD_TAG(msgPathVal);

		pathValueLen = strlen(header->sysAdrPath.value[i]);
		memcpy(tempinside, header->sysAdrPath.value[i], pathValueLen);
		tempinside += pathValueLen;

		GEN_ADD_CLOSETAG(msgPathVal);
	}
	GEN_ADD_CLOSETAG(msgSysAdrPath);

	/*	loc Address	*/
	GEN_ADD_TAG(msgLocAdrPath);
	for (OV_UINT i = 0; i < pathLen; i++) {
		GEN_ADD_TAG(msgPathVal);

		pathValueLen = strlen(header->locAdrPath.value[i]);
		memcpy(tempinside, header->locAdrPath.value[i], pathValueLen);
		tempinside += pathValueLen;

		GEN_ADD_CLOSETAG(msgPathVal);
	}
	GEN_ADD_CLOSETAG(msgLocAdrPath);

	/* currentInd */
	GEN_ADD_TAG(msgCurInd);
	memcpy(tempinside, curIndStr, strlen(curIndStr));
	tempinside += strlen(curIndStr);
	GEN_ADD_CLOSETAG(msgCurInd);

	/*	message ID	*/
	GEN_ADD_TAG(msgId);
	if(header->msgId) {
		memcpy(tempinside, header->msgId, msgIdLength);
		tempinside += msgIdLength;
	} else {
		snprintf(tempinside, 11, "%i", msgID);
		tempinside += strlen(tempinside);
	}
	GEN_ADD_CLOSETAG(msgId);

	/*	reference message id	*/
	if(header->refMsgId) {
		GEN_ADD_TAG(msgRefId);
		memcpy(tempinside, header->refMsgId, refMsgIdLength);
		tempinside += refMsgIdLength;
		GEN_ADD_CLOSETAG(msgRefId);
	}

	/*	auth	*/
	if(header->auth) {
		GEN_ADD_TAG(msgAuth);
		memcpy(tempinside, header->auth, authLength);
		tempinside += authLength;
		GEN_ADD_CLOSETAG(msgAuth);
	}

	/*	close header	*/
	GEN_ADD_CLOSETAG(msgHdr);
	/*	terminating '\0'	*/
	*tempinside = '\0';
	return temp;
}

/*
 * generates a message with flat body form from strings service, operation and respective val-definitions
 * uses the memstack. don't forget to call lock/unlock around it.
 * createContainerObject = TRUE tells the function to put a containing sd-element around the val-elements. the name of this element
 * can be specified via containerName
 * ids, values, units and types have to be of same length. each set of entries [i] defines one <val> element. the entries may be empty
 */
OV_DLLFNCEXPORT OV_STRING acplt_simpleMsg_GenerateFlatBody(
		const OV_STRING service, const OV_STRING operation,
		OV_BOOL createContainingObject, const OV_STRING containerName,
		OV_STRING_VEC* ids, OV_STRING_VEC* values, OV_STRING_VEC* units,
		OV_STRING_VEC* types) {
	OV_STRING temp = NULL;
	OV_UINT svcLength = 0;
	OV_UINT opLength = 0;
	OV_UINT containerNameLength = 0;
	OV_UINT itemLength = 0;
	OV_STRING tempinside = NULL;
	OV_UINT length = 0;
	OV_UINT i;

	if(ids->veclen != values->veclen || ids->veclen != units->veclen
			|| ids->veclen != types->veclen) {
		ov_logfile_error("GenerateFlatBody: vectors are not of same length");
		return NULL;
	}

	/*	determine message length (memory!)	*/
	/*	bdy element (<> + </> + 2*msgBdy)	*/
	length = 2 + 3 + 2 * sizeof(msgBdy);

	/*	containing sd if wanted	*/
	if(createContainingObject) { /*	(<> + </> + 2*msgSd)	*/
		length += 2 + 3 + 2 * (sizeof(msgSd) - 1);
		if(containerName) {
			containerNameLength = strlen(containerName);
			length += 1 + (sizeof(msgValId) - 1) + containerNameLength + 1;
		}
	}
	/*	service 	*/
	if(service) { /*	' ' +$msgSvc+$svcLength+"	*/
		svcLength = strlen(service);
		length += 1 + (sizeof(msgSvc) - 1) + svcLength + 1; /*	no terminating \0	*/
	}

	/*	operation	*/
	if(operation) { /*	' '+$msgOp+$opLength+"	*/
		opLength = strlen(operation);
		length += 1 + (sizeof(msgOp) - 1) + opLength + 1; /*	no terminating \0	*/
	}

	for (i = 0; i < ids->veclen; i++) { /*	iterate over all values and reserve space	*/
		/*	generic val element <>+$msgVal	*/
		length += 2 + sizeof(msgVal) - 1;
		/*	value	*/
		if(values->value[i]) {
			length += strlen(values->value[i]);
		} else { /*	empty value /	*/
			length += 1;
		}
		/*	id ' '+$msgValId+idLength+"	*/
		if(ids->value[i]) {
			length += 1 + (sizeof(msgValId) - 1) + strlen(ids->value[i]) + 1;
		}
		/*	type ' '+$msgValType+typeLength+"	*/
		if(types->value[i]) {
			length += 1 + (sizeof(msgValType) - 1) + strlen(types->value[i]) + 1;
		}
		/*	unit ' '+$msgValUnit+unitLength+"	*/
		if(units->value[i]) {
			length += 1 + (sizeof(msgValUnit) - 1) + strlen(units->value[i]) + 1;
		}
	}

	/*	length determined; allocate memory and concatenate elements and attributes	*/
	temp = ov_memstack_alloc((length + 1) * sizeof(char));
	if(!temp) {
		ov_logfile_error("generateFlatBody: heap out of memory");
		return NULL;
	}
	tempinside = temp;

	/*	build body	*/
	GEN_ADD_TAGBEGIN(msgBdy);
	/*	service 	*/
	if(service) {
		*tempinside = ' ';
		tempinside++;
		memcpy(tempinside, msgSvc, sizeof(msgSvc) - 1);
		tempinside += sizeof(msgSvc) - 1;
		memcpy(tempinside, service, svcLength);
		tempinside += svcLength;
		*tempinside = '"';
		tempinside++;
	}

	/*	operation	*/
	if(operation) {
		*tempinside = ' ';
		tempinside++;
		memcpy(tempinside, msgOp, sizeof(msgOp) - 1);
		tempinside += sizeof(msgOp) - 1;
		memcpy(tempinside, operation, opLength);
		tempinside += opLength;
		*tempinside = '"';
		tempinside++;
	}

	*tempinside = '>';
	tempinside++;

	/*	containing sd-element if wanted	*/
	if(createContainingObject) {
		GEN_ADD_TAGBEGIN(msgSd);
		/*	id 	*/
		if(containerName) {
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValId, sizeof(msgValId) - 1);
			tempinside += sizeof(msgValId) - 1;
			memcpy(tempinside, containerName, containerNameLength);
			tempinside += containerNameLength;
			*tempinside = '"';
			tempinside++;
		}
		*tempinside = '>';
		tempinside++;
	}

	/*	values	*/
	for (i = 0; i < ids->veclen; i++) { /*	iterate over all values and concatenate	*/

		GEN_ADD_TAGBEGIN(msgVal);
		/*	id 	*/
		if(ids->value[i]) {
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValId, sizeof(msgValId) - 1);
			tempinside += sizeof(msgValId) - 1;
			itemLength = strlen(ids->value[i]);
			memcpy(tempinside, ids->value[i], itemLength);
			tempinside += itemLength;
			*tempinside = '"';
			tempinside++;
		}
		/*	type	*/
		if(types->value[i]) {
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValType, sizeof(msgValType) - 1);
			tempinside += sizeof(msgValType) - 1;
			itemLength = strlen(types->value[i]);
			memcpy(tempinside, types->value[i], itemLength);
			tempinside += itemLength;
			*tempinside = '"';
			tempinside++;
		}
		/*	unit	*/
		if(units->value[i]) {
			*tempinside = ' ';
			tempinside++;
			memcpy(tempinside, msgValUnit, sizeof(msgValUnit) - 1);
			tempinside += sizeof(msgValUnit) - 1;
			itemLength = strlen(units->value[i]);
			memcpy(tempinside, units->value[i], itemLength);
			tempinside += itemLength;
			*tempinside = '"';
			tempinside++;
		}
		/*	value	*/
		if(values->value[i]) {
			*tempinside = '>';
			tempinside++;
			itemLength = strlen(values->value[i]);
			memcpy(tempinside, values->value[i], itemLength);
			tempinside += itemLength;
			GEN_ADD_CLOSETAG(msgVal);
		} else {
			*tempinside = '/';
			tempinside++;
			*tempinside = '>';
			tempinside++;
		}
	}

	/*	containing sd-element if wanted	*/
	if(createContainingObject) {
		GEN_ADD_CLOSETAG(msgSd);
	}

	GEN_ADD_CLOSETAG(msgBdy);
	/*	terminating '\0'	*/
	*tempinside = '\0';
	return temp;
}
