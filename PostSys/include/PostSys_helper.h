#ifndef OV_COMPILE_LIBRARY_PostSys_helper
#define OV_COMPILE_LIBRARY_PostSys_helper

#include "libov/ov_macros.h"
#include "libov/ov_ov.h"

#include "PostSys.h"

// POSTSYS: sending states of messages
#define MSGNEW 0
#define MSGREADYFORSENDING 1
#define MSGWAITING 2
#define MSGDONE 3
#define MSGRECEIVERERROR 4
#define MSGFATALERROR 5
// POSTSYS: receiving states of messages
#define MSGNEWARRIVED 10
#define MSGDONERECEIVE 11

#define COMPATH "/communication"
#define MSGDELIVERY "/communication/PostSys"
#define SENDINGINSTANCE "/communication/PostSys/sendingInstance"
#define REGISTEREDPATH "/communication/ServiceProvider/SystemServices"
#define KSMSG_IDENTIFIER "KSMSG"

OV_DLLFNCEXPORT OV_RESULT PostSys_createAnonymousObject(
    OV_INSTPTR_ov_class pClass, OV_INSTPTR_ov_domain pParent,
    OV_STRING identifier, OV_INSTPTR_ov_object* pObj);

OV_DLLFNCEXPORT OV_RESULT PostSys_createAnonymousMessage(
    OV_INSTPTR_ov_domain pParent, OV_STRING identifier,
    OV_INSTPTR_ov_object* pObj);

OV_DLLFNCEXPORT OV_RESULT PostSys_parseAndDeliverMsg(
    const OV_STRING value, OV_INSTPTR_PostSys_Message* createdMsg,
    OV_INSTPTR_ov_domain* msgCreatedIn);

/**
 * @brief duplicate
 *
 * @param pMsgCpy
 * @param pMsg
 *
 * @return result
 */
OV_DLLFNCEXPORT OV_RESULT PostSys_Message_copy(
    OV_INSTPTR_PostSys_Message pMsgCpy, const OV_INSTPTR_PostSys_Message pMsg);

/**
 * @brief pointer to inbox. if no inbox exists creates one.
 *
 * @param pdom parent object of inbox
 *
 * @return inbox
 */
OV_DLLFNCEXPORT OV_INSTPTR_PostSys_inbox
                PostSys_inbox_get(OV_INSTPTR_ov_domain pdom, OV_BOOL create);

/**
 * @brief sets only dst through ks. Other nodes in path stays same.
 *
 * @param pinst
 * @param dstKS
 * [//host[:MANAGERPort]/servername[:serverport]/]instancePath
 *
 * @return result
 */
OV_DLLFNCEXPORT OV_RESULT PostSys_msgCreator_dst_set(
    OV_INSTPTR_PostSys_msgCreator pinst, OV_STRING dstKS);

/**
 * @brief sets indexth element in path. returns BADPARAM if index >= pathLen
 *
 * @param pinst
 * @param dstKS
 * [//host[:MANAGERPort]/servername[:serverport]/]instancePath
 * @param index
 *
 * @return
 */
OV_DLLFNCEXPORT OV_RESULT PostSys_msgCreator_pathElem_set(
    OV_INSTPTR_PostSys_msgCreator pinst, OV_STRING dstKS, OV_UINT index);

#endif
