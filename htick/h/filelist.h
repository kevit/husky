/* $Id: filelist.h,v 1.11 2011/02/15 20:12:30 stas_degteff Exp $ */
#ifndef _FILELIST_H
#define _FILELIST_H

#include <huskylib/compiler.h>

#include <fidoconf/fidoconf.h>
#if defined(__TURBOC__) && defined(__DOS__)
typedef unsigned long off_t;
#endif

void filelist(void);

#endif
