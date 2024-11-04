

// files.c  

#include "mysync.h"



// copy_files()
//      receives directory paths, and 
//      for each directory, opens the 
//      directory and copies all files 
//      that need to be synced into the dir. 



void copy_files(struct Directory *dirstruct)
{
    for (int i = 0; i < ndirs; i++) {                                       // for every directory 

        DIR *dir = opendir(dirstruct[i].dirname);                           // open the directory
        CHECK_OPEN(dir, dirstruct[i].dirname);  
       
        for (int j = 0; j < sync_files_count; j++) {                        // for every up-to-date file to be synced across the directories

            char fulldestpath[MAXPATHLEN];
            sprintf(fulldestpath, "%s/%s", dirstruct[i].dirname, syncing_fileslist[j].filename ); 
            if (strcmp(fulldestpath, syncing_fileslist[j].filepath) == 0) {                 
                continue;                                                   // if source path and destination path are the same, skip copying.   
            }

            int fd0 = open(syncing_fileslist[j].filepath, O_RDONLY);        // open the up-to-date source file
            CHECK_COPY(fd0, syncing_fileslist[j].filename);                
  
            unsigned int permissions; 
            if (pflag) {                                                    
                permissions = syncing_fileslist[j].permissions;             // if -p true, set permissions same as source file.      
            }
            else {
                permissions = 0666;                                         // else, default permissions. 
            }

        
            fprintf(stdout, "gave file %s permissions: %u\n", syncing_fileslist[j].filename, permissions); 
            int fd1 = open(fulldestpath, O_WRONLY | O_CREAT | O_TRUNC, permissions);    // open destination file 
            if(fd1 == -1) {                                                             
                close(fd0);
                CHECK_COPY(fd1, dirstruct[i].dirname); 
            }

            char   buffer[BUFSIZ];                                   
            size_t got;
            while((got = read(fd0, buffer, sizeof (buffer))) > 0) {       // copy over contents of source file to destination file
                size_t put; 
                if((put = write(fd1, buffer, got)) != got ) {  
                    close(fd0); close(fd1);                               // close source and destination file 
                }
                CHECK_COPY(put, dirstruct[i].dirname);

            }   
            CHECK_COPY(got, syncing_fileslist[j].filepath); 

        }
        closedir(dir);                                                  // close the directory
    }
    return; 
}






// check_file():
//      receives file name, and checks if that 
//      file is in the list of up-to-date files. 
//      if it is, keep the one with the latest 
//      modification time, and if not, add it. 




int check_file(char *filename, time_t mtime)
{
    if (filename == NULL) {
        return -1; 
    }
    for (int i = 0; i < sync_files_count ; i++) {                           // for every file in the up-to-date, files-to-be-synced array
        if (strcmp(syncing_fileslist[i].filename , filename) == 0 ) {                                  
            if (syncing_fileslist[i].mtime < mtime ) {                      // if file is already in array, keep file w/ latest modification time.  
                return i;                                                                              
            }
            else {                                                                              
                return -1; 
            }
        }
    }
    return sync_files_count;                                                // if file is not in the array, return n. of elements so it can be added. 
}








// sync_files():
//      for every directory received as input, 
//      add each of their files to the
//      up-to-date array of files, and if file
//      already exists, keep the latest one. 





void sync_files(struct Directory *dirstruct)                                                                     
{ 
    syncing_fileslist = NULL;                                               // re-initialise array of files-to-be-synced for every successive call. 
    sync_files_count = 0; 
    for (int i = 0; i < ndirs ; i++) {                                      // for every directory, 

        for (int f = 0; f < dirstruct[i].numfiles; f++) {                   // for every file in that directory, 

            int checkfile_returnvalue = check_file(dirstruct[i].files[f].filename, dirstruct[i].files[f].mtime); //check if file in the up-to-date array of files. 

            if ( checkfile_returnvalue != -1 ) {                            // if not, file is added to up-to-date array. 
                if (checkfile_returnvalue == sync_files_count) {
    
                    syncing_fileslist = realloc(syncing_fileslist, (sync_files_count + 1) * sizeof(struct File));        
                    CHECK_ALLOC(syncing_fileslist); 
                    sync_files_count++; 
                }
                syncing_fileslist[checkfile_returnvalue].filename = dirstruct[i].files[f].filename; 
                syncing_fileslist[checkfile_returnvalue].filepath = dirstruct[i].files[f].filepath; 
                syncing_fileslist[checkfile_returnvalue].mtime = dirstruct[i].files[f].mtime;  
                syncing_fileslist[checkfile_returnvalue].permissions = dirstruct[i].files[f].permissions; 
            }
        }
    }
    if (vflag) {                                                            // if -v is true, print files to be synced. 
        fprintf(stdout, "\nfiles to be synced:\n"); 
        if (sync_files_count == 0) {
            fprintf(stdout, "[none]\n\n"); 
        }
        for (int b = 0; b < sync_files_count; b++){
            fprintf(stdout, "%s, %lu\n", syncing_fileslist[b].filename, syncing_fileslist[b].mtime); 
        }
        fprintf(stdout, "\n"); 
    }
    if (nflag) {                                                           // if -n is true, skip the copying files part. 
        return; 
    }
    
    
    
    copy_files(dirstruct);                                              
    return; 
    
}







// globbing()
//      takes input of file name and 
//      array of regular expressions (previously
//      converted from -i/-o patterns using glob2regex). 
//      After compiling and comparing, if found a match 
//      return true, else return false. 





int globbing(char *filename, char **regularexpressions, int sizeoflist) 
{

    for (int i = 0; i < sizeoflist; i++) {                                  // for every pattern (converted to reg. expression) in
        regex_t regex;                                                      // the -i or -o array, 
        int compile_check = regcomp(&regex, regularexpressions[i], 0);      // compile reg. expression using regcomp() 
        if (compile_check != 0) {
            fprintf(stderr, "Error: error compiling regex\n"); 
            exit(EXIT_FAILURE); 
        }
        int exec_result = regexec(&regex, filename, 0, NULL, 0);            // compare file to compiled expression using regexec()
        if (exec_result == 0) { 
            return 1;                                                       // if found a match, return true
        }
        else if (exec_result != REG_NOMATCH){ 
            fprintf(stderr, "Error: error executing regex\n"); 
            exit(EXIT_FAILURE); 
        }
    }
    return 0;                                                               // else, return false. 
}







// add_file():
//      compares file to -i and -o patterns, 
//      skips or keeps it accordingly, and then 
//      adds it to the directory structure provided.       



void add_file(int index, struct Directory *dirstruct, char *filename, char *fullpath, time_t mtime, mode_t mode) 
{ 

    int globreturn;                                                         
    static char **i_regularexpressions; 
    static char **o_regularexpressions; 
    bool ignore_first = false; 
    
    if (ignoredfiles != NULL) {                                             // if -i patterns were provided, 
        if (iflag) {                                                        // if first -i pattern, 
            i_regularexpressions = malloc(sizeofignored * sizeof(char*));   
            CHECK_ALLOC(i_regularexpressions);
            for (int i = 0; i < sizeofignored; i++) {
                i_regularexpressions[i] = glob2regex(ignoredfiles[i]);      // convert all -i patterns to reg. expressions
                if (i_regularexpressions[i] == NULL) {
                    fprintf(stderr, "Error: -i or -o pattern could not be processed\n"); 
                    exit(EXIT_FAILURE); 
                }
            }
            iflag = !iflag; 
        }        

        globreturn = globbing(filename, i_regularexpressions, sizeofignored); // compare each file to -i patterns
        ignore_first = true; 
        if (globreturn == 1) {                                                // if file matches -i pattern, skip it. 
            return; 
        }
    }
    if (onlyfiles != NULL && !ignore_first) {                                 // if -o patterns were provided and file was not provided as -i pattern. 
        if (oflag) {
            o_regularexpressions = malloc(sizeofonly * sizeof(char*)); 
            CHECK_ALLOC(o_regularexpressions);
            for (int i = 0; i < sizeofonly; i++) {
                o_regularexpressions[i] = glob2regex(onlyfiles[i]);           // convert all -o patterns to regular expressions first time called. 
                if (o_regularexpressions[i] == NULL) {
                    fprintf(stderr, "Error: -i or -o pattern could not be processed\n"); 
                    exit(EXIT_FAILURE); 
                }
            }
            oflag = !oflag; 
        }        
        globreturn = globbing(filename, o_regularexpressions, sizeofonly);   // compare file to -o patterns
        if (globreturn == 0) {                                               // if files does not match, skip it. 
            return; 
        }
    }
    struct File *temp_files = realloc(dirstruct[index].files, (dirstruct[index].numfiles + 1) * sizeof(struct File)); 
    CHECK_ALLOC(temp_files); 
    dirstruct[index].files = temp_files; 
    

    dirstruct[index].files[dirstruct[index].numfiles].filename = strdup(filename);                // add file to file struct in directory struct. 
    dirstruct[index].files[dirstruct[index].numfiles].filepath = strdup(fullpath); 
    dirstruct[index].files[dirstruct[index].numfiles].mtime = mtime;                              // include modification time 
    dirstruct[index].files[dirstruct[index].numfiles].permissions = mode;                         // include permission type. 

    if (vflag){ 
        fprintf(stdout, "From directory added file: %s, %lu, %u\n", dirstruct[index].files[dirstruct[index].numfiles].filename,
        dirstruct[index].files[dirstruct[index].numfiles].mtime,
        dirstruct[index].files[dirstruct[index].numfiles].permissions); 
    }
    dirstruct[index].numfiles++; 

    

}; 