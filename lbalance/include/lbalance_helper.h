#include "libov/ov_macros.h"
#include "libov/ov_ov.h"

#define LBALANCE_SEP ";"

#define LB_NDB_PATH "/data/lbalance/BRAD.nbDB"
#define LB_REQREC_PATH "/data/lbalance/BRAD.reqReceiver"
#define LB_SENDINIT_PATH "/data/lbalance/BRAD.sendInitiator"

#define LB_INTERNALERROR 64
#define LB_EXTERNALERROR 128
#define LB_CASEERROR                                                           \
  case LB_INTERNALERROR:                                                       \
    break;                                                                     \
  case LB_EXTERNALERROR:                                                       \
    break;                                                                     \
  default:                                                                     \
    ov_logfile_error("lbalance_lbalance_helper: Unknown status");              \
    break;

#define LB_APPMON_W_SEP "&"
#define LB_APPMON_GSENAME_SEP "_"
#define LB_APPMON_APPREQ_SEP "\t"

#define LB_APPMON_INIT 0
#define LB_APPMON_DONE 1
#define LB_APPMON_ERROR 64

#define LB_BRAD_I 0
#define LB_BRAD_B 1
#define LB_BRAD_R 2
#define LB_BRAD_A 3
#define LB_BRAD_D 4

#define LB_REQSENDER_INIT 0
#define LB_REQSENDER_SENT 1

#define LB_REQREC_INIT 0
#define LB_REQREC_JSONLEN 2
#define LB_REQREC_IPPOS 0
#define LB_REQREC_LOADPOS 1

#define LB_NBINFORMER_INIT 0
#define LB_NBINFORMER_SENT 1
#define LB_NBINFORMER_JSONLEN 5
#define LB_NBINFORMER_IPPOS 0
#define LB_NBINFORMER_SERVERPOS 1
#define LB_NBINFORMER_LOADPOS 2
#define LB_NBINFORMER_CAPPOS 3
#define LB_NBINFORMER_INFOPOS 4

#define LB_NDB_INIT 0

#define LB_ACCEPTNOTIFIER_INIT 0
#define LB_ACCEPTNOTIFIER_SENT 1
#define LB_ACCEPTNOTIFIER_KEYWORD "ACCEPT"

#define LB_SENDINIT_INIT 0
