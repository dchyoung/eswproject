/*
  Module: Any data conversion functions.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>

#include "parser.h"
#include "convdata.h"

//master function that calls corresponding data conversion function
void conversion(char* option, int floatDigit, char* file_out)
{
    int idx, unit_1_idx, unit_2_idx;
    char unit_1[5];
    char unit_2[5]; 
    
    
    //Parse each unit of convert from/to from option string 
    idx = 0;
    idx = parseToken(idx, option, unit_1, '-');
    parseToken(idx, unit, unit_2, '-');

    //Convert unit string to corresponding index
    unit_1_idx = get_unit_idx(unit_1);
    unit_2_idx = get_unit_idx(unit_2);
        
    if( unit_1_idx >= 0 && unit_2_idx < 6  ) {
	conv_mass(unit_1_idx, unit_2_idx, floatDigit, file_out);
    }

    //Conversion 2:
    
    //Conversion 3:

    //....
}


int get_unit_idx(char* unit)
{
    char* unit_table[6] = {"mcg", "mg", "g", "kg", "lb", "oz"};
    int i;
    
    for(i = 0; i < 6; i++) {
	if( !(strcmp(unit, unit_table[i]) )) {
	    unit_idx = i;
	    return i;
	}
    }

    return -1;
    
}



double get_convFactor_mass(int unit_1_idx, int unit_2_idx, char* unit)
{
    
    double convFactor;
    
    //Find converting factor
    //if units are SI mass units
    if(unit_1_idx < 4 && unit_2_idx < 4 ) {
	int i, unitDistance;

	unitDistance = unit_2_idx - unit_1_idx;

	if( unitDistance < 0 ) {
	    convFactor = 1;

	    for( i = 0; i < abs(unitDistance); i++ ) {
		convFactor *= 1/1000;
	    }
	    
	}
	else if( unitDistance == 0 ) {
	    convFactor = 1;
	}
	else {
	    convFactor = 1;

	    for( i = 0; i < unitDistance; i++ ) {
		convFactor *= 1000;
	    }
	}
    }

    //else if units are english mass units
    else if( unit_1_idx > 3 && unit_2_idx > 3 ) {
	if(unit_1_idx == 4 ) {
	    if(unit_2_idx == 4 ) {
		convFactor = 1;
	    }
	    else if(unit_2_idx == 5) {
		convFactor = 16;
	    }
	}
	else if(unit_1_idx == 5) { //if unit_1 is ounce
	    if(unit_2_idx == 4) {
		convFactor = 1/16;
	    }
	    else if(unit_2_idx == 5) {
		convFactor = 1;
	    }
	}
	
    }

    //else if units are english and SI unit
    else {
	unitDistance = unit_2_idx - unit_1_idx;
	
	if( unitDistance > 0 ) {
	    if( unit_1_idx == 4 ) {
		int j;
		
		convFactor = 1/2.2;

		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor /= 1000;
		}

		
	    }
	    else if( unit_1_idx == 5 ) {
		int j;
		convFactor = 1 / 35.27;

		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor /= 1000;
		}
		
	    }
	    
	}
	else if ( unitDistance < 0 ) {
	    if( unit_2_idx == 4 ) {
		int j;
		
		convFactor = 2.2;

		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor *= 1000;
		}

		
	    }
	    else if( unit_2_idx == 5 ) {
		int j;
		convFactor = 35.27;

		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor *= 1000;
		}
		
	    }
	}
    }

    return convFactor;   
    
}


//Conversion function 1: Convert kilo to gram
//For iniitial test. Needs update
void conv_mass(char* unit, int floatDigit, char* file_out)
{
    int fd;
    duoble val, convFactor;
    char word[100];
    
    //1. open file
    fd = newFile(file_out);

    //2. get conversion factor
    convFactor = get_convFactor_mass(unit);
    
    while(1) {
	int n;
	
	//2. read word
	if( readWord(word) == 0 )
	    break;

	//3. Convert string to float
	val = atof(word);
    
	//4. get conversion factor
	
	val = val * convFactor;
    
	//5. convert float to string
	
	fToStr(val, floatDigit, word);
	
	//6. Write to outputfile
	n = write(fd, word, strlen(word));

	//7. error
	if( n == -1 ) {
	    perror("write");
	    exit(errno);
	}
	
    }

    close(fd);
}

//Conversion function 2:


//Conversion function 3:

//Conversion function 4:

//Conversion function 5:

//Conversion function 6:
