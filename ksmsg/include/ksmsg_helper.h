#ifndef _KSMSG_HELPER_H_
#define _KSMSG_HELPER_H_

#include "ksmsg.h"
#include "libov/ov_macros.h"
#include "libov/ov_ov.h"

OV_DLLFNCEXPORT
OV_RESULT ksmsg_msgClient_path_deleteElement(OV_INSTPTR_ksmsg_msgClient pMsg,
                                             OV_UINT                    ind);
#define EXECUTERPATH "/data/exec"

#endif /* ifndef _KSMSG_HELPER_H_ */
