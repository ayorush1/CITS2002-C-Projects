
#include "mysync.h"



void process_single_dir(struct dirent *dp, DIR *dstream, struct Directory *dirstruct, char *dirpathnames[], int index, bool aflag, bool rflag, int numofdir)
{

    while ( (dp = readdir(dstream)) != NULL) {                    // break into another function maybe 
                
        struct stat stat_buffer; 
        char fullpath[MAXPATHLEN]; 
        
        sprintf(fullpath, "%s/%s", dirpathnames[index], dp->d_name); 
        

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
            continue; 
        }

        if ( aflag == false) {
            if (dp->d_name[0] == '.') {
                continue;
            }
        }

        if ( stat(fullpath, &stat_buffer) != 0 ) {

            fprintf(stderr, "could not obtain file '%s' attributes\n", dp->d_name);        
            exit(EXIT_FAILURE); 
        }
        
        else if (S_ISREG(stat_buffer.st_mode)) {
        
            
            add_file(index, dirstruct, dp->d_name, fullpath, stat_buffer.st_mtime, stat_buffer.st_mode);   // need to error check mtime + st_mode (that there is valid output)?

            
        }
        else if (S_ISDIR(stat_buffer.st_mode) && rflag) {

                            
            char *subdirpaths[numofdir];
            fprintf(stdout, "found subdirectory: %s\n", dp->d_name); 
            
            for (int l = 0; l < numofdir; l++) {
                
                
                char subdirfullpath[MAXPATHLEN]; 
                sprintf(subdirfullpath, "%s/%s", dirpathnames[l], dp->d_name); 


                subdirpaths[l] = strdup(subdirfullpath);                        // copy each parent directory's pathname

                

            }
            

            process_subdirs(dirpathnames, subdirpaths, aflag); 
            fprintf(stdout, "subdirectory '%s' processed\n", dp->d_name); 
            fprintf(stdout, "back to parent directory \n\n"); 
        }
    }
}















while ( (subdp = readdir(subdirstream)) != NULL) {                    // break into another function maybe 
            
            struct stat stat_buffer2; 
            char fullpath[MAXPATHLEN]; 
            
            sprintf(fullpath, "%s/%s", subdirpaths[i], subdp->d_name); 
          

            if (strcmp(subdp->d_name, ".") == 0 || strcmp(subdp->d_name, "..") == 0){
                continue; 
            }

            if ( aflag == false) {
                if (subdp->d_name[0] == '.') {
                    continue;
                }
            }

            if ( stat(fullpath, &stat_buffer2) != 0 ) {

                fprintf(stderr, "could not obtain file '%s' attributes\n", subdp->d_name);         // exit or not? 
                exit(EXIT_FAILURE); 
            }
            
            else if (S_ISREG(stat_buffer2.st_mode)) {
                
                add_file(i, subdirectories, subdp->d_name, fullpath, stat_buffer2.st_mtime, stat_buffer2.st_mode);   // need to error check mtime + st_mode (that there is valid output)?

                
            }
            else if (S_ISDIR(stat_buffer2.st_mode)) {

                fprintf(stdout, "found subdirectory: %s\n", subdp->d_name);
                char *subdirpaths2[subdircount];
                
                for (int l = 0; l < subdircount; l++) {
                    
                    char subdirfullpath[MAXPATHLEN]; 
                    sprintf(subdirfullpath, "%s/%s", subdirpaths[l], subdp->d_name); 
                    subdirpaths2[l] = strdup(subdirfullpath);                        // copy each parent directory's pathname


                }

                process_subdirs(subdircount, subdirpaths2, aflag); 
                fprintf(stdout, "subdirectory '%s' processed\n\n\n", subdp->d_name); 

            }

        }
        fprintf(stdout, "finished processing subdirectory: %s\n", subdirectories[i].dirname);
        closedir(subdirstream); 

    




while ( (dp = readdir(dirstream)) != NULL) {                    // break into another function maybe 
            
            struct stat stat_buffer; 
            char fullpath[MAXPATHLEN]; 
            
            sprintf(fullpath, "%s/%s", argv[i], dp->d_name); 
          

            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
                continue; 
            }

            if ( aflag == false) {
                if (dp->d_name[0] == '.') {
                    continue;
                }
            }

            if ( stat(fullpath, &stat_buffer) != 0 ) {

                fprintf(stderr, "could not obtain file '%s' attributes\n", dp->d_name);        
                exit(EXIT_FAILURE); 
            }
            
            else if (S_ISREG(stat_buffer.st_mode)) {
            
                
                add_file(i, directories, dp->d_name, fullpath, stat_buffer.st_mtime, stat_buffer.st_mode);   // need to error check mtime + st_mode (that there is valid output)?

                
            }
            else if (S_ISDIR(stat_buffer.st_mode) && rflag) {

                                
                char *subdirpaths[argc];
                fprintf(stdout, "found subdirectory: %s\n", dp->d_name); 
                
                for (int l = 0; l < argc; l++) {
                    
                    
                    char subdirfullpath[MAXPATHLEN]; 
                    sprintf(subdirfullpath, "%s/%s", argv[l], dp->d_name); 


                    subdirpaths[l] = strdup(subdirfullpath);                        // copy each parent directory's pathname

                    

                }
                

                process_subdirs(argc, subdirpaths, aflag); 
                fprintf(stdout, "subdirectory '%s' processed\n", dp->d_name); 
                fprintf(stdout, "back to parent directory \n\n"); 
            }
        }