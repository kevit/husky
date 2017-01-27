/* $Id: version.h,v 1.6 2010/02/28 12:24:22 stas_degteff Exp $
   This file is part of NLTOOLS, the nodelist processor of the Husky fidonet
   software project.
*/

#ifndef _VERSION_H
#define _VERSION_H

#include "cvsdate.h"

/* basic version number */
#define VER_MAJOR 1
#define VER_MINOR 9
#define VER_PATCH 0
#define VER_BRANCH BRANCH_CURRENT

extern char      *versionStr;

#define printversion(  ) { printf( "%s\n\n", versionStr ); }

#endif
