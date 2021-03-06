# $Id: makefile.inc,v 1.4 2002/11/18 14:31:22 stas_degteff Exp $
# include-file for other makefiles
#
# This file is part of the Emailpkt package,
# part of the husky fidosoft project
#

OBJS	= encdec$(OBJ) misc$(OBJ)
DOCS	+= README
HFILES	= cvsdate.h ecommon.h version.h
MANPAGES= hesend.1 hereceive.1

commonall: commonhesend commonhereceive

commonhesend: $(OBJS) hesend$(OBJ)
	$(CC) $(LFLAGS) -o hesend$(EXE) $(OBJS) hesend$(OBJ) $(LIBS)

commonhereceive:  $(OBJS) hereceive$(OBJ)
	$(CC) $(LFLAGS) -o hereceive$(EXE) $(OBJS) hereceive$(OBJ) $(LIBS)

commonclean:
	-$(RM) $(RMOPT) *$(OBJ)

commondistclean: commonclean
	-$(RM) $(RMOPT) hesend$(EXE)
	-$(RM) $(RMOPT) hereceive$(EXE)
