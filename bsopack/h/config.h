/* $Id: config.h,v 1.2 2003/10/17 08:32:55 andr_lukyanov Exp $ */

#ifndef _CONFIG_H
#define _CONFIG_H

#include <fidoconf/fidoconf.h>

extern s_fidoconfig *config;
extern char *logFileName;
extern char *fidoConfigFile;

void getConfig();
void freeConfig();
void getOpts(int argc, char **argv);

#endif
