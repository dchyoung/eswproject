#ifndef CONVDATA_H
#define CONVDATA_H

/*Unit ID number Macros*/
#define NUM_OF_UNITS 61

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

//Time scale
#define SEC    31
#define MIN    32
#define HR     33

//Velocity units
#define M_S    34
#define M_H    35
#define KM_S   36
#define KM_H   37
#define FT_S   38
#define FT_H   39
#define MI_S   40
#define MI_H   41
#define KN     42

//Angle units
#define RAD    43
#define DEG    44
#define _PI    45


//Temperature units
#define F      46
#define C      47
#define K      48

//Volume units
#define ML     49
#define CC     50
#define CM3    51
#define DL     52
#define L      53
#define M3     54

#define IN3    55
#define OZ_V   56
#define GAL    57
#define FT3    58
#define BBL    59
#define YD3    60


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

/*Conversion 4: Time Scale (sec, min, hour)*/
double get_convFactor_time(int unit_1_idx, int unit_2_idx);

/*Conversion 5: Velocity units (m/s, m/h, km/s, km/h, ft/s, ft/h, mi/s, mi/h, kn)*/
double get_convFactor_velocity(int unit_1_idx, int unit_2_idx);

/*Conversion 6 : pi units (deg or rad)*/


/*Conversion 7 : Angle units (rad, deg)*/
double get_convFactor_angle(int unit_1_idx, int unit_2_idx);


/*Conversion 8 : Temperature units (F , C , K ) */
double get_convFactor_temper(int unit_1_idx, int unit_2_idx);



/*Conversion 9: Volume (ml, cc, cm3, dl, l, m3, in3, ft3, yd3, gal, bbl, oz)*/
double get_convFactor_volume_SI(int unit_1_idx, int unit_2_idx); //Volume SI
double get_convFactor_volume_imperial(int unit_1_idx, int unit_2_idx); //Volume Imperial




#endif

