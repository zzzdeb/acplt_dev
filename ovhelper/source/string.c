
/******************************************************************************
 *
 *   FILE
 *   ----
 *   string.c
 *
 *   History
 *   -------
 *   2018-01-25   File created
 *
 ******************************************************************************/

#include "libov/ov_macros.h"
#include "libov/ov_string.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OV_DLLFNCEXPORT OV_RESULT ov_string_get(OV_STRING* str1, OV_STRING str2,
                                        OV_INT beg, OV_INT end) {
  /*
   *	local variables
   */
  OV_STRING res = NULL;
  OV_UINT   slen = ov_string_getlength(str2);
  OV_UINT   nlen, nbeg, nend;
  OV_UINT   tempMaxStringLength = 0;
  /*
   *	check parameters
   */
  if(!str1)
    return OV_ERR_BADPARAM;

  if(!str2)
    return OV_ERR_BADPARAM;

  if(abs(beg) > slen || abs(end) > slen)
    return OV_ERR_BADPARAM;

  nbeg = beg < 0 ? slen + beg : beg;
  nend = end < 0 ? slen + end : end;

  if(nbeg == nend) {
    ov_string_setvalue(str1, NULL);
    return OV_ERR_OK;
  }

  if(nbeg > nend)
    return OV_ERR_BADPARAM;

  nlen = nend - nbeg;
  /*
   *	get strings length and check for limits
   */
  tempMaxStringLength = ov_vendortree_MaxStringLength();
  if(tempMaxStringLength && nlen + 1 > tempMaxStringLength) {
    return OV_ERR_BADVALUE;
  }

  /*
   *	allocate memory for new string
   */
  res = ov_database_realloc(*str1, nlen + 1);
  if(!res) {
    return OV_ERR_DBOUTOFMEMORY;
  };

  /*
   *	copy the new value
   */
  strncpy(res, str2 + beg, nlen);
  *(res + nlen) = '\0';

  /*
   *	finish
   */
  *str1 = res;

  return OV_ERR_OK;
}

/* checks if str in vec is. */
OV_DLLFNCEXPORT OV_BOOL ov_strvec_contains(const OV_STRING_VEC* vec,
                                           const OV_STRING      str) {
  if(!vec || !str)
    return 0;

  for(OV_UINT i = 0; i < vec->veclen; i++) {
    if(!ov_string_compare(vec->value[i], str))
      return 1;
  }
  return 0;
}
