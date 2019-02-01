/******************************************************************************
 *
 *   file
 *   ----
 *   time.c
 *
 *   history
 *   -------
 *   Do 31 Jan 2019 17:14:18 CET   file created
 *
 ******************************************************************************/

#include "object_helper.h"
#include "ovhelper.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT void ov_timespan_multiply(OV_TIME_SPAN*      res,
                                          const OV_TIME_SPAN src,
                                          const OV_SINGLE    factor) {
  if(!res)
    return;
  OV_TIME tres = {res->secs, res->usecs};
  OV_TIME tsrc = {src.secs, src.usecs};
  ov_time_multiply(&tres, tsrc, factor);
  res->secs = tres.secs;
  res->usecs = tres.usecs;
  return;
}

OV_DLLFNCEXPORT void ov_time_multiply(OV_TIME* res, const OV_TIME src,
                                      const OV_SINGLE factor) {
  if(!res)
    return;
  res->secs = 0;
  res->usecs = 0;
  if(factor < 0) {
    ov_logfile_warning("ovhelper_time: cant handle negative multiplication");
    return;
  }
  if(factor == 0) {
    res->secs = 0;
    res->usecs = 0;
    return;
  }
  OV_UINT secs = src.secs;
  OV_UINT usecs = src.usecs;
  if(factor > 1) {
		usecs = src.usecs * factor;
		secs = src.secs * factor;
    secs += (OV_UINT)(usecs / 1000000);
    usecs = usecs % 1000000;
  }
  if(factor < 1) {
    secs = src.secs / factor;
    usecs = ((src.secs - secs) * 1000000 + src.usecs) / factor;
  }
  res->secs = secs;
  res->usecs = usecs;
  return;
}
