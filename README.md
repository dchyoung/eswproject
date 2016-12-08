# eswproject

-Process numeric datafile
-Input: Raw data file (numeric data deliminated by space) through standard input stream
-Output: Processed data file

Supported Functions: 
- 6 Mass units conversion : "mcg", "mg", "g", "kg", "lb", "oz",
- 17 Metric units conversion : "atto", "femto", "pico", "nano", "micro", "milli", "centi", "deci", "none", "deca", "hecto", "kilo", 
                               "mega", "giga", "tera", "peta", "exa",
- 8 Length units conversion : "mm", "cm", "m", "km", "in", "ft", "yd", "mile",
- 3 Time scales conversion : "sec", "min", "hr",
- 9 velocity units conversion : "m/s", "m/h", "km/s", "km/h", "ft/s", "ft/h", "mi/s", "mi/h", "kn",
- 3 angle units conversion : "rad", "deg", "pi" ,
- 3 Temperature units conversion : "ml", "cc", "cm3", "dl", "l", "m3",
- 12 Volume units conversion : "in3", "oz_v", "gal", "ft3", "bbl", "yd3"

- Mean computation
- Variance computation
- Standard devication computation
- Histogram


Command Format:
- Number of floating point digit set 2 as defaul

cat dataFile | ./datamaster (1stOption) (2ndOption) (NumberOfFloatingPointDigits) (outputFileName) 
cat dataFile | ./datamaster -conv km/s-mi/s outputFile
cat dataFile | ./datamaster -comp mean outputFile

