/*
  Copyright (c) 2011 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2011 Center for Bioinformatics, University of Hamburg

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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "core/assert_api.h"
#include "core/types_api.h"
#include "match/stamp.h"

/* replaced byte with offset to avoid * 8 operation in loop */

/* be careful only works for litte endian byte order */

static void gt_radix_phase_GtUlong(unsigned int offset,
                                   GtUlong *source,
                                   GtUlong *dest,
                                   unsigned long len)
{
  unsigned long idx, s, c, *sp, *cp, count[256] = {0};
  const size_t increment = sizeof (*source);
  uint8_t *bp;

  /* count occurences of every byte value */
  bp = ((uint8_t *) source) + offset;
  for (idx = 0; idx < len; idx++, bp += increment)
  {
    count[*bp]++;
  }

  /* compute partial sums */
  for (s = 0, cp = count, idx = 0; idx < 256UL; idx++, cp++)
  {
    c = *cp;
    *cp = s;
    s += c;
  }

  /* fill dest with the right values in the right place */
  bp = ((uint8_t *) source) + offset;
  for (sp = source; sp < source + len; bp += increment, sp++)
  {
    dest[count[*bp]++] = *sp;
  }
}

void gt_radixsort_GtUlong(GtUlong *source, GtUlong *temp,unsigned long len)
{
  /* allocate heap memory to avoid the need of additional parameter */
  gt_assert(temp != NULL && source != NULL);
  gt_radix_phase_GtUlong(0, source, temp, len);
  gt_radix_phase_GtUlong(1U, temp, source, len);
  gt_radix_phase_GtUlong(2U, source, temp, len);
  gt_radix_phase_GtUlong(3U, temp, source, len);
#ifdef _LP64
  gt_radix_phase_GtUlong(4U, source, temp, len);
  gt_radix_phase_GtUlong(5U, temp, source, len);
  gt_radix_phase_GtUlong(6U, source, temp, len);
  gt_radix_phase_GtUlong(7U, temp, source, len);
#endif
}

#define GT_RADIX_ACCESS_CHAR(SP) ((*(SP) >> shift) & 255UL)

static void gt_radix_phase_GtUlong2(size_t offset,
                                    GtUlong *source,
                                    GtUlong *dest,
                                    unsigned long left,
                                    unsigned long right)
{
  unsigned long idx, s, c, *sp, *cp;
  const size_t maxoffset = sizeof (unsigned long) - 1;
  unsigned long count[256] = {0};
  const size_t shift = (maxoffset - offset) * CHAR_BIT;

  /* count occurences of every byte value */
  for (sp = source + left; sp <= source + right; sp++)
  {
    count[GT_RADIX_ACCESS_CHAR(sp)]++;
  }
  /* compute partial sums */
  for (s = 0, cp = count; cp < count + 256UL; cp++)
  {
    c = *cp;
    *cp = s;
    s += c;
  }
  /* fill dest with the right values in the right place */
  for (sp = source+left; sp <= source + right; sp++)
  {
    dest[left+count[GT_RADIX_ACCESS_CHAR(sp)]++] = *sp;
  }
  memcpy(source+left,dest+left,(size_t) sizeof (*source) * (right - left + 1));
  if (offset < maxoffset)
  {
    for (idx = 0; idx < 256UL; idx++)
    {
      unsigned long newleft = (idx == 0) ? 0 : count[idx-1];
      /* |newleft .. count[idx]-1| = count[idx]-1-newleft+1
                                   = count[idx]-newleft > 1
      => count[idx] > newleft + 1 */
      if (newleft+1 < count[idx])
      {
        gt_radix_phase_GtUlong2(offset+1,
                                source,
                                dest,
                                left+newleft,
                                left+count[idx]-1);
      }
    }
  }
}

void gt_radixsort_GtUlong2(GtUlong *source, GtUlong *dest, unsigned long len)
{
  gt_radix_phase_GtUlong2(0,source,dest,0,len-1);
}

/* assume that the first element in GtUlongPair is the sort key */

static void gt_radix_phase_GtUlongPair(unsigned int offset,
                                       GtUlongPair *source,
                                       GtUlongPair *dest,
                                       unsigned long len)
{
  unsigned long idx, *cp, s, c, count[256] = {0};
  GtUlongPair *sp;
  const size_t increment = sizeof (*source);
  uint8_t *bp;

  /* count occurences of every byte value */
  bp = ((uint8_t *) source) + offset;
  for (idx = 0; idx < len; idx++, bp += increment)
  {
    count[*bp]++;
  }

  /* compute partial sums */
  for (s = 0, cp = count, idx = 0; idx < 256UL; idx++, cp++)
  {
    c = *cp;
    *cp = s;
    s += c;
  }

  /* fill dest with the right values in the right place */
  bp = ((uint8_t *) source) + offset;
  for (sp = source; sp < source + len; bp += increment, sp++)
  {
    dest[count[*bp]++] = *sp;
  }
}

void gt_radixsort_GtUlongPair(GtUlongPair *source, GtUlongPair *temp,
                              unsigned long len)
{
  /* allocate heap memory to avoid the need of additional parameter */
  gt_assert(temp != NULL && source != NULL);
  gt_radix_phase_GtUlongPair(0, source, temp, len);
  gt_radix_phase_GtUlongPair(1U, temp, source, len);
  gt_radix_phase_GtUlongPair(2U, source, temp, len);
  gt_radix_phase_GtUlongPair(3U, temp, source, len);
#ifdef _LP64
  gt_radix_phase_GtUlongPair(4U, source, temp, len);
  gt_radix_phase_GtUlongPair(5U, temp, source, len);
  gt_radix_phase_GtUlongPair(6U, source, temp, len);
  gt_radix_phase_GtUlongPair(7U, temp, source, len);
#endif
}
