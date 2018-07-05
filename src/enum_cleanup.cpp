/**
 * @file enum_cleanup.cpp
 * works on the last comma withing enum
 *
 * @author  Guy Maurel Juli 2018
 * @license GPL v2+
 */
#include "enum_cleanup.h"
#include "uncrustify_types.h"
#include "uncrustify.h"
#include "chunk_list.h"
#include "logger.h"


void enum_cleanup(void)
{
   LOG_FUNC_ENTRY();

   if (cpd.settings[UO_mod_enum_last_comma].a == AV_IGNORE)
   {
      // nothing to do
      return;
   }

   chunk_t *pc = chunk_get_head();  // Issue #858
   while (pc != nullptr)
   {
      if (  chunk_is_token(pc, CT_BRACE_CLOSE)
         && pc->parent_type == CT_ENUM)
      {
         LOG_FMT(LTOK, "%s(%d): orig_line is %zu, type is %s\n",
                 __func__, __LINE__, pc->orig_line, get_token_name(pc->type));
         chunk_t *prev = chunk_get_prev_ncnl(pc);
         if (prev == nullptr)
         {
            return;
         }
         if (chunk_is_token(prev, CT_COMMA))
         {
            if (cpd.settings[UO_mod_enum_last_comma].a == AV_REMOVE)
            {
               chunk_del(prev);
            }
         }
         else
         {
            if (  cpd.settings[UO_mod_enum_last_comma].a == AV_ADD
               || cpd.settings[UO_mod_enum_last_comma].a == AV_FORCE)
            {
               // create a comma
               chunk_t comma;
               comma.orig_line = prev->orig_line;
               comma.orig_col  = prev->orig_col + 1;
               comma.nl_count  = 0;
               comma.flags     = 0;
               comma.type      = CT_COMMA;
               comma.str       = ",";
               chunk_add_after(&comma, prev);
            }
         }
      }
      pc = chunk_get_next(pc);
   }
} // enum_cleanup
