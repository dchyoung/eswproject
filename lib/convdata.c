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
    double convFactor;
    
    //Parse each unit of convert from/to from option string
    //Each unit is seperated by '-' character
    idx = parseToken(idx, option, unit_1, '-');
    parseToken(idx, option, unit_2, '-');

    //Convert unit string to corresponding unit index
    unit_1_idx = get_unit_idx(unit_1);
    unit_2_idx = get_unit_idx(unit_2);
    
    //Conversion 1: Unit index between 0 and 5: Conventional mass unit    
    if( (unit_1_idx >= 0 && unit_1_idx < 6) &&
	(unit_2_idx >= 0 && unit_2_idx < 6) )
    {	
	//Get conversion scale factor for the units
	convFactor = get_convFactor_mass(unit_1_idx, unit_2_idx);
	
	//Call mass conversion
	_convert(convFactor, floatDigit, file_out);
    }

    //Conversion 2: Unit Index number between 6 and 22: Metric Unit Conversion
    else if( (unit_1_idx >= 6 && unit_1_idx < 23) &&
	(unit_2_idx >= 6 && unit_2_idx < 23) )
    {
	//Get conversion scale factor for the units
	convFactor = get_convFactor_metric(unit_1_idx, unit_2_idx);
	_convert(convFactor, floatDigit, file_out);
    }
    
    //Conversion 3: Length Unit: 23 - 30
    else if( (unit_1_idx >= 23 && unit_1_idx < 30) &&
	(unit_2_idx >= 23 && unit_2_idx < 30) )
    {
	//Get conversion scale factor for the units
	convFactor = get_convFactor_metric(unit_1_idx, unit_2_idx);
	_convert(convFactor, floatDigit, file_out);
    }
    //Conversion 4:

    //....
    else {
	printf("Invalid Unit.\n");
	exit(0);
    }
}


int get_unit_idx(char* unit)
{
    char* unit_table[NUM_OF_UNITS] = {
	//Conventional Mass Unit (0 ~ 5)
	"mcg", "mg", "g", "kg", "lb", "oz",
	//Metric Unit (6 ~ 22)
        "atto", "femto", "pico", "nano", "micro", "milli", "centi", "deci",
	"none", "deca", "hecto", "kilo", "mega", "giga", "tera", "peta", "exa",
	//Length Unit (23 ~ 30)
	"mm", "cm", "m", "km", "in", "ft", "yd", "mile" 
    };
    
    int i;
    
    for(i = 0; i < NUM_OF_UNITS; i++) {
	if( !(strcmp(unit, unit_table[i]) )) {
	    return i;
	}
    }

    return -1;
    
}

void _convert(double convFactor, int floatDigit, char* file_out)
{
    int fd;
    double val;
    char word[BUFLEN];
    
    //1. open file to write
    fd = newFile(file_out);

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

//Conversion 1: Conventional mass unit conversion
//Input: unit number of mass units, Output: Scale factor between the units.
double get_convFactor_mass(int unit_1_idx, int unit_2_idx)
{
    int i, unitDistance;
    double convFactor;

    //Conversion Between SI Units
      //Unit number between 0 and 4: SI mass unit
    if( (unit_1_idx < LB) && (unit_2_idx < LB) ) {

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
    else if( unit_1_idx > KG && unit_2_idx > KG ) {

	//Convert from pound to
	if(unit_1_idx == LB ) {

	    if(unit_2_idx == LB ) {
		convFactor = 1;
	    }

	    //To ounce: x16
	    else if(unit_2_idx == OZ) {
		convFactor = 16;
	    }
	}
	//Convert from Ounce to 
	else if(unit_1_idx == OZ) { 

	    //To pound: x(1/16)
	    if(unit_2_idx == LB) {
		convFactor = 1/16;
	    }
	    
	    else if(unit_2_idx == OZ) {
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
	    if( unit_1_idx == LB ) {
		int j;

		//First Convert pound to Kilogram
		convFactor = 1/2.2;

		//Adjust scale for SI mass units
		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor = convFactor /  1000;
		}	
	    }

	    //if converting from ounce
	    else if( unit_1_idx == OZ ) {
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
	    if( unit_2_idx == LB ) {
		int j;

		//Fisrt Convert to pound
		convFactor = 2.2;

		//Adjust scale for SI units
		for(j = 0; j < unitDistance - 1; j++) {
		    convFactor = convFactor * 1000;
		}	
	    }

	    //If converting to ounce
	    else if( unit_2_idx == OZ ) {
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

//Conversion 2: Metric units
double get_convFactor_metric(int unit_1_idx, int unit_2_idx)
{
    int i, unitDistance, correction = 0;
    double convFactor = 1;

    unitDistance = unit_2_idx - unit_1_idx;

    //Up conversion
    if( unitDistance > 0 ) {
	//convFactor assumed to be every cubic

	for( i = 0; i < unitDistance; i++ ) {
	    convFactor = convFactor / 1000;
	}

	//correction scale factor
	for( i = unit_1_idx + 1; i <= unit_2_idx; i++) {
	    if( i >= CENTI && i <= KILO ) {
		correction++;
	    }
	}
	
	for( i = 0; i < correction; i++) {
	    convFactor = convFactor * 100;
	}
     }
    
    //Down conversion
    else if( unitDistance < 0 ) {

	//convFactor assumed to be every cubic
	for( i = 0; i < abs(unitDistance); i++ ) {
	    convFactor = convFactor * 1000;
	}

	//correction scale factor
	for( i = unit_1_idx - 1; i >= unit_2_idx; i--) {
	    if( i >= 11 && i <= 16 ) {
		correction++;
	    }
	}
	
	for( i = 0; i < correction; i++) {
	    convFactor = convFactor / 100;
	}
    }
   

    return convFactor;    
}


//Conversion function 3: Length, Unit Number between (23 ~ 30)
double get_convFactor_length_SI(int unit_1_idx, int unit_2_idx)
{
    double convFactor = 1;
    int unitDistance;

    unitDistance = unit_2_idx - unit_1_idx;

    if( unitDistance > 0 ) {
	if( unit_1_idx == MM ) {
	    if( unitDistance == 1 ) 
		convFactor = 1/10;
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1 / (10 * 100);
	    
	    else if( unitDistance == 3 ) 
		convFactor = 1 / (10 * 100 * 1000);
	    
	}

	else if( unit_1_idx == CM ) {
	    if( unitDistance == 1 ) 
		convFactor = 1/100;
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1 / (100 * 1000);
	    
	}

	else if( unit_1_idx == M ) 
	    convFactor = 1/1000;
	
		
    }
    else if( unitDistance < 0 ) {
	if( unit_1_idx == KM ) {
	    if( abs(unitDistance) == 1 ) 
		convFactor = 1000;
	    
	    else if( abs(unitDistance) == 2 ) 
		convFactor = 100 * 1000;
	    
	    else if( abs(unitDistance) == 3 ) 
		convFactor = 10 * 100 * 1000;
	    
	}

	else if( unit_1_idx == M ) {
	    if( abs(unitDistance) == 1 ) 
		convFactor = 100;
	    
	    else if( abs(unitDistance) == 2 ) 
		convFactor = 10 * 100;
	    
	}
        else if( unit_1_idx == CM ) 
	    convFactor = 10;	
    }

    return convFactor;
}
double get_convFactor_length_imperial(int unit_1_idx, int unit_2_idx)
{
    double convFactor = 1;
    int unitDistance;

    
    unitDistance = unit_2_idx - unit_1_idx;

    if( unitDistance > 0 ) {
	if( unit_1_idx == IN ) {
	    if( unitDistance == 1 ) 
		convFactor = 1 / 12;
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1 / (12 * 3);
	    
	    else if( unitDistance == 3 ) 
		convFactor = 1 / (12 * 3 * 1760);
	    
	}

	else if( unit_1_idx == FT ) {
	    if( unitDistance == 1 ) 
		convFactor = 1/3;
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1 / (3 * 1760);
	    
	}

	else if( unit_1_idx == YD ) 
	    convFactor = 1/1760;
	
		
    }
    else if( unitDistance < 0 ) {
	if( unit_1_idx == MILE ) {
	    if( abs(unitDistance) == 1 ) 
		convFactor = 1760;
	    
	    else if( abs(unitDistance) == 2 ) 
		convFactor = 3 * 1760;
	    
	    else if( abs(unitDistance) == 3 ) 
		convFactor = 12 * 3 * 1760;
	    
	}

	else if( unit_1_idx == YD ) {
	    if( abs(unitDistance) == 1 ) 
		convFactor = 3;
	    
	    else if( abs(unitDistance) == 2 ) 
		convFactor = 12 * 3;
	    
	}
        else if( unit_1_idx == FT ) 
	    convFactor = 12;	
    }

    return convFactor;
}

double get_convFactor_length(int unit_1_idx, int unit_2_idx)
{
    double convFactor = 1;

    
    //case 1: conversion between SI units
    if( (unit_1_idx >= MM && unit_1_idx <= KM) &&
	(unit_2_idx >= MM && unit_2_idx <= KM) )
    {
	convFactor = get_convFactor_length_SI(unit_1_idx, unit_2_idx);
    }

    //case 2: Conversion between Imperial units
    else if( (unit_1_idx >= IN && unit_1_idx <= MILE) &&
	(unit_2_idx >= IN && unit_2_idx <= MILE) )
    {
	convFactor = get_convFactor_length_imperial(unit_1_idx, unit_2_idx);
    }
    
    //case 3: conversion between SI and Imperial
    else
    {
	//SI to imperial
	if(unit_1_idx >= MM && unit_1_idx <= KM) {
	    //change unit 1 to corresponding impeirial
	    switch (unit_1_idx) {
		case MM:
		    convFactor = get_convFactor_length_imperial(IN, unit_2_idx);
		    convFactor = convFactor / 25.4;
		    break;
		case CM:
		    convFactor = get_convFactor_length_imperial(IN, unit_2_idx);
		    convFactor = convFactor / 2.54;
		    break;
		case M:
		    convFactor = get_convFactor_length_imperial(YD, unit_2_idx);
		    convFactor = convFactor * 1.09 ;
		    break;
		case KM:
		    convFactor = get_convFactor_length_imperial(MILE, unit_2_idx);
		    convFactor = convFactor / 1.6;
		    break;
	        default:
		    break;
	    }

	}
	
	//imperial to SI
	else {
	    //change unit 1 to corresponding si
	   switch (unit_1_idx) {
	        case IN: //inch
		    convFactor = get_convFactor_length_SI(CM, unit_2_idx);
		    convFactor = convFactor * 2.54;
		    break;
	        case FT: //ft
		    convFactor = get_convFactor_length_SI(CM, unit_2_idx);
		    convFactor = convFactor * 30.48;
		    break;
	        case YD: //yd
		    convFactor = get_convFactor_length_SI(M, unit_2_idx);
		    convFactor = convFactor / 1.09 ;
		    break;
	        case MILE: //mile
		    convFactor = get_convFactor_length_SI(KM, unit_2_idx);
		    convFactor = convFactor * 1.6;
		    break;
	        default:
		    break;
	    }
	}
    }

    return convFactor;   
}

//Conversion function 4:

//Conversion function 5:
