/* sockutil.h - simple utility functions declarations */

/* $Id: sockutil.h,v 1.2 2005/09/08 17:55:34 stas_degteff Exp $ */

#ifndef __HUSKY__SOCKUTIL_H
#define __HUSKY__SOCKUTIL_H

void die(char * message);

#ifndef CMSG_DATA
#define CMSG_DATA(cmsg) ((cmsg)->cmsg_data)
#endif

#endif
