/*
  Copyright (c) 2009 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2009 Center for Bioinformatics, University of Hamburg

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

#ifndef DIFF_COVER_H
#define DIFF_COVER_H

#include <stdbool.h>
#include "seqpos-def.h"

typedef unsigned long Diffvalue;

typedef struct Differencecover Differencecover;

Differencecover *differencecover_new(unsigned int vparam);
void differencecover_delete(Differencecover *dcov);
void differencecovers_check(Seqpos maxcheck,Seqpos totallength);
unsigned int differencecover_rank(const Differencecover *dcov,Seqpos pos);
unsigned int differencecover_offset(const Differencecover *dcov,
                                    Seqpos pos1,Seqpos pos2);
void differencecover_sample(const Differencecover *dcov,Seqpos totallength);

#endif
