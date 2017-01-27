/* $Id: bsoutil.h,v 1.3 2004/01/14 02:56:49 andr_lukyanov Exp $ */

#ifndef _BSOUTIL_H_
#define _BSOUTIL_H_

#ifndef MAXPATH
#define MAXPATH 256
#endif

void packNetMailForLink(s_link *link);
char *addr2str(s_link *link);

#endif
