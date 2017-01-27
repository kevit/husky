/* $Id: version.h,v 1.1 2010/07/21 09:57:32 stas_degteff Exp $ */
/******************************************************************************
 * HPTZIP --- library for zipinternal packer/unpacker
 ******************************************************************************
 * Copyright (C) 1998-2002
 *
 * Husky Delopment Team
 *
 * Internet: http://husky.sourceforge.net
 *
 * This file is part of HPTZIP.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library/Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; see file COPYING. If not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * See also http://www.gnu.org
 *****************************************************************************
 */

#ifndef __HPTZIP__VERSION_H
#define __HPTZIP__VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hptzip.h"

/* values for 5th parameter of GenVersionStr() */
/* typedef enum { */
/*        BRANCH_CURRENT=1, BRANCH_STABLE=2, BRANCH_RELEASE=3 */
/* }branch_t; */

/* this is version number of HPTzip */
#define HZ_VER_MAJOR 1
#define HZ_VER_MINOR 9
#define HZ_VER_PATCH 0
#define HZ_VER_BRANCH BRANCH_CURRENT

/* Check version of fidoconfig library
 * return zero if test failed; non-zero if passed
 * test cvs need for DLL version only, using #include <fidoconf/cvsdate.h>
  const char *hptzipdate(){
  static
  #include "../hptzip/cvsdate.h"
  return cvs_date;
  }
  CheckHptzipVersion( ..., hptzipdate());
 */
HUSKYEXT int CheckHptzipVersion( int need_major, int need_minor,
                      int need_patch, branch_t need_branch, const char *cvs );

#ifdef __cplusplus
}
#endif

#endif
