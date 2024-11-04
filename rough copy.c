// rough work 




/* working with file and directory data structures help */


rough work 



struct Directory *temp_directories = realloc(directories, ndirs * sizeof(struct Directory)); 

    if ( temp_directories == NULL ) {                                                      // memory error handling
        fprintf(stderr, "cannot allocate memory\n");  
        exit(EXIT_FAILURE); 
    }
    directories = temp_directories; 








#include "mysyncheaderfile.h"
#include <stdio.h>
#include <stdlib.h>

struct File {
    char *filename;
    unsigned int modiftime;
};

struct Directory {
    char *dirname;
    struct File *files; // Pointer to an array of files in the directory
    size_t num_files;   // Number of files in the directory
};

int main() {
    // Example directory structure
    struct File files_in_dir1[] = {
        {"file1.txt", 12345},
        {"file2.txt", 67890},
    };

    struct File files_in_dir2[] = {
        {"file3.txt", 11111},
    };

    struct Directory directories[] = {
        {"dir1", NULL, 0}, // Initialize with empty files array
        {"dir2", NULL, 0}, // Initialize with empty files array
    };

    // Dynamically allocate memory for files in each directory
    directories[0].files = malloc(sizeof(files_in_dir1));
    directories[0].num_files = sizeof(files_in_dir1) / sizeof(files_in_dir1[0]);
    directories[0].files = files_in_dir1;

    directories[1].files = malloc(sizeof(files_in_dir2));
    directories[1].num_files = sizeof(files_in_dir2) / sizeof(files_in_dir2[0]);
    directories[1].files = files_in_dir2;

    // Accessing the data
    for (size_t i = 0; i < sizeof(directories) / sizeof(directories[0]); i++) {
        printf("Directory: %s\n", directories[i].dirname);
        printf("Number of Files: %zu\n", directories[i].num_files);
    }

    // Free dynamically allocated memory
    free(directories[0].files);
    free(directories[1].files);

    return 0;
}








Printing -i and -o arguments: 

fprintf(stdout, "-i arguments received: ");
    for (int i = 0; i < sizeofignored; i++) {
        fprintf(stdout, "%s, ", ignoredfiles[i]); 
    }
fprintf(stdout, "\n"); 
    
fprintf(stdout, "-o arguments received: ");
    for (int i = 0; i < sizeofonly; i++) {
        fprintf(stdout, "%s, ", onlyfiles[i]); 
    }
fprintf(stdout, "\n"); 



Printing argc and argv afterwards 

fprintf(stdout, "%i\n", argc); 

    for (int i = 0; i < argc; i++) {

        fprintf(stdout, "%s\n", argv[i]);
    }















. and .. directory thingies: 


 //if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
            //    continue; 
            //}
            





Permissions p flag + chmod


            if (pflag) {
                if (chmod(fulldestpath, syncing_fileslist[j].permissions) != 0) {
                    fprintf(stderr, "Error: could not update file permissions\n"); 
                    exit(EXIT_FAILURE); 
                }
                fprintf(stdout, "gave file %s permissions: %u\n", syncing_fileslist[j].filename, syncing_fileslist[j].permissions); 
            }
            else {
                if (chmod(fulldestpath, 0666) != 0) {
                    fprintf(stderr, "Error: could not update file permissions\n"); 
                    exit(EXIT_FAILURE); 
                }
                fprintf(stdout, "gave file %s permissions: 0666 \n", syncing_fileslist[j].filename); 
            }