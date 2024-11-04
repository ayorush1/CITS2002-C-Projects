
// globals 

#include "mysync.h"



struct Directory *directories = NULL;
int ndirs = 0; 

struct File *syncing_fileslist = NULL; 
int sync_files_count = 0; 

char **ignoredfiles = NULL;         
int sizeofignored = 0;              
char **onlyfiles = NULL;
int sizeofonly = 0;

bool nflag = false; 
bool vflag = false; 
bool pflag = false; 
bool iflag = false; 
bool oflag = false; 