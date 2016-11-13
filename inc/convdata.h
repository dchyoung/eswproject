#ifndef CONVDATA_H
#define CONVDATA_H

void conversion(char *option, int floatDigit, char *file_out);
int get_unit_idx(char* unit);
double get_convFactor_mass(int unit_1_idx, int unit_2_idx);
void conv_mass(int unit_1_idx, int unit_2_idx, int floatDigit, char* file_out);

#endif
