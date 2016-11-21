#ifndef CONVDATA_H
#define CONVDATA_H

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



#endif
