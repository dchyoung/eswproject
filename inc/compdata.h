/*
  Module: Any data computation.
 */

#ifndef COMPDATA_H
#define COMPDATA_H


//Master Index Function
void computation(char *option, int floatDigit, char *file_out);

//Function 1 : Compute mean of input data
void comp_mean(int floatDigit, char *file_out);

//Function 2 : Compute weighting of each value of input data
void comp_prob(int floatDigit, char *file_out);

//Function3: Histogram
void comp_hist(int distance, int floatDigit, char *file_out);

//Function 4 : Comute standard deviation of input data
void comp_stdde(int floatDigit, char *file_out);

//Function 5 : Comute variance of input data
void comp_vari(int floatDigit, char *file_out);

//Function6:
 


#endif
