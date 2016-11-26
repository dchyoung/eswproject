#ifndef CONVDATA_H
#define CONVDATA_H

/*Unit ID number Macros*/
#define NUM_OF_UNITS 31
//Mass units
#define MCG    0
#define MG     1
#define G      2
#define KG     3
#define LB     4
#define OZ     5

//SI units
#define ATTO   6
#define FEMTO  7
#define PICO   8
#define NANO   9
#define MICRO  10
#define MILLI  11
#define CENTI  12
#define DECI   13
#define NONE   14
#define DECA   15
#define HECTO  16
#define KILO   17
#define MEGA   18
#define GIGA   19
#define TERA   20
#define PETA   21
#define EXA    22

//Length units
#define MM     23
#define CM     24
#define M      25
#define KM     26
#define IN     27
#define FT     28
#define YD     29
#define MILE   30


//Master index function that calls each conversion
void conversion(char *option, int floatDigit, char *file_out);

//Actual converting function by conversion scale factor
void _convert(double convFactor, int floatDigit, char* file_out);

//Input: unit name in string, return: unit ID number
int get_unit_idx(char* unit);

//Input: Two units by their ID numbers, return: scale factor between units
/*Conversion 1: Conventional Mass unit conversion. (mg, g, kg, lb, oz)*/
double get_convFactor_mass(int unit_1_idx, int unit_2_idx);

/*Conversion 2: Metric units (atto, femto, pico, nano, micro, milli, centi, deci,
	                      none, deca, hecto, kilo, mega, giga, tera, peta, exa*/
double get_convFactor_metric(int unit_1_idx, int unit_2_idx);

/*Conversion 3: Length units (mm, cm, m, km, in, ft, yd, mile)*/
double get_convFactor_length(int unit_1_idx, int unit_2_idx);
double get_convFactor_length_imperial(int unit_1_idx, int unit_2_idx);
double get_convFactor_length_SI(int unit_1_idx, int unit_2_idx);

#endif
