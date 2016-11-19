#ifndef CONVDATA_H
#define CONVDATA_H

//Master index function that calls each conversion
void conversion(char *option, int floatDigit, char *file_out);

//Conversion Function 1: Mass conversion. (mg, g, kg, lb, oz)
void conv_mass(int unit_1_idx, int unit_2_idx, int floatDigit, char* file_out);

//Input: unit name in string, return: unit ID number
int get_unit_idx(char* unit);
//Input: Two units by their ID numbers, return: scale factor between units
double get_convFactor_mass(int unit_1_idx, int unit_2_idx);


#endif
