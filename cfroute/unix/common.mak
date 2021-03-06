### Requires gmake!

### Build rules
.SUFFIXES:
.SUFFIXES: .cpp $(OBJ) .c

VPATH=$(SRCDIR)
.cpp$(OBJ):
	$(CXX) -c -o $*$(OBJ) -I$(SRCDIR) -I../.. $(CFLAGS) $(REL) $<
.c$(OBJ):
	$(CC) -c -o $*$(OBJ) -I$(SRCDIR) -I../.. $(CFLAGS) $(REL) $<

all: cfroute$(EXE) fc2cfr$(EXE)

cfroute$(EXE): $(cfrobjs)
	$(CXX) $(LFLAGS) -o cfroute$(EXE) $(cfrobjs) $(LIBS)

fc2cfr$(EXE): fc2cfr$(OBJ)
	$(CXX) $(LFLAGS) -o fc2cfr$(EXE) fc2cfr$(OBJ) $(LIBS2) $(LIBS)

cfroute.o: akas.cpp basic.cpp config.cpp datetime.cpp \
        encdet.cpp  errors.hpp fastecho.cpp handlers.cpp log.cpp macro.cpp \
        netmail.cpp password.cpp protos.hpp routing.cpp scontrol.cpp \
        errors.hpp fecfg146.h protos.hpp squish.cpp dirute.h buffer.hpp \
	structs.hpp
dirute.o: dirute.c dirute.h
buffer.o: buffer.cpp buffer.hpp
structs.o: structs.cpp structs.hpp
inbounds.o: inbounds.cpp inbounds.hpp
fecfg146.o: fecfg146.c fecfg146.h

clean:
	-rm cfroute$(EXE)
	-rm cfroute$(OBJ)
	-rm dirute$(OBJ)
	-rm buffer$(OBJ)
	-rm structs$(OBJ)
	-rm fecfg146$(OBJ)
	-rm fc2cfr$(OBJ)
