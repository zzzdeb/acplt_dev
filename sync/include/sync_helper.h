#ifndef _SYNC_HELPER_
#define _SYNC_HELPER_

#include "libov/ov_macros.h"
#include "libov/ov_ov.h"

typedef struct {
  OV_STRING_VEC ksapiset;
  OV_STRING_VEC ksapiget;
} DSYNC_PCACHE;

#define DEFAULT_POSTSYS_EXECUTER "/data/exec"
#define MSGCREATOR_TRIGGER ".value"
#define DSYNC_PATH_DEST "/data/dsyncDst"
#define PLAYER_PATH_DEST "/data/dsyncDst.player"
#define DSYNC_DOWNLOAD_PATH_DEST_EXT ".syncDownload"
#define PLAYER_SRCNODE_PATH_DEST "/data/dsyncDst.player.bufNode"
#define PLAYER_DSTNODE_PATH_DEST "/data/dsyncDst.player.dstNode"

#define KSAPISET 0
#define KSAPIGET 1
#define SYNC_CONFIGURE_LEN 2

#define SYNC_DOWNLOAD_INIT 0
#define SYNC_DOWNLOAD_DONE 1

#define DSYNC_DST_INIT 0
#define DSYNC_DST_ACTIVE 1
#define DSYNC_DST_WAITINGPLAYER 2
#define DSYNC_DST_DONE 8
#define DSYNC_DST_ERROR 64

#endif
