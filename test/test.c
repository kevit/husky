/*
$Id: test.c,v 1.68 2011/11/14 20:52:04 stas_degteff Exp $
   $Revision: 1.68 $
   $Date: 2011/11/14 20:52:04 $
   $Author: stas_degteff $

Test  endian (little endian or big endian)
*/

#include <stdio.h>
#include <string.h>

main(){
 unsigned int ival=0x34333231;
 char sval[5]="";
 memcpy(sval,&ival,4);
 printf("Little Endian test. Result \"1234\" should be on little-endian platform\n"
        "Integer in hex: %08X(hex), bytes: 1:%c 2:%c 3:%c 4:%c\n", ival, sval[0], sval[1], sval[2], sval[3] );
 printf("Binary representation of 32-bit integer in IO operation: \"");
 fwrite(&ival, sizeof(ival), 1, stdout);
 printf("\"\n");
 return (int)ival;
}
