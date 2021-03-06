/*
  Copyright (c) 2008-2009 Gordon Gremme <gordon@gremme.org>
  Copyright (c) 2008      Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef ASSERT_API_H
#define ASSERT_API_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Assert module */

#define GT_EXIT_PROGRAMMING_ERROR  2

#ifndef NDEBUG
/* The <gt_assert()> macro tests the given <expression> and if it is false, the
   calling process is terminated. A diagnostic message is written to <stderr>
   and the <exit(3)> function is called (with error code 2 as argument),
   effectively terminating the program.
   If <expression> is true, the <gt_assert()> macro does nothing. */
#define gt_assert(expression)                                                \
        do {                                                                 \
          if (!(expression)) {                                               \
            fprintf(stderr, "Assertion failed: (%s), function %s, file %s, " \
                    "line %d.\nThis is a bug, please report it at "          \
                    "https://github.com/genometools/genometools/issues.\n",  \
                    #expression, __func__, __FILE__, __LINE__);              \
            /*@ignore@*/                                                     \
            exit(GT_EXIT_PROGRAMMING_ERROR);                                 \
            /*@end@*/                                                        \
          }                                                                  \
        } while (false)
#else
#define gt_assert(expression) ((void) 0)
#endif

#endif
