# $Id: makefile.be,v 1.1 2002/06/26 19:36:58 stas Exp $
#
# Generic Legacy Makefile for BeOS systems
# Written by Paul Galashin, 2:5053/777.12
#
# No support for the Husky build environment, no support for Fidoconfig
# Direct Termcap / ANSI terminal I/O

# install programs in:
BINDIR=~/bin

# where to put the recoding table
READMAPS=~/.msged.readmaps
WRITMAPS=~/.msged.writmaps

# where to find the configuration file
DEFAULT_CONFIG_FILE=~/.msged

INCDIR=..

# options for installing programs
IBOPT=-s -c

CC=     cc
CFLAGS= -I$(INCDIR)
CDEFS=  -DUNIX -DUSE_MSGAPI -DUNAME=\"BeOS\" -DREADMAPSDAT='"$(READMAPS)"'
-DWRITMAPSDAT='"$(WRITMAPS)"' -DDEFAULT_CONFIG_FILE='"$(DEFAULT_CONFIG_FILE)"'
COPT=   -O3
LFLAGS= -L../smapi -L../fidoconfig -s

TARGET= msged
MSGAPI=-lsmapibe
TERMCAP=-ltermcap

objs=   addr.o     \
        ansi.o     \
        areas.o    \
        bmg.o      \
        charset.o  \
        config.o   \
        control.o  \
        curses.o   \
        date.o     \
        dialogs.o  \
        dirute.o   \
        dlgbox.o   \
        dlist.o    \
        echotoss.o \
        environ.o  \
        fconf.o    \
        fecfg145.o \
        fido.o     \
        filedlg.o  \
        flags.o    \
        freq.o     \
        gestr120.o \
        getopts.o  \
        group.o    \
        help.o     \
        helpcmp.o  \
        helpinfo.o \
        init.o     \
        keycode.o  \
        list.o     \
        maintmsg.o \
        makemsgn.o \
        memextra.o \
        menu.o     \
        misc.o     \
        mnu.o      \
        msg.o      \
        msged.o    \
        mxbt.o     \
        normalc.o  \
        nshow.o    \
        quick.o    \
        quote.o    \
        readmail.o \
        readtc.o   \
        screen.o   \
        strextra.o \
        system.o   \
        template.o \
        textfile.o \
        timezone.o \
        userlist.o \
        vsev.o     \
        vsevops.o  \
        win.o      \
        wrap.o

all: $(TARGET) testcons

.c.o:
        $(CC) $(COPT) $(CFLAGS) $(CDEFS) -c $<

$(TARGET): $(objs)
        $(CC) $(COPT) $(LFLAGS) -o $(TARGET) $(objs) $(MSGAPI) $(TERMCAP)

testcons:  testcons.o
        $(CC) $(COPT) $(LFLAGS) -o testcons testcons.o $(TERMCAP)

clean:
        -rm *.o *~

distclean: clean
        -rm $(TARGET) testcons

install: $(TARGET)
        install -c $(IBOPT) $(TARGET) $(BINDIR)
        install -c bin/latin/readmaps.dat $(READMAPS)
        install -c bin/latin/writmaps.dat $(WRITMAPS)
