/*
  Copyright (c) 2007 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg

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

#ifndef SFX_INPUT_H
#define SFX_INPUT_H

#include "core/filelengthvalues.h"
#include "core/arraydef.h"
#include "seqpos-def.h"
#include "sfx-progress.h"
#include "encseq-def.h"

typedef struct
{
  Encodedsequence *encseq;
} Sfxseqinfo;

int fromfiles2Sfxseqinfo(Sfxseqinfo *sfxseqinfo,
                         ArraySeqpos *sequenceseppos,
                         Sfxprogress *sfxprogress,
                         const GtStr *str_indexname,
                         const GtStr *str_smap,
                         const GtStr *str_sat,
                         const GtStrArray *filenametab,
                         bool isdna,
                         bool isprotein,
                         bool isplain,
                         bool outtistab,
                         bool outdestab,
                         bool outsdstab,
                         bool outssptab,
                         Verboseinfo *verboseinfo,
                         GtError *err);

int fromsarr2Sfxseqinfo(Sfxseqinfo *sfxseqinfo,
                        const GtStr *indexname,
                        Verboseinfo *verboseinfo,
                        GtError *err);

void freeSfxseqinfo(Sfxseqinfo *sfxseqinfo);

#endif
