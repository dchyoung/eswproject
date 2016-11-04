#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procdata.h"

/*
  proper argument format
  datamaster -(1st option) sub-option output_file_name
  datamaster -conv         ktog       data2.txt
 */

int main(int argc, char *argv[])
{
    //1. Check number of arguments
    
    if( argc < 4 ){
	printf("Invalid command format.");
	exit(0);
    }
        
    //2. Get command option
    if( !(strcmp(argv[1], "-conv")) )
    {
	//1. Call conversion module with suboptions and  output filename
	conversion(argv[2], argv[3]);
	
    }

    return 0;
}
