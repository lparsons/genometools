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

#include "core/chardef.h"
#include "core/symboldef.h"
#include "seqpos-def.h"
#include "divmodmul.h"
#include "esa-splititv.h"

#define SEQUENCE(ENCSEQ,POS) (((POS) == totallength) \
                             ? (GtUchar) SEPARATOR\
                             : getencodedchar(ENCSEQ,POS,readmode))

static Seqpos lcpintervalfindrightbound(const Encodedsequence *encseq,
                                        Readmode readmode,
                                        Seqpos totallength,
                                        const Seqpos *suftab,
                                        GtUchar cc,
                                        Seqpos offset,
                                        Seqpos left,
                                        Seqpos right)
{
  Seqpos pos, mid;
  GtUchar midcc;

  while (right > left+1)
  {
    mid = DIV2(left+right);
    pos = suftab[mid] + offset;
    midcc = SEQUENCE(encseq,pos);
    if (cc < midcc)
    {
      right = mid;
    } else
    {
      left = mid;
    }
  }
  return left;
}

bool lcpintervalfindcharchildintv(const Encodedsequence *encseq,
                                  Readmode readmode,
                                  Seqpos totallength,
                                  const Seqpos *suftab,
                                  Simplelcpinterval *itv,
                                  GtUchar cc,
                                  Seqpos offset,
                                  Seqpos left,
                                  Seqpos right)
{
  GtUchar leftcc, rightcc;
  Seqpos pos, rightbound, leftbound = left;

  pos = suftab[right] + offset;
  rightcc = SEQUENCE(encseq,pos);
  while (true)
  {
    pos = suftab[leftbound] + offset;
    leftcc = SEQUENCE(encseq,pos);
    if (leftcc == rightcc)
    {
      break;
    }
    rightbound = lcpintervalfindrightbound(encseq,readmode,
                                           totallength,suftab,leftcc,
                                           offset,leftbound,right);
    if (leftcc == cc)
    {
      itv->left = leftbound;
      itv->right = rightbound;
      return true;
    }
    if (leftcc > cc)
    {
      return false;
    }
    leftbound = rightbound+1;
  }
  if (leftcc == cc)
  {
    itv->left = leftbound;
    itv->right = right;
    return true;
  }
  return false;
}

#define ADDCURRENTLBOUND(V)\
        bwci->spaceBoundswithchar[bwci->nextfreeBoundswithchar].lbound = V

#define ADDPREVIOUSRBOUND(V)\
        if (bwci->nextfreeBoundswithchar > 0)\
        {\
          bwci->spaceBoundswithchar[bwci->nextfreeBoundswithchar-1].rbound = V;\
        }

#define ADDCURRENTINCHAR(V)\
        bwci->spaceBoundswithchar[bwci->nextfreeBoundswithchar++].inchar = V

void lcpintervalsplitwithoutspecial(ArrayBoundswithchar *bwci,
                                    const Encodedsequence *encseq,
                                    Readmode readmode,
                                    Seqpos totallength,
                                    const Seqpos *suftab,
                                    Seqpos parentoffset,
                                    Seqpos parentleft,
                                    Seqpos parentright)
{
  GtUchar leftcc, rightcc;
  Seqpos rightbound = 0, leftbound = parentleft;

  /* call lcpintervalextendlcp and verify if interval can be extended by
     some character */
  bwci->nextfreeBoundswithchar = 0;
  rightcc = SEQUENCE(encseq,suftab[parentright] + parentoffset);
  while (true)
  {
    leftcc = SEQUENCE(encseq,suftab[leftbound] + parentoffset);
    gt_assert(bwci->nextfreeBoundswithchar < bwci->allocatedBoundswithchar);
    if (ISSPECIAL(leftcc))
    {
      ADDPREVIOUSRBOUND(rightbound);
      ADDCURRENTLBOUND(rightbound+1);
      return;
    }
    ADDPREVIOUSRBOUND(leftbound-1);
    ADDCURRENTLBOUND(leftbound);
    ADDCURRENTINCHAR(leftcc);
    if (leftcc == rightcc)
    {
      break;
    }
    rightbound = lcpintervalfindrightbound(encseq,readmode,totallength,suftab,
                                           leftcc,parentoffset,
                                           leftbound,parentright);
    leftbound = rightbound+1;
  }
  gt_assert(bwci->nextfreeBoundswithchar < bwci->allocatedBoundswithchar);
  ADDPREVIOUSRBOUND(parentright);
  ADDCURRENTLBOUND(parentright+1);
}

GtUchar lcpintervalextendlcp(const Encodedsequence *encseq,
                           Readmode readmode,
                           const Seqpos *suftab,
                           Seqpos totallength,
                           GtUchar alphasize,
                           Seqpos parentoffset,
                           Seqpos parentleft,
                           Seqpos parentright)
{
  GtUchar ccl, ccr;

  ccl = SEQUENCE(encseq,suftab[parentleft] + parentoffset);
  ccr = SEQUENCE(encseq,suftab[parentright] + parentoffset);
  if (ccl != ccr || ISSPECIAL(ccl))
  {
    return alphasize;
  }
  gt_assert(ccl < alphasize);
  return ccl;
}
