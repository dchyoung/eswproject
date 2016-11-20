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

//Master function that calls corresponding data conversion function
void conversion(char* option, int floatDigit, char* file_out)
{
    int idx = 0, unit_1_idx, unit_2_idx;
    char unit_1[5];
    char unit_2[5]; 
    
    //Parse each unit of convert from/to from option string 
    idx = parseToken(idx, option, unit_1, '-');
    parseToken(idx, option, unit_2, '-');

    //Convert unit string to corresponding unit index
    unit_1_idx = get_unit_idx(unit_1);
    unit_2_idx = get_unit_idx(unit_2);
    
    //Unit index between 0 and 5: mass unit    
    if( unit_1_idx >= 0 && unit_2_idx < 6  ) {
	//Call mass conversion
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
	    return i;
	}
    }

    return -1;
    
}



double get_convFactor_mass(int unit_1_idx, int unit_2_idx)
{
    int i, unitDistance;
    double convFactor;

    //Conversion Between SI Units
      //Unit number between 0 and 4: SI mass unit
    if( (unit_1_idx < 4) && (unit_2_idx < 4) ) {

	unitDistance = unit_2_idx - unit_1_idx;

	//Conversion Factor = 1000^(unitDistance)
	if( unitDistance < 0 ) {
	    convFactor = 1;
	    
	    for( i = 0; i < abs(unitDistance); i++ ) {
		convFactor = convFactor * 1000;
	    }
	}

	//No Scale
	else if( unitDistance == 0 ) {
	    convFactor = 1;
	}

	//Conversion Factor = (1/1000)^(unitDistance)
	else {
	    convFactor = 1;

	    for( i = 0; i < unitDistance; i++ ) {
		convFactor = convFactor / 1000;
	    }
	}
    }

    //Conversion Between English units (unit number 4 to 5)
    else if( unit_1_idx > 3 && unit_2_idx > 3 ) {

	//Convert from pound to
	if(unit_1_idx == 4 ) {

	    if(unit_2_idx == 4 ) {
		convFactor = 1;
	    }

	    //To ounce: x16
	    else if(unit_2_idx == 5) {
		convFactor = 16;
	    }
	}
	//Convert from Ounce to 
	else if(unit_1_idx == 5) { 

	    //To pound: x(1/16)
	    if(unit_2_idx == 4) {
		convFactor = 1/16;
	    }
	    
	    else if(unit_2_idx == 5) {
		convFactor = 1;
	    }
	}
	
    }

    //Conversion between English and SI unit
    else {
	unitDistance = unit_2_idx - unit_1_idx;

	//Case1: Conversion from English to SI
	if( unitDistance > 0 ) {

	    //if converting from pound
	    if( unit_1_idx == 4 ) {
		int j;

		//First Convert pound to Kilogram
		convFactor = 1/2.2;

		//Adjust scale for SI mass units
		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor = convFactor /  1000;
		}	
	    }

	    //if converting from ounce
	    else if( unit_1_idx == 5 ) {
		int j;

		//First convert ounce to kilogram
		convFactor = 1 / 35.27;

		//Adjust scale for SI mass units
		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor = convFactor /  1000;
		}		
	    }	    
	}
	//Case2: Conversion from SI to English
	else if ( unitDistance < 0 ) {

	    //If converting to pound
	    if( unit_2_idx == 4 ) {
		int j;

		//Fisrt Convert to pound
		convFactor = 2.2;

		//Adjust scale for SI units
		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor = convFactor * 1000;
		}	
	    }

	    //If converting to ounce
	    else if( unit_2_idx == 5 ) {
		int j;
		
		//First convert to ounce
		convFactor = 35.27;

		//Adjust scale for SI units
		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor = convFactor * 1000;
		}		
	    }
	}
    }

    return convFactor;    
}


//Conversion function 1: mass unit conversion
void conv_mass(int unit_1_idx, int unit_2_idx, int floatDigit, char* file_out)
{
    int fd;
    double val, convFactor;
    char word[BUFLEN];
    
    //1. open file to write
    fd = newFile(file_out);

    //2. get conversion scale factor between units
    convFactor = get_convFactor_mass(unit_1_idx, unit_2_idx);

    //3. Read word by word, converting
    while(1) {
	int n;
	
	//read word
	if( readWord(word, STDIN_FILENO) == 0 )
	    break;

	//Convert string to float
	val = atof(word);
    
	//Scale the value
	val = val * convFactor;
    
	//Convert float to string
	fToStr(val, floatDigit, word);
	
	//Write to outputfile
	n = write(fd, word, strlen(word));

	//write error
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
