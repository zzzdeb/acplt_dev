/******************************************************************************
 *
 *   FILE
 *   ----
 *   inbox.c
 *
 *   History
 *   -------
 *   Sa 29 Dez 2018 12:26:10 CET   File created
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_PostSys
#define OV_COMPILE_LIBRARY_PostSys
#endif

#include "PostSys.h"
#include "PostSys_helper.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"

#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_INSTPTR_PostSys_inbox
                PostSys_inbox_get(OV_INSTPTR_ov_domain pdom, OV_BOOL create) {
  OV_RESULT                result = OV_ERR_OK;
  OV_INSTPTR_PostSys_inbox inboxdomain = NULL;
  char                     inboxName[] = "INBOX";
  OV_ELEMENT               parentObject, part;
  OV_UINT                  inboxNameLength = sizeof(inboxName);
  OV_UINT                  identifier_length = 0;
  ov_memstack_lock();

  if(!pdom) {
    ov_logfile_warning("PostSys_inbox: badparam null pointer");
    ov_memstack_unlock();
    return NULL; /*	we could not find the service, but we don't care
                         :-) (the specs...)	*/
  }

  /*	iterate over all children in containment to find inbox	*/
  Ov_ForEachChildEx(ov_containment, pdom, inboxdomain, PostSys_inbox) {
    if(ov_string_comparei(inboxName, inboxdomain->v_identifier) ==
       OV_STRCMP_EQUAL) { /*	inbox found	*/
      break;
    }
  }
  if(!inboxdomain) {
    /*	inbox not found, maybe it's a part...	*/
    parentObject.pobj = Ov_StaticPtrCast(ov_object, pdom);
    parentObject.elemtype = OV_ET_OBJECT;
    part.elemtype = OV_ET_NONE;

    /*	iterate over all parts	*/
    ov_element_getnextpart(&parentObject, &part, OV_ET_OBJECT);
    while(part.elemtype != OV_ET_NONE) {
      if(part.elemunion.ppart) {
        inboxdomain = Ov_DynamicPtrCast(PostSys_inbox, part.pobj);
        if(inboxdomain) {
          identifier_length = strlen(inboxdomain->v_identifier) + 1;
          if(identifier_length !=
             inboxNameLength) /*	identifiers length differs from "INBOX"
                                 --> this must be the wrong one	*/
            continue;
          else {
            if(ov_string_compare(
                   inboxName, ov_string_toupper(inboxdomain->v_identifier)) ==
               OV_STRCMP_EQUAL) { /*	inbox found	*/
              break;
            }
          }
        }
        inboxdomain = NULL;
      }
      ov_element_getnextpart(&parentObject, &part, OV_ET_OBJECT);
    }

    if(!inboxdomain) {
      /*	not a part either	*/
      if(!create) {
        ov_memstack_unlock();
        return NULL;
      } else {
        result = Ov_CreateObject(PostSys_inbox, inboxdomain, pdom, "inbox");
        if(Ov_Fail(result)) {
          ov_logfile_error("%u: %s: couldnot create inbox", result,
                           ov_result_getresulttext(result));
          ov_memstack_unlock();
          return NULL;
        }
        ov_memstack_unlock();
        return inboxdomain;
      }
    }
  }
  return NULL;
}
