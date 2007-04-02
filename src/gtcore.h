/*
  Copyright (c) 2003-2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2003-2007 Center for Bioinformatics, University of Hamburg
  See LICENSE file or http://genometools.org/license.html for license details.
*/

#ifndef GTCORE_H
#define GTCORE_H

/* The core GenomeTools library (libgtcore) header */
#include <libgtcore/alpha.h>         /* alphabet class */
#include <libgtcore/array.h>         /* array class */
#include <libgtcore/array2dim.h>     /* 2-dimensional array class */
#include <libgtcore/bioseq.h>        /* biosequence class */
#include <libgtcore/bittab.h>        /* bittab class */
#include <libgtcore/countingsort.h>  /* countingsort module */
#include <libgtcore/cstr.h>          /* C-string class */
#include <libgtcore/disc_distri.h>   /* discrete distribution class */
#include <libgtcore/dlist.h>         /* double-linked list class */
#include <libgtcore/ensure.h>        /* defines the ensure macro */
#include <libgtcore/env.h>           /* environment class */
#include <libgtcore/fasta.h>         /* fasta module */
#include <libgtcore/fileutils.h>     /* file utilities module */
#include <libgtcore/gc_content.h>    /* GC-content module */
#include <libgtcore/grep.h>          /* grep module */
#include <libgtcore/hashtable.h>     /* hashtable class */
#include <libgtcore/mathsupport.h>   /* math support module */
#include <libgtcore/minmax.h>        /* min/max module */
#include <libgtcore/msort.h>         /* msort module */
#include <libgtcore/option.h>        /* option parser class */
#include <libgtcore/outputfile.h>    /* outputfile module */
#include <libgtcore/orf.h>           /* ORF module */
#include <libgtcore/parseutils.h>    /* parse utilities module */
#include <libgtcore/phase.h>         /* phase module */
#include <libgtcore/progressbar.h>   /* progressbar module */
#include <libgtcore/queue.h>         /* queue class */
#include <libgtcore/range.h>         /* range class */
#include <libgtcore/safeop.h>        /* safe operations module */
#include <libgtcore/scorefunction.h> /* score function class */
#include <libgtcore/scorematrix.h>   /* score matrix class */
#include <libgtcore/splitter.h>      /* splitter class */
#include <libgtcore/str.h>           /* string class */
#include <libgtcore/strand.h>        /* strand module */
#include <libgtcore/tokenizer.h>     /* tokenizer class */
#include <libgtcore/translate.h>     /* translate module */
#include <libgtcore/undef.h>         /* undef module */
#include <libgtcore/versionfunc.h>   /* version module */
#include <libgtcore/warning.h>       /* warning module */
#include <libgtcore/xansi.h>         /* ANSI wrapper module */
#include <libgtcore/xposix.h>        /* POSIX wrapper module */

#endif
