
// mysync.c

#include "mysync.h"




// usage(): called after program usage errors, reports the program's synopsis,
//          returns void. 

void usage(void)                                                                        
{ 
    fprintf(stderr, " incorrect usage of mysync.\n"); 
    fprintf(stderr, " Usage: mysync [options] dir1 dir2 [dir3 ...]\n"); 
    fprintf(stderr, " options include: \n"); 
    fprintf(stderr, "   -a              synchronise all files\n"); 
    fprintf(stderr, "   -i pattern      ignore files matching specified pattern\n"); 
    fprintf(stderr, "   -n              report actions that would be performed but do not synchronise\n"); 
    fprintf(stderr, "   -o pattern      only synchronise files matching the pattern\n"); 
    fprintf(stderr, "   -p              preserve modification time and protection modes of files\n"); 
    fprintf(stderr, "   -r              recursively synchronises all directories\n"); 
    fprintf(stderr, "   -v              be verbose and report actions\n"); 
    fprintf(stderr, "\n\n"); 

    return; 
}



// main(): receives command-line arguments, and validates + stores 
//         any options input. 


int main( int argc, char *argv[]) 
{
                                                            
    int opt;                                                

    bool aflag = false;                                     // declare options as false initially
    bool rflag = false;

    opterr = 0;                                             // set error reporting by getopt() to false. 


    while( ( opt = getopt(argc, argv, OPTLIST)) != -1 ) {   // parse through (any) options given. 

        switch(opt){ 

            case 'a': 
                aflag = true;                               
                break; 

            case 'i': 
                iflag = true;                               // make iflag true and store -i pattern given. 
                sizeofignored++;
                ignoredfiles = realloc(ignoredfiles, sizeofignored * sizeof(char*));       
                CHECK_ALLOC(ignoredfiles);               
                ignoredfiles[sizeofignored - 1] = strdup(optarg);                                  
                break; 
            
            case 'n': 
                vflag = true; 
                nflag = true;
                break; 

            case 'o': 
                oflag = true;                               // make oflag true and store -o pattern given. 
                sizeofonly++; 
                onlyfiles = realloc(onlyfiles, sizeofonly * sizeof(char*));                
                CHECK_ALLOC(onlyfiles); 
                onlyfiles[sizeofonly - 1] = strdup(optarg); 
                break; 

            case 'p': 
                pflag = true;
                break; 

            case 'r': 
                rflag = true;
                break; 

            case 'v': 
                vflag = true;
                break; 

            default: 
                argc = -1;                                  // set argc to -1 if command-line option not recognised. 
                break; 
                
        }
    }
    
    
    argc  -= optind;                                        // set argc to the number of non-option arguments received
    argv  += optind;                                        // set argv to point to the first non-option argument. 

    if ( argc <= 1 ) {                                      // check if argc set to -1 or if less than 2 directories given. 
        
        usage(); 
        exit(EXIT_FAILURE);
    }
  
    


    process_toplevel_dir(argc, argv, aflag, rflag);        





    return 0; 

} 



































