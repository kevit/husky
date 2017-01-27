/* $Id: seenby.h,v 1.10 2011/02/15 20:12:30 stas_degteff Exp $ */
#ifndef _SEENBY_H_
#define _SEENBY_H_

#include <fidoconf/fidoconf.h>
#include "fcommon.h"

int seenbyAdd  ( hs_addr **seenby, UINT* anzseenby, hs_addr *Aka);
int seenbyComp ( hs_addr *seenby, int anzseenby, hs_addr Aka);
int seenbySort ( hs_addr *seenby, int anzseenby);

#endif
