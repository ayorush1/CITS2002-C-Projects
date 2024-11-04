

// This is the directories.c source file. 
// The purpose of this source file is to 
// - receive top-level directories
// - open and read each directory 
// - pass any regular files to file.c 
// - recursively search directories if -r . 



#include "mysync.h" 



// check_empty_directory(): 
//      checks if a given subdirectory exists in 
//      any of the relative parent directories 
//      and if so whether it contains any contents.  


bool check_empty_directory(char *subdirpaths[]) {

    for (int i =0; i < ndirs; i++) {                            

        if (access(subdirpaths[i], F_OK) == 0) {                // check whether subdirectory exists or not 
            DIR             *emptydirstream; 
            struct dirent   *emptydp; 
            bool empty; 
            emptydirstream = opendir(subdirpaths[i]);           // open subdirectory 
            CHECK_OPEN(emptydirstream, subdirpaths[i]); 

            while ((emptydp = readdir(emptydirstream)) != NULL) {   // check for any contents in the directory    
                if (strcmp(emptydp->d_name, ".") != 0 && strcmp(emptydp->d_name, "..") != 0) { 
                    empty = false;                                                                                                            
                    break;                                      // if there exists contents, return false.                   
                }
            }
            closedir(emptydirstream);
            return empty;
        }
    }
    return 0; 
}










// open_directory(): 
//      open a directory, and if successful, 
//      store its name and create file struct. 
//      to store its files. 

 

void open_directory(int index, DIR **dstream, char *directorypathnames[], struct Directory *dirstruct)
{

    *dstream = opendir(directorypathnames[index]);              // open the given directory 
    CHECK_OPEN(dstream, directorypathnames[index]);             // check if opened succesfully 

    dirstruct[index].dirname = strdup(directorypathnames[index]); 
    dirstruct[index].files = malloc( sizeof(struct File) );     // allocate memory for atleast one file in directory

    CHECK_ALLOC(dirstruct[index].files);                        

    return; 
}










// process_single_dir():
//      iterates through contents of a directory, 
//      passing any regular files to the files.c 
//      source file and any sub-directories 
//      to the process_subdir() function. 



void process_single_dir(DIR *dstream, struct Directory *dirstruct, char *dirpathnames[], int index, bool aflag, bool rflag, int numofdir)
{
    struct dirent   *dp;                                        

    while ( (dp = readdir(dstream)) != NULL) {                              // iterate through contents of directory.     
        struct stat stat_buffer; 
        char fullpath[MAXPATHLEN];  
        sprintf(fullpath, "%s/%s", dirpathnames[index], dp->d_name);        // create path for entity in directory
        
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
            continue;                                                       // skip '.' and '..' entities. 
        }
        if ( aflag == false) {                                              // skip '.' files unless -a is true. 
            if (dp->d_name[0] == '.') {
                continue;
            }
        }

        if ( stat(fullpath, &stat_buffer) != 0 ) {                          // obtain files attributes and check for errors.
            fprintf(stderr, "could not obtain file '%s' attributes\n", dp->d_name);        
            exit(EXIT_FAILURE); 
        }
        else if (S_ISREG(stat_buffer.st_mode)) {                            // if regular file, pass file to files.c source file. 
            add_file(index, dirstruct, dp->d_name, fullpath, stat_buffer.st_mtime, stat_buffer.st_mode);  
        }
        else if (S_ISDIR(stat_buffer.st_mode) && rflag) {                   // if directory and -r is true        
            char *subdirpaths[numofdir];                                    
            if (vflag){
                fprintf(stdout, "found subdirectory: %s\n", dp->d_name); 
            }

            for (int l = 0; l < numofdir; l++) {                            // create paths to subdirectory from each top directory. 
                char subdirfullpath[MAXPATHLEN]; 
                sprintf(subdirfullpath, "%s/%s", dirpathnames[l], dp->d_name); 
                subdirpaths[l] = strdup(subdirfullpath);                     
            }
            
            process_subdirs(numofdir, subdirpaths, aflag);                  // pass array of paths to process_subdir() function. 
            if (vflag){
                fprintf(stdout, "subdirectory '%s' processed\n", dp->d_name); 
                fprintf(stdout, "back to parent directory \n\n"); 
            }
        }
    }
}






// process_subdirs(): 
//      receive a subdirectory, create that subdirectory 
//      in any parent directory where not exists, and 
//      populate a directory structure for that subdirectory. 
//      Then, sync the subdirectories. 




void process_subdirs(int subdircount, char *subdirpaths[], bool aflag) 
{
    struct Directory *subdirectories = malloc(subdircount * sizeof(struct Directory));  
    CHECK_ALLOC(subdirectories)   

    if (check_empty_directory(subdirpaths)) {                               // check if subdirectory empty, and ignore if so. 
        return; 
    }

    for (int k = 0; k < subdircount; k++ ) {                                // for every parent directory, 
        if (access(subdirpaths[k], F_OK) != 0) {                            // check if the subdirectory exists 
            fprintf(stdout, "\n"); 
            fprintf(stdout, "creating subdirectory: %s\n", subdirpaths[k]); 
            if (mkdir(subdirpaths[k], 0777) != 0) {                         // if it does not, create the subdirectory. 
                fprintf(stderr, "Error: could not make directory: %s\n", subdirpaths[k]); 
            }
        } 
    }
    for (int i = 0; i < subdircount; i++) {                                 // now iterate through those subdirectories. 
        DIR             *subdirstream = NULL; 
        open_directory(i, &subdirstream, subdirpaths, subdirectories);      // open subdirectory 
        if (vflag){
            fprintf(stdout, "\nprocessing subdirectory: %s\n\n", subdirectories[i].dirname);
        }
        
        process_single_dir(subdirstream, subdirectories, subdirpaths, i, aflag, true, subdircount); // process subdirectory
        if (vflag){
            fprintf(stdout, "finished processing subdirectory: %s\n", subdirectories[i].dirname);
        }
        closedir(subdirstream); 
    }

    sync_files(subdirectories);                                             // now sync the subdirectory among all the parent directories. 


    return;  

}






// process_toplevel_dir(): 
//      receives top-lvl directories, and 
//      for each of them, opens and processes
//      them. Then, syncs all the directories. 




void process_toplevel_dir(int argc, char **argv, bool aflag, bool rflag)
{
    ndirs = argc;                                                           
    struct Directory *directories = malloc(ndirs * sizeof(struct Directory));      
    CHECK_ALLOC(directories);                                               // create array of directory structures for each top-lvl directory 

    for (int i = 0; i < argc; i++) {                                        // for each top-lvl directory
        DIR             *dirstream = NULL; 
        open_directory(i, &dirstream, argv, directories);                   // open directory 
        if (vflag){
            fprintf(stdout, "Processing Parent Directory: %s\n\n", argv[i]);
        }
        process_single_dir(dirstream, directories, argv, i, aflag, rflag, argc);    // process directory 
        closedir(dirstream);                                                        // close directory 
    }
    sync_files(directories);                                                // then, sync all directories. 


    return; 
}



