# $Id: makefile.inc,v 1.2 2010/03/27 15:59:20 nick_voronin Exp $
#
# Common makefiles part (include-file) for hptkill

# Use defines before including this file:
# _OBJ	object files suffix begins with point (".o", ".obj", ... )
# _LIB	library files suffix begins with point (".a", ".lib", ... )
# _DLL	dinamic linking library files suffix begins with point (".dll", ".so", ...)
# LIBPREFIX  library filename prefix ("lib", "", ...)
# LIBSUFFIX  library filename suffix (compiler ID, three characters, somewhere empty)
# DLLPREFIX  dinamic linking library prefix (usually empty)
# DLLSUFFIX  dinamic linking library suffix (compiler ID or empty)
# O, B, L, H prefixes to use with names of objects, binaries, libraries and headers respectively

# Module name
LIBNAME = hptkill

# Version
VERMAJOR= 1
VERMINOR= 9
VERPATCH= 0
VERH	= $(VERMAJOR).$(VERMINOR)
VER	= $(VERH).$(VERPATCH)

# Sources directory
_SRC_DIR= ../src/
# Headers directory
H_DIR	= ../h/

# Header files
HEADERS	= $(H)cvsdate.h $(H)version.h

# Object files (please sort list to easy check by human)
OBJS	= $(O)hptkill$(_OBJ)


# Binary file(s) to build from sources
PROGRAMS= $(B)$(LIBNAME)$(_EXE)

# List of FIDOCONFIG libraries required to build binary file(s)
LIBS	=
