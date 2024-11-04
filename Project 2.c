

/* CITS2002 Systems Programming - Project 2 2023  */

/* Arush Kathal 23730792  */

#include "mysync.h"
#include <stdlib.h>
#include <stdio.h> 


int main (int argc, char *argv[]) 
{ 

    typedef struct listtype {
        char devicename[20] ; 
        char *string; 
        struct listtype *next; 

    } LIST; 

    LIST struccy = {"keyboard", "ytes", NULL}; 

    return 0; 

}





                subdirectories[0].dirname = strdup(dp->d_name); 
                subdirectories[0].files = malloc( sizeof(struct File) );                             // allocate memory for atleast one file in directory

                if ( directories[i].files == NULL ) {                                                     
                    fprintf(stderr, "cannot allocate memory\n");  
                    exit(EXIT_FAILURE); 
                }

                subdirectories[1].dirname = strdup(dp->d_name); 
                subdirectories[1].files = malloc( sizeof(struct File) );                             // allocate memory for atleast one file in directory

                char subdir1fullpath[MAXPATHLEN]; 
                char subdir2fullpath[MAXPATHLEN]; 

                subdir1fullpath = strdup(fullpath); 
                sprintf(subdir2fullpath, "%s/%s", argv[1], dp->d_name); 

                
                process_subdirs(subdir1fullpath, subdir2fullpath, subdirectories); 










Hi, 

I had just had 2 quick questions about the 2nd project: 

1. Since we are exiting mysync whenever memory cannot be allocated, is it then not necessary to create a temporary variable when reallocating more memory? For example, currently I have: 

    temp_datastructure = realloc(main_datastructure, ...) 
    if (temp_datastructure) == NULL {
    ...
    }
    main_datastructure = temp_datastructure.

However should I change this to: 

    main_datastructure = realloc(main_datastructure, ...) 
    if (main_datastructure == NULL) {
    ...
    }


