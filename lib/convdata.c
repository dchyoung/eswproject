/*
  Module: Any data conversion functions.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>
#include <math.h>

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
    if( (unit_1_idx >= MCG && unit_1_idx <= OZ) &&
	(unit_2_idx >= MCG && unit_2_idx <= OZ) )
    {	
	//Get conversion scale factor for the units
	convFactor = get_convFactor_mass(unit_1_idx, unit_2_idx);
	
	//Call mass conversion
	_convert(convFactor, floatDigit, file_out);
    }

    //Conversion 2: Unit Index number between 6 and 22: Metric Unit Conversion
    else if( (unit_1_idx >= ATTO && unit_1_idx <= EXA) &&
	(unit_2_idx >= ATTO && unit_2_idx <= EXA) )
    {
	//Get conversion scale factor for the units
	convFactor = get_convFactor_metric(unit_1_idx, unit_2_idx);
	_convert(convFactor, floatDigit, file_out);
    }
    
    //Conversion 3: Length Unit: 23 - 30
    else if( (unit_1_idx >= MM && unit_1_idx <= MILE) &&
	(unit_2_idx >= MM && unit_2_idx <= MILE) )
    {
	//Get conversion scale factor for the units
	convFactor = get_convFactor_length(unit_1_idx, unit_2_idx);
	//##debug
	printf("Conversion Factor == %f\n", convFactor);
	//##debug
	_convert(convFactor, floatDigit, file_out);
    }

    //Conversion 4: Time scale
    else if( (unit_1_idx >= SEC && unit_1_idx <= HR) &&
	(unit_2_idx >= SEC && unit_2_idx <= HR) )
    {
	//Get conversion scale factor for the units
	convFactor = get_convFactor_time(unit_1_idx, unit_2_idx);
	//##debug
	printf("Conversion Factor == %f\n", convFactor);
	//##debug
	_convert(convFactor, floatDigit, file_out);
    }
    
    //Conversion 5: Velocity conversion
    else if( (unit_1_idx >= M_S && unit_1_idx <= KN) &&
	(unit_2_idx >= M_S && unit_2_idx <= KN) )
    {
	//Get conversion scale factor for the units
	convFactor = get_convFactor_velocity(unit_1_idx, unit_2_idx);
	//##debug
	printf("Conversion Factor == %f\n", convFactor);
	//##debug
	_convert(convFactor, floatDigit, file_out);
    }


    //Conversion 6 : pi(3.14) conversion
    else if( (unit_1_idx == _PI) )
    {
	//Circle rate
	double pi = 3.14;
	//Get conversion scale factor for the units
	convFactor = pi;
	//##debug
	printf("Conversion Factor == %f\n", convFactor);
	//##debug
	_convert(convFactor, floatDigit, file_out);
    }
    

    //Conversion 7 : Angle conversion
    else if( (unit_1_idx >= RAD && unit_1_idx <= DEG) &&
	(unit_2_idx >= RAD && unit_2_idx <= DEG) )
    {
	//Get conversion scale factor for the units
	convFactor = get_convFactor_angle(unit_1_idx, unit_2_idx);
	//##debug
	printf("Conversion Factor == %f\n", convFactor);
	//##debug
	_convert(convFactor, floatDigit, file_out);
    }


    //Conversion 8 : Temperature conversion
    else if( (unit_1_idx >= F && unit_1_idx <= K) &&
	(unit_2_idx >= F && unit_2_idx <= K) )
    {
	//Convert input data
	conv_temperature(unit_1_idx, unit_2_idx, floatDigit, file_out);
    }


    //Conversion 9 : Volume Conversion
    else if( (unit_1_idx >= ML && unit_1_idx <= YD3) &&
	(unit_2_idx >= ML && unit_2_idx <= YD3) )
    {
	//assign same unit index for cm3, cc, and ml
	if( (unit_1_idx >= ML && unit_1_idx <= CM3) ) {
	    unit_1_idx = CM3;
	}

	if( (unit_2_idx >= ML && unit_2_idx <= CM3) ) {
	    unit_2_idx = CM3;
	}

	//Conversion between SI units
	if( (unit_1_idx >= ML && unit_1_idx <= M3) &&
	    (unit_2_idx >= ML && unit_2_idx <= M3) )
	{
	    //Get conversion scale factor for the units
	    convFactor = get_convFactor_volume_SI(unit_1_idx, unit_2_idx);
	}

	//Conversion between imperial
	else if ( (unit_1_idx >= IN3 && unit_1_idx <= YD3) &&
		  (unit_2_idx >= IN3 && unit_2_idx <= YD3) )
	{
	    //Get conversion scale factor for the units
	    convFactor = get_convFactor_volume_imperial(unit_1_idx, unit_2_idx);
	}

	//Conversion between SI and Imperial
	else {
	    //SI to imperial
	    if( (unit_1_idx >= ML && unit_1_idx <= M3) ){
		switch (unit_1_idx) {
		    case CM3:
			unit_1_idx = IN3;
			convFactor = 0.061;
		        break;
		
		    case DL:
			unit_1_idx = OZ_V;
			convFactor = 3.38;
		        break;
		    
		    case L:
			unit_1_idx = GAL;
			convFactor = 0.26;
			break;  

		    case M3:
			unit_1_idx = YD3;
			convFactor = 1.30;
			break;
		    default:
		        break;
		}
		
		convFactor = convFactor * get_convFactor_volume_imperial(unit_1_idx, unit_2_idx);
	    }
	    
	    //Imperial to SI
	    else {
		switch (unit_1_idx) {
		    case IN3:
			unit_1_idx = CM3;
			convFactor = 16.38;
		        break;
		
		    case OZ_V:
			unit_1_idx = DL;
			convFactor = 0.29;
		        break;
		    
		    case GAL:
			unit_1_idx = L;
			convFactor = 3.78;
			break;  

		    case FT3:
			unit_1_idx = L;
			convFactor = 28.31;
			break;
		    case BBL:
			unit_1_idx = M3;
			convFactor = 0.15;
			break;
		    case YD3:
			unit_1_idx = M3;
			convFactor = 0.76;
			break;
			
		    default:
		        break;
		}
		convFactor = convFactor * get_convFactor_volume_imperial(unit_1_idx, unit_2_idx);
	    }	    
	}	    
	
	//##debug
	printf("Conversion Factor == %f\n", convFactor);
	//##debug
	_convert(convFactor, floatDigit, file_out);
    }

    

    //Conversion 10:



    //....
    else {
	printf("Invalid Unit.\n");
	exit(0);
    }
}



//Unit_
int get_unit_idx(char* unit)
{
    char* unit_table[NUM_OF_UNITS] = {
	//Conventional Mass Unit (0 ~ 5)
	"mcg", "mg", "g", "kg", "lb", "oz",

	//Metric Unit (6 ~ 22)
        "atto", "femto", "pico", "nano", "micro", "milli", "centi", "deci",
	"none", "deca", "hecto", "kilo", "mega", "giga", "tera", "peta", "exa",

	//Length Unit (23 ~ 30)
	"mm", "cm", "m", "km", "in", "ft", "yd", "mile",

	//Time scale (31 ~ 33)
	"sec", "min", "hr",

	//Velocity Unit (34 ~ 42)
	"m/s", "m/h", "km/s", "km/h", "ft/s", "ft/h", "mi/s", "mi/h", "kn",

	//Angle Unit (43 ~ 45)
	"rad", "deg", "pi" ,

	//Temperature Unit (46 ~ 48)
	"F", "C", "K",

	//Volume SI (49 ~ 54)
	"ml", "cc", "cm3", "dl", "l", "m3",

	//Volume Imperial (55 ~ 60)
	"in3", "oz_v", "gal", "ft3", "bbl", "yd3"
	
    };
    
    int i;
    
    for(i = 0; i < NUM_OF_UNITS; i++) {
	if( !(strcmp(unit, unit_table[i]) )) {
	    return i;
	}
    }

    return -1;
    
}


//
void _convert(double convFactor, int floatDigit, char* file_out)
{
    int fd;
    double val;
    char word[BUFLEN];
    
    //1. open file to write
    fd = newFile(file_out);

    //2. Read word by word, converting
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
		convFactor = 1.0/16;
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
		convFactor = 1.0/10;
     
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1.0 / (10 * 100);
	    
	    else if( unitDistance == 3 ) 
		convFactor = 1.0 / (10 * 100 * 1000);
	    
	}

	else if( unit_1_idx == CM ) {
	    if( unitDistance == 1 ) 
		convFactor = 1.0/100;
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1.0 / (100 * 1000);
	    
	}

	else if( unit_1_idx == M ) 
	    convFactor = 1.0/1000;
	
		
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
		convFactor = 1.0 / 12;
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1.0 / (12 * 3);
	    
	    else if( unitDistance == 3 ) 
		convFactor = 1.0 / (12 * 3 * 1760);
	    
	}

	else if( unit_1_idx == FT ) {
	    if( unitDistance == 1 ) 
		convFactor = 1.0/3;
	    
	    else if( unitDistance == 2 ) 
		convFactor = 1.0 / (3 * 1760);
	    
	}

	else if( unit_1_idx == YD ) 
	    convFactor = 1.0/1760;
	
		
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
//Conversion function 4: Time scale
double get_convFactor_time(int unit_1_idx, int unit_2_idx)
{
     double convFactor = 1;
     int unitDistance;
     int i;

     unitDistance = unit_2_idx - unit_1_idx;

     if( unitDistance > 0 ) {
	 for( i = 0; i < unitDistance; i++ ) {
	     convFactor = convFactor / 60;
	 }
	 
     }
     else if( unitDistance < 0 ) {
	 for( i = 0; i < abs(unitDistance); i++ ) {
	     convFactor = convFactor * 60;
	 } 
     }

     return convFactor;   
    
}

//Conversion function 5: Velocity conversion

double get_convFactor_velocity(int unit_1_idx, int unit_2_idx)
{
    int unit1, unit2;
    double convFactor = 1;

    if( unit_1_idx != unit_2_idx ) {

	//1. Check if the conversion involves knott
	 if( unit_1_idx == KN ) {
	     convFactor = convFactor * 1.85;
	     unit_1_idx = KM_H;
	 }
    
	 if( unit_2_idx == KN ) {
	     convFactor = convFactor / 1.85;
	     unit_2_idx = KM_H;
	 }

	 //2. Time scale conversion
	if( (unit_1_idx % 2) != (unit_2_idx % 2) ) {

	    // x/sec to x hour
	    if( unit_1_idx % 2 == 0 ) {
		convFactor = convFactor * get_convFactor_time(HR, SEC);
	    }

	    // x/hr to x/sec
	    else {
		convFactor = convFactor * get_convFactor_time(SEC, HR);
	    }
	}

	//Get length unit of unit 1
	switch (unit_1_idx) {
	    case M_S:
	    case M_H:
		unit1 = M;
		break;
		
	    case KM_S:
	    case KM_H:
		unit1 = KM;
		break;

	    case FT_S:
	    case FT_H:
		unit1 = FT;
		break;

	    case MI_S:
	    case MI_H:
		unit1 = MILE;
		break;
		
	    default:
		break;
	}

	//Get length unit of unit 2
	switch (unit_2_idx) {
	    case M_S:
	    case M_H:
		unit2 = M;
		break;
		
	    case KM_S:
	    case KM_H:
		unit2 = KM;
		break;

	    case FT_S:
	    case FT_H:
		unit2 = FT;
		break;

	    case MI_S:
	    case MI_H:
		unit2 = MILE;
		break;
		
	    default:
		break;
	}

	convFactor = convFactor * get_convFactor_length(unit1, unit2);

	 
    }

    return convFactor;   
    
}



//Conversion function 6 : Circle rate conversion


//Conversion function 7 : Angle conversion
double get_convFactor_angle(int unit_1_idx, int unit_2_idx)
{
	double convFactor = 1;

	// 1. When you want to change Radian
	if( unit_1_idx == RAD ) {

		//Conversion formula
		convFactor = convFactor * (3.14/180);
    	}

	// 2. When you want to change Degree
	else if( unit_1_idx == DEG ) {

		//Conversion formula
		convFactor = convFactor * (180/3.14);
	}

	// 3. Units error_1 : Wrong input error
	else {
		convFactor = convFactor * 1;
		printf("Wrong input error\n");
	}

    return convFactor;

}


//Conversion function 8 : Temperature conversion
void conv_temperature(int unit_1_idx, int unit_2_idx, int floatDigit, char *file_out)
{
    int fd;		//File descriptor
    double temper;	// Random parameter 
    char word[BUFLEN];	//Data Buffer

   
    //1. open file to write
    fd = newFile(file_out);

    //2. Read word by word, converting
    while(1) {
	int n;
	
	//read word
	if( readWord(word, STDIN_FILENO) == 0 )
	    break;

	//Convert string to float
	temper = atof(word);
    
	//1. When you want to change F
	if(unit_1_idx == F){

	   //When you want to switch to C
	   if(unit_2_idx == C){

		//Conversion formula
		temper = (temper - 32) / 1.8;
	   }

	   //When you want to switch to K
	   else if(unit_2_idx == K){

		//Conversion formula
		temper = ( (temper - 32) / 1.8 ) + 273.15;
	   }

	   // Units error_1 : Wrong input error
	   else {
	   	temper = temper*1;
		printf("Wrong input error_1\n");
	   }
	}

	//2. When you want to change C
	else if(unit_1_idx == C){
	
	   //When you want to switch to F
	   if(unit_2_idx == F){

		//Conversion formula
		temper = (temper * 1.8) + 32;
	   }

	   //When you want to switch to k
	   else if(unit_2_idx == K){

		//Conversion formula
		temper = temper + 273.15;
	   }

	   // Units error_2 : Wrong input error
	   else {
	   	temper = temper*1;
		printf("Wrong input error_2\n");
	   }
	}

	//3. When you want to change K
	else if(unit_1_idx == K){

	   //When you want to switch to F
	   if(unit_2_idx == F){

		//Conversion formula
		temper = ( (temper - 273.15) * 1.8 ) + 32;
	   }

	   //When you want to switch to C
	   else if(unit_2_idx == C){

		//Conversion formula
		temper = temper - 273.15;
	   }

	   // Units error_3 : Wrong input error
	   else {
	   	temper = temper*1;
		printf("Wrong input error_2\n");
	   }
	}
    
	//Convert float to string
	fToStr(temper, floatDigit, word);
	
	//Write to outputfile
	n = write(fd, word, strlen(word));

	//Write error : Write error
	if( n == -1 ) {
	    perror("write error\n");
	    exit(errno);
	}	
    }

    printf("Conversion Factor == %f\n", temper);
    close(fd);
}



//Conversion function 9: Volume SI
double get_convFactor_volume_SI(int unit_1_idx, int unit_2_idx)
{
    double convFactor = 1;
    switch (unit_1_idx) {
	    case CM3:
		if( unit_2_idx == DL )
		    convFactor = 1.0/100;
		else if ( unit_2_idx == L )
		    convFactor = 1.0/(100 * 10);
		else if ( unit_2_idx == M3 )
		    convFactor = 1.0/(100 * 10 * 1000);			
		break;
		
	    case DL:
		if( unit_2_idx == CM3 )
		    convFactor = 100;
		else if ( unit_2_idx == L )
		    convFactor = 1.0/(10);
		else if ( unit_2_idx == M3 )
		    convFactor = 1.0/(10 * 1000);			
		break;

	    case L:
	        if( unit_2_idx == CM3 )
		    convFactor = (100 * 10);
		else if ( unit_2_idx == DL )
		    convFactor = 10;
		else if ( unit_2_idx == M3 )
		    convFactor = 1.0/(1000);			
		break;

	    case M3:
		if( unit_2_idx == CM3 )
		    convFactor = 1000 * 10 * 100;
		else if ( unit_2_idx == DL )
		    convFactor = (1000 * 10);
		else if ( unit_2_idx == L )
		    convFactor = (1000);			
		break;      
		
	    default:
		break;
	}
    return convFactor;
}


double get_convFactor_volume_imperial(int unit_1_idx, int unit_2_idx)
{
    double convFactor = 1;
    switch (unit_1_idx) {
	    case IN3:
		if( unit_2_idx == OZ_V )
		    convFactor = 1.0/1.80;
		else if ( unit_2_idx == GAL )
		    convFactor = 1.0/(1.80 * 127.99);
		else if ( unit_2_idx == FT3 )
		    convFactor = 1.0/(1.80 * 127.99 * 7.48);
		else if ( unit_2_idx == BBL )
		    convFactor = 1.0 / (1.80 * 127.99 * 7.48 * 5.61);
		else if( unit_2_idx == YD3 )
		    convFactor = 1.0/(1.80 * 127.99 * 7.48 * 5.61 * 4.81);
		break;
		
	    case OZ_V:
	        if( unit_2_idx == IN3 )
		    convFactor = 1.80;
		else if ( unit_2_idx == GAL )
		    convFactor = 1.0/(127.99);
		else if ( unit_2_idx == FT3 )
		    convFactor = 1.0/(127.99 * 7.48);
		else if ( unit_2_idx == BBL )
		    convFactor = 1.0 / (127.99 * 7.48 * 5.61);
		else if( unit_2_idx == YD3 )
		    convFactor = 1.0/(127.99 * 7.48 * 5.61 * 4.81);
		break;

	    case GAL:
	        if( unit_2_idx == IN3 )
		    convFactor = 1.80 * 127.99;
		else if ( unit_2_idx == OZ_V )
		    convFactor = (127.99);
		else if ( unit_2_idx == FT3 )
		    convFactor = 1.0/7.48;
		else if ( unit_2_idx == BBL )
		    convFactor = 1.0 / (7.48 * 5.61);
		else if( unit_2_idx == YD3 )
		    convFactor = 1.0/(7.48 * 5.61 * 4.81);
		break;

	    case FT3:
	        if( unit_2_idx == IN3 )
		    convFactor = 1.80 * 127.99 * 7.48;
		else if ( unit_2_idx == OZ_V )
		    convFactor = (127.99 * 7.48);
		else if ( unit_2_idx == GAL )
		    convFactor = 7.48;
		else if ( unit_2_idx == BBL )
		    convFactor = 1.0 / (5.61);
		else if( unit_2_idx == YD3 )
		    convFactor = 1.0/(5.61 * 4.81);
		break;
	    case BBL:
	        if( unit_2_idx == IN3 )
		    convFactor = 1.80 * 127.99 * 7.48 * 5.61;
		else if ( unit_2_idx == OZ_V )
		    convFactor = (127.99 * 7.48 * 5.61);
		else if ( unit_2_idx == GAL )
		    convFactor = 7.48 * 5.61;
		else if ( unit_2_idx == FT3 )
		    convFactor = 5.61;
		else if( unit_2_idx == YD3 )
		    convFactor = 1.0/4.81;
		break;
		
	    case YD3:
	        if( unit_2_idx == IN3 )
		    convFactor = 1.80 * 127.99 * 7.48 * 5.61 * 4.81;
		else if ( unit_2_idx == OZ_V )
		    convFactor = (127.99 * 7.48 * 5.61 * 4.81);
		else if ( unit_2_idx == GAL )
		    convFactor = 7.48 * 5.61 * 4.81;
		else if ( unit_2_idx == FT3 )
		    convFactor = 5.61 * 4.81;
		else if( unit_2_idx == BBL )
		    convFactor = 4.81;
		break;
		
	    default:
		break;
	}
    return convFactor;
}



//Conversion function 10:




