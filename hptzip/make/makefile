#
# Universal makefile for GNU make for hptzip
# HUSKY project (c) 1998-2003 HUSKY Team.
#
# $Id: makefile,v 1.13 2010/07/21 09:37:37 stas_degteff Exp $
#

include ../../huskymak.cfg
include osdep.inc

TOP=../..
SRCDIR=../src
_H_DIR = $(MODULENAME)

MODULENAME=hptzip
VER_MAJOR=0
VER_MINOR=1
VER_RELEASE=0

ifeq ($(DEBUG), 1)
  CFLAGS = -I../hptzip -I$(INCDIR) $(DEBCFLAGS) $(WARNFLAGS)
  LFLAGS = $(DEBLFLAGS)
else
  CFLAGS = -I../hptzip -I$(INCDIR) $(OPTCFLAGS) $(WARNFLAGS)
  LFLAGS = $(OPTLFLAGS)
endif
# /vk was:
#CFLAGS+= -I../hptzip -I$(INCDIR) $(OPTCFLAGS) $(WARNFLAGS)
#LFLAGS+= -L../../build/$(PLATFORM)/lib -I$(LIBDIR)

SRCS_libhptzip=hptzip.c zip.c unzip.c
ifeq ($(PLATFORM),win32)
   SRCS_libhptzip+= iowin32.c
else
   SRCS_libhptzip+= ioapi.c
endif
OBJS_libhptzip=$(call src2obj, $(SRCS_libhptzip))
LIBS_libhptzip=z husky
CFLAGS_libhptzip=
LFLAGS_libhptzip=

LIBRARIES=hptzip
BINARIES=
HEADERS=hptzip.h

include makefile.prg
