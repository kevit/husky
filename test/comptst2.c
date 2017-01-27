/* Small code for test compiler directives and modifiers on compile time.
 *
 */

#pragma message("Test for near")
char * near ident="$Id: comptst2.c,v 1.1 2003/01/16 06:52:52 stas_degteff Exp $";

#pragma message("Test for far")
char * far r="$Revision: 1.1 $";

#pragma message("Test for huge")
char huge c[];

#pragma message("Test for cdecl")
char* cdecl a(){return "$Autor";}

#pragma message("Test for pascal")
char* pascal d(){ return "$Date: 2003/01/16 06:52:52 $";}

#pragma message("Test for fortran")
char fortran x(){ return 1;}

#pragma message("Test for interrupt handler")
int interrupt i1();
void interrupt i2();
cdecl interrupt i3();
