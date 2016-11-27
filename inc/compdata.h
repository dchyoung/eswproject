/*
  Module: Any data computation.
 */

#ifndef COMPDATA_H
#define COMPDATA_H


//Master Index Function
void computation(char *option, int floatDigit, char *file_out);

//Function1: Compute mean
void comp_mean(int floatDigit, char *file_out);
//Function2: Compute weighted value
void comp_prob(int floatDigit, char *file_out);
//Function3: Histogram
void comp_hist(int distance, int floatDigit, char *file_out);
//Function3: Standard deviation
void comp_stdde(int floatDigit, char *file_out);
//Function3: Variance
void comp_vari(int floatDigit, char *file_out);


#endif
