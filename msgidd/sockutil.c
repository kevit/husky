/* sockutil.c - simple utility functions */

/* $Id: sockutil.c,v 1.3 2005/09/08 19:01:03 stas_degteff Exp $ */

#include <stdio.h>
#include <stdlib.h>

#include "sockutil.h"

/* issue an error message via perror() and terminate the program */
void die(char * message) {
    perror(message);
    exit(1);
}
