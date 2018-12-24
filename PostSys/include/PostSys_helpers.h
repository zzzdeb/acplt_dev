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

OV_DLLFNCEXPORT OV_RESULT PostSys_Message_copy(
    OV_INSTPTR_PostSys_Message pMsgCpy, const OV_INSTPTR_PostSys_Message pMsg);

#endif
