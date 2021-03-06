/* platform-specific header file, linux version */

#ifndef __OS_H__
#define __OS_H__

#define UNIX
#define LINUX
#define OS "Linux"
#define dirSepC '/'
#define dirSepS "/"
#define osDir "linux"
#define osTmpDir "/tmp/"

extern char *defaults[numIdx];

#define numZipFiles 12
char *zipFiles[numZipFiles];

#define numPrograms 8
char *programs[numPrograms];

#define numNeededPrograms 2
char *neededPrograms[numNeededPrograms];

#define numMakeCfgFiles 2
tCfgFileMap makeCfgFiles[numMakeCfgFiles];

#define numGlobalCfgFiles 7
tCfgFileMap globalCfgFiles[numGlobalCfgFiles];


// returns 0 if file exists
int fexist(char *fname);

// returns 0 if directory exists
int direxist(char *dname);

// returns 0 if successfull
int copyFile(char *sourceName, char *destName);

// returns 0 if successfull
int touchFile(char *fname);

// returns 0 if successfull
int linkFile(char *source, char *dest);

// display a file
void showFile(char *fname);

// get the contents of a file
char *readFile(char *fname);

// create directory and parent directories (if needed), returns 0 if
// successfull
int mkdirp(char *dirname);

// unpack archive into current directory
// returns 0 if successfull
int unpackFile(char *fname);

void waitForKey();

// checks if all a program is installed, returns 0 if found
int checkprogram(char *name);

// returns 0 if successfull
int createusers();

// set access rights
void setMode(char *fname, int mode);

// set default permission mask
void setUmask(int mask);

// set user and group
void setOwner(char *path, char *userName, char *groupName);

// returns 0 if successfull
int createUserConfig(char *userName, char *groupName);

// returns 0 if successfull
int createSystemConfigPrecompile();

// returns 0 if successfull
int createSystemConfigAfterInstall();

// returns 0 if user has root privileges
int checkRootAccess();

// call a function as the specified user and group
int callAsUser(char *userName, char *groupName,
	       int func(char *userName, char *groupName) );

// returns 0 on success
int setVar(char *varName, char *content);

char *getVar(char *varName);

// returns 0 if successfull
int compileNodelists(char *userName, char *groupName);

// clear screen
void clrscr();

// init os-dependent variables, do os-dep. checks
int osInit();

// free os-dependent variables
void osDone();

#endif

