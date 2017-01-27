# $Id: makefile.inc,v 1.3 2003/05/16 10:22:55 val_khokhlov Exp $
# Common part of makefiles: object files list & etc.
# (Include file for other makefiles)
# Written by Stas Degteff 2:5080/102; g@grumbler.org

OBJS    = sortarea$(_OBJ) purgearea$(_OBJ) packarea$(_OBJ) linkarea$(_OBJ) \
          fixarea$(_OBJ) undelete$(_OBJ) hptutil$(_OBJ)

default: all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(TARGETFLAG)$(TARGET) $(OBJS) $(LIBS)

commonclean:
	-$(RM) $(RMOPT) *$(_OBJ)

commondistclean:
	-$(RM) $(RMOPT) $(TARGET)
