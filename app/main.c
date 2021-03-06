#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "convdata.h"
#include "compdata.h"

/*
 * Data conversion / computation processing program.

  -Proper argument format examples:
   >>datamaster -(1st option) sub-option output_file_name

   Conversions:
   >>datamaster -conv         k-g        numOfDigits     dataoutput.txt
   >>datamaster -conv         nano-centi numOfDigits     dataoutput.txt

   Computations:
   >>datamaster -comp         mean       numOfDigits     dataoutput.txt  
   >>datamster  -comp         prob       numOfDigits     dataoutput.txt
   >>datamaster -comp         hist-10    numOfDigits     dataoutput.txt
 */


int main(int argc, char *argv[])
{
     
    //1. Check number of arguments
    
    if( argc < 4 ){
	fprintf(stderr, "Invalid Command Format.");
	exit(0);
    }
        
    //Get the first option of the command: data conversion or computation
    //If 1st option is to convert
    if( !(strcmp(argv[1], "-conv")) ) {
	//Call conversion module with suboptions and  output filename
	if( argc == 5 ) {
	    
	    int floatDigit;            //number of digits after the point

	    floatDigit = atoi(argv[3]);

	    conversion(argv[2], floatDigit, argv[4]);
	    
	}
	else {
	    conversion(argv[2], 2, argv[3]);//default 2 digits after the point
	}
	
    }

  
    //Else 1st option is to compute
    else if( !(strcmp(argv[1], "-comp")) ) {
	//Call computation module with suboptions and output filename
	if( argc == 5 ) {
	    int floatDigit;

	    floatDigit = atoi(argv[3]);
	    
	    computation(argv[2], floatDigit, argv[4]);
	}
	else
	    computation(argv[2], 2, argv[3]); 
    }
    

    //Else invalid command format
    else {
	fprintf(stderr, "Invalid Command Format.\n");
	exit(0);
    }

    

    return 0;
}
