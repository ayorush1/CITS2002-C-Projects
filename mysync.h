

//  CITS2002 Project 2 2023
//  Student1:   23730792    Arush Kathal

#define OPTLIST "ai:no:prv"                                 // define all possible command-line options 
#define _POSIX_C_SOURCE  200809L                            // define the required POSIX standard


#include <string.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#include <getopt.h> 

#include  <sys/types.h>
#include  <dirent.h>
#include  <sys/stat.h>
#include  <sys/param.h>
#include  <unistd.h>
#include  <time.h> 
#include  <fcntl.h>
#include <regex.h>                                          



#define CHECK_ALLOC(p) if (p == NULL) { \
    fprintf(stderr, "Error: could not allocate memory\n"); \
    exit(EXIT_FAILURE); \
}                                                           // define macro for checking memory allocation. 

#define CHECK_OPEN(q, dirname) if (q == NULL) { \
    fprintf(stderr, "Error: directory address: %s could not be opened\n", dirname); \
    exit(EXIT_FAILURE); \
}                                                           // define macro for checking directory opening. 

#define CHECK_COPY(f, name) if (f == -1) { \
    fprintf(stderr, "Error: error in copying operation with: %s\n", name); \
    exit(EXIT_FAILURE); \
}                                                           // define macro for checking file opening 


struct File {                                               // structure which will store the name, path,  
    char *filename;                                         // modification time and permission type for each file once. 
    char *filepath; 
    time_t mtime; 
    mode_t permissions;
}; 


struct Directory {                                          // structure which will store the directory name, a pointer 
    char *dirname;                                          // to an array of File structures storing the files for that 
    struct File *files;                                     // directory, and the n. of files in that directory. 
    int numfiles; 
}; 
extern struct Directory *directories;                       // initialise array of struct Directory's to store top lvl. dir's. 
extern int ndirs; 



// directories.c
extern void process_toplevel_dir(int argc, char **argv, bool aflag, bool rflag);
extern void process_subdirs(int subdircount, char *subdirpaths[], bool aflag); 


// files.c
extern void add_file(int index, struct Directory *dirstruct, char *filename, char *fullpath, time_t mtime, mode_t mode); 
extern void sync_files(struct Directory *dirstruct); 
extern struct File *syncing_fileslist;                      // array of struct File's to store information of all the files 
extern int sync_files_count;                                // that will need to be synced. 

extern bool pflag;
extern bool nflag; 
extern bool vflag; 


// glob2regex.c 

extern char *glob2regex(char *glob); 
extern char **ignoredfiles;                                 // pointer to array of pointers to strings storing any -i patterns.
extern int sizeofignored;                                   
extern char **onlyfiles;                                    // pointer to array of pointers to strings storing any -o patterns.
extern int sizeofonly;


extern bool iflag; 
extern bool oflag; 
