# $Id: makefile.inc,v 1.3 2004/01/14 07:10:18 andr_lukyanov Exp $
#
# Common makefiles part (include-file) for mpost

# Use defines before including this file:
# _OBJ	object files suffix begins with point (".o", ".obj", ... )
# _LIB	library files suffix begins with point (".a", ".lib", ... )
# _DLL	dinamic linking library files suffix begins with point (".dll", ".so", ...)
# LIBPREFIX  library filename prefix ("lib", "", ...)
# LIBSUFFIX  library filename suffix (compiler ID, three characters, somewhere empty)
# DLLPREFIX  dinamic linking library prefix (usually empty)
# DLLSUFFIX  dinamic linking library suffix (compiler ID or empty)
# _SRC_DIR  sources dir

LIBNAME = mpost

# Version
VERMAJOR= 2
VERMINOR= 0
VERPATCH= 0
VERH	= $(VERMAJOR).$(VERMINOR)
VER	= $(VERH).$(VERPATCH)

# Object files (please sort list to easy check by human)
OBJS	= mpostp$(_OBJ)

# Header files
HEADERS	=

# Sources directory
_SRC_DIR= ./

# Headers directory
H_DIR	= ./h/

# Binary file(s) to build from sources
PROGRAMS= $(LIBNAME)$(_EXE)

# List of libraries required to build binary file(s)
LIBS	=
