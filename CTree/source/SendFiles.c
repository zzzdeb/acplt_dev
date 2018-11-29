/******************************************************************************
 *
 *   FILE
 *   ----
 *   SendFiles.c
 *
 *   History
 *   -------
 *   2018-04-28   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_CTree
#define OV_COMPILE_LIBRARY_CTree
#endif

#include <stdlib.h>
#include <string.h>

#include "CTree.h"
#include "libov/ov_macros.h"
#include "libov/ov_ov.h"
#include "libov/ov_result.h"

#include "ksbase.h"
#include "ksbase_helper.h"

enum state {
  INITIAL,
  SENT_WAITING,
  DONE,
  INTERNAL_ERROR,
  EXTERNAL_ERROR,
  STATESLEN
};

OV_DLLFNCEXPORT OV_RESULT CTree_SendFiles_reset_set(
    OV_INSTPTR_CTree_SendFiles pobj, const OV_BOOL value) {
  OV_RESULT                    result = OV_ERR_OK;
  OV_INSTPTR_ksbase_ClientBase pClient =
      Ov_StaticPtrCast(ksbase_ClientBase, &pobj->p_ks);
  OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
  if(value && (!pobj->v_reset)) {
    pobj->v_status = INITIAL;
    pobj->v_result = OV_ERR_OK;

    Ov_SetDynamicVectorLength(&pobj->v_filesToSend, 0, STRING);
    Ov_SetDynamicVectorLength(&pobj->v_whereToSend, 0, STRING);
    ov_string_setvalue(&pobj->v_ErrorMsg, NULL);
    pobj->v_trigger = 0;

    if(pClient) {
      Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);
      if(pVtblClient) {
        result = pVtblClient->m_reset(pClient);
      }
    }
  }
  pobj->v_reset = value;
  return result;
}

/* create path without variable. $ACPLT_HOME/dev -> /home/zzz/....../acplt/dev.
 * works for now only for first*/
OV_STRING pathWithoutVariable(OV_STRING path) {
  OV_STRING pathWV = NULL;
  OV_STRING tmpString = NULL;
  OV_STRING currentChar = NULL;
  ov_string_setvalue(&pathWV, path);
  if(*path != '$') {
    return pathWV;
  }
  ov_string_setvalue(&tmpString, path);
  currentChar = strchr(tmpString, '/');
  *currentChar = 0;
  ov_string_setvalue(&pathWV, getenv(tmpString + 1));
  *currentChar = '/';
  if(pathWV == NULL) {
    ov_string_setvalue(&tmpString, NULL);
    return pathWV;
  }
  ov_string_append(&pathWV, currentChar);
  ov_string_setvalue(&tmpString, NULL);
  return pathWV;
}

OV_RESULT filesToBytes(OV_BYTE_VEC* bvec, OV_STRING path) {
  OV_RESULT result = OV_ERR_OK;
  FILE*     lib = NULL;
  ov_memstack_lock();
  OV_UINT   number_of_bytes = 0;
  OV_STRING varFreePath = pathWithoutVariable(path);

  lib = fopen(varFreePath, "rb");
  ov_string_setvalue(&varFreePath, NULL);
  if(!lib) {
    bvec->value = NULL;
    bvec->veclen = 0;
    return OV_ERR_BADPATH;
  }

  fseek(lib, 0L, SEEK_END);
  number_of_bytes = ftell(lib);
  rewind(lib);

  bvec->value = (OV_BYTE*)ov_memstack_alloc(number_of_bytes * sizeof(OV_BYTE));
  bvec->veclen = fread(bvec->value, sizeof(OV_BYTE), number_of_bytes, lib);
  if(bvec->veclen == -1) {
    bvec->value = NULL;
    bvec->veclen = 0;
    return OV_ERR_GENERIC;
  }

  if(lib) fclose(lib);
  ov_memstack_unlock();
  return result;
}
/*/a/b/c/d.so to d.so*/
OV_STRING name_from_path(const OV_STRING path) {
  if(!path) return NULL;
  OV_STRING ret = NULL;
  char*     found = strrchr(path, '/');
  if(found)
    ov_string_setvalue(&ret, found + 1);
  else
    ov_string_setvalue(&ret, path);
  return ret;
}

/* call back */
OV_DLLFNCEXPORT void sendFiles_callback(const OV_INSTPTR_ov_domain this,
                                        const OV_INSTPTR_ov_domain that) {
  OV_INSTPTR_CTree_SendFiles   pinst = Ov_StaticPtrCast(CTree_SendFiles, this);
  OV_INSTPTR_ksbase_ClientBase pClient =
      Ov_StaticPtrCast(ksbase_ClientBase, that);
  OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
  OV_UINT                      itemsLength;
  OV_RESULT*                   itemsResults = NULL;
  OV_RESULT                    result;

  if(!this || !that) {
    ov_logfile_error("callback issued with NULL pointers. aborting.");
    return;
  }

  Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);

  if(!pVtblClient) {
    ov_logfile_error(
        "%s callback: could not determine Vtable of Client %s. aborting",
        this->v_identifier, that->v_identifier);
    pinst->v_status = INTERNAL_ERROR;
    pinst->v_result = OV_ERR_BADOBJTYPE;
    return;
  }
  ov_memstack_lock();

  result = pVtblClient->m_processSetVar(pClient, NULL,
                                        (OV_RESULT*)&(pinst->v_result),
                                        &itemsLength, &itemsResults);
  if(Ov_Fail(result)) {
    pinst->v_status = INTERNAL_ERROR;
    pinst->v_result = result;
    ov_memstack_unlock();
    return;
  }
  pinst->v_status = DONE;
  pinst->v_result = OV_ERR_OK;
  ov_logfile_info("SendFiles done.");
  ov_memstack_unlock();

  if(Ov_Fail(pinst->v_result)) {
    pinst->v_status = EXTERNAL_ERROR;
    ov_memstack_unlock();
    return;
  }
  // debug
  for(OV_UINT i = 0; i < itemsLength; i++) {
    ov_logfile_info("%u: %s", itemsResults[i],
                    ov_result_getresulttext(itemsResults[i]));
  }

  if(pinst->v_postCallback.callbackFunction)
    (*pinst->v_postCallback.callbackFunction)(
        pinst->v_postCallback.instanceCalled,
        Ov_StaticPtrCast(ov_domain, pinst));
  return;
}

OV_RESULT CTree_SendFiles_execute(OV_INSTPTR_CTree_SendFiles pinst) {
  OV_RESULT result = OV_ERR_OK;

  OV_STRING_VEC fileNames = {0, NULL};
  OV_UINT_VEC   filePositions = {0, NULL};
  OV_BYTE_VEC   unitedBfiles = {0, NULL};
  OV_BYTE_VEC*  pbfiles = {0, NULL};

  OV_INSTPTR_ksbase_ClientBase pClient =
      Ov_StaticPtrCast(ksbase_ClientBase, &pinst->p_ks);
  result = CTree_helper_setKSParam(pClient, pinst->v_targetKS);

  OV_VTBLPTR_ksbase_ClientBase pVtblClient = NULL;
  Ov_GetVTablePtr(ksbase_ClientBase, pVtblClient, pClient);
  if(!pVtblClient) {
    pinst->v_status = INTERNAL_ERROR;
    return OV_ERR_GENERIC;
  }

  ov_memstack_lock();
  /*file names*/
  //	fileNames.veclen = pinst->v_filesToSend.veclen;
  //	for (OV_UINT i = 0; i < fileNames.veclen; ++i)
  //	{
  //		fileNames.value[i] =
  // name_from_path(pinst->v_filesToSend.value[i]);
  //	}
  /*file bytes*/
  pbfiles = Ov_HeapMalloc(pinst->v_filesToSend.veclen * sizeof(OV_BYTE_VEC));

  for(OV_UINT i = 0; i < pinst->v_filesToSend.veclen; i++) {
    result = filesToBytes(&pbfiles[i], pinst->v_filesToSend.value[i]);
    switch(result) {
      case OV_ERR_OK: break;
      case OV_ERR_BADPATH:
        ov_logfile_warning("file: %s doesnt exist",
                           pinst->v_filesToSend.value[i]);
        break;
      default: return result;
    }
  }
  /* byte positions*/
  OV_UINT  byteSize = 0;
  OV_BYTE* currentByte = NULL;

  filePositions.veclen = pinst->v_filesToSend.veclen;
  filePositions.value =
      Ov_HeapMalloc(pinst->v_filesToSend.veclen * sizeof(OV_UINT));

  for(OV_UINT i = 0; i < pinst->v_filesToSend.veclen; i++) {
    byteSize += pbfiles[i].veclen;
    filePositions.value[i] = byteSize;
    ov_logfile_info("bytes %u: file", pbfiles[i].veclen);
  }
  ov_logfile_info("bytes to send %u", byteSize);

  unitedBfiles.veclen = byteSize;
  unitedBfiles.value = Ov_HeapMalloc(byteSize);
  currentByte = unitedBfiles.value;
  for(OV_UINT i = 0; i < pinst->v_filesToSend.veclen; i++) {
    memcpy(currentByte, pbfiles[i].value, pbfiles[i].veclen);
    ov_logfile_info("coping %u bytes at %u", pbfiles[i].veclen, currentByte);
    currentByte += pbfiles[i].veclen;
  }
  /*
   * 2. send files
   */
  enum { FILENAMES, FILEPOSITIONS, BFILES, TRIGGER, SENDFORMATLEN };

  OV_SETVAR_ITEM libSend[SENDFORMATLEN] = {
      [FILENAMES] = {"/data/CTree/WriteFiles.filesToWrite", {0}},
      [FILEPOSITIONS] = {"/data/CTree/WriteFiles.filesPositions", {0}},
      [BFILES] = {"/data/CTree/WriteFiles.byteFiles", {0}},
      [TRIGGER] = {"/data/CTree/WriteFiles.trigger", {0}}};
  //		OV_SETVAR_ITEM libSend[1] = {[LIBNAMES] =
  //{"/TechUnits/WriteFile.fileToWrite", {0}}};

  libSend[FILENAMES].var_current_props.value.vartype = OV_VT_STRING_VEC;
  if(pinst->v_whereToSend.veclen == pinst->v_filesToSend.veclen)
    libSend[FILENAMES].var_current_props.value.valueunion.val_string_vec =
        pinst->v_whereToSend;
  else
    libSend[FILENAMES].var_current_props.value.valueunion.val_string_vec =
        pinst->v_filesToSend;
  libSend[FILEPOSITIONS].var_current_props.value.vartype = OV_VT_UINT_VEC;
  libSend[FILEPOSITIONS].var_current_props.value.valueunion.val_uint_vec =
      filePositions;
  libSend[BFILES].var_current_props.value.vartype = OV_VT_BYTE_VEC;
  libSend[BFILES].var_current_props.value.valueunion.val_byte_vec =
      unitedBfiles;
  libSend[TRIGGER].var_current_props.value.vartype = OV_VT_INT;
  libSend[TRIGGER].var_current_props.value.valueunion.val_bool = 1;

  /* setting var */
  result = pVtblClient->m_requestSetVar(pClient, NULL, SENDFORMATLEN, libSend,
                                        (OV_INSTPTR_ov_domain)pinst,
                                        &sendFiles_callback);

  if(!(pClient->v_state & KSBASE_CLST_ERROR))
    pinst->v_status = SENT_WAITING;
  else {
    pinst->v_status = INTERNAL_ERROR;
    pinst->v_result = OV_ERR_GENERIC;
    /* cleaning */
  }

  /* cleaning */
  for(OV_UINT i = 0; i < fileNames.veclen; ++i) {
    ov_string_setvalue(&fileNames.value[i], NULL);
  }

  ov_memstack_unlock();
  Ov_HeapFree(pbfiles);
  Ov_HeapFree(unitedBfiles.value);

  return result;
}

OV_RESULT CTree_SendFiles_execute_withCallback(
    OV_INSTPTR_CTree_SendFiles pinst, OV_INSTPTR_ov_domain that,
    void (*callback)(OV_INSTPTR_ov_domain, OV_INSTPTR_ov_domain)) {
  /* outer object to call after finished*/
  pinst->v_postCallback.callbackFunction = callback;
  pinst->v_postCallback.instanceCalled = that;
  return CTree_SendFiles_execute(pinst);
}

OV_DLLFNCEXPORT void CTree_SendFiles_typemethod(OV_INSTPTR_fb_functionblock pfb,
                                                OV_TIME* pltc) {
  /*
   *   local variables
   */

  OV_INSTPTR_CTree_SendFiles pinst = Ov_StaticPtrCast(CTree_SendFiles, pfb);
  OV_RESULT                  result = OV_ERR_OK;
  ov_string_setvalue(&pinst->v_ErrorMsg, NULL);
  pinst->v_result = OV_ERR_OK;

  result = CTree_SendFiles_execute(pinst);
  switch(result) {
    case OV_ERR_OK:
      pinst->v_result = -1;
      ov_logfile_info("SendFiles done, waiting for response.");
      break;
    case OV_ERR_BADPARAM:
      pinst->v_result = result;
      ov_logfile_error("SendFiles failed.");
      break;
    default:
      pinst->v_result = OV_ERR_GENERIC;
      ov_logfile_error("SendFiles failed. : %s",
                       ov_result_getresulttext(result));
  }
  return;
}
