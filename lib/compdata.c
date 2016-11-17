/*
  Module: Any data computation function.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>

#include "parser.h"
#include "compdata.h"

//master function that calls corresponding data conversion function
void computation(char *option, int floatDigit, char *file_out)
{
    //Computation 1: computation selecting data
    if( !(strcmp(option, "mean")) ) {
	comp_mean(floatDigit, file_out);
    }

    //Conversion 2: computation all data
    if( !(strcmp(option, "prob")) ) {
	comp_prob(floatDigit, file_out);
    }
    
    //Conversion 3:

    //....
}

//Computation 1 : Compute selecting data
//For initial test. Needs update
void comp_mean(int floatDigit, char *file_out)
{
    int fd;
    int i = 0;
    float sum = 0;
    float val, mean;
    char word[100];
    
    //1. open file
    fd = newFile(file_out);

    while(1) {

     //2. read word
     if( readWord(word) == 0 )
      break;

     //3. Convert string to float
     val = atof(word);
    
     //4. compute sum of selecting data
     sum = sum + val;
     i = i + 1;

    }

    //5. convert float to string
    if (i != 0){
     int n;
     mean = sum / i;
	 
	 //Convert mean to string
	 fToStr(mean, floatDigit, word); 
	 
	 //write mean to file
     n = write(fd, word, strlen(word));

     //7. error_1
     if(n == -1) {
      perror("write error");
      exit(errno);
     }
    }

    //8. error_2
    else if ( i == 0 ) {
     perror("Null data");
     exit(errno);
    }


    close(fd);
}

//Compute 2: Compute all data
//For initial test. Needs update
void comp_prob(int floatDigit, char *file_out)
{
    int fd, n;
    int i = 0;
    float prob;
    char word[100];
    
    //1. open file
    fd = newFile(file_out);

    while(1) {
     if( readWord(word) == 0 )
     break;

     //3. compute counting of all data
     i = i + 1;
    }

    while(i != 0) {

     //2. read word
     if( readWord(word) == 0 )
      break;

     //4. Convert string to float & convert float to strin

     prob = atof(word);
     prob = prob / i;
     fToStr(prob, floatDigit, word);;

     //5. Write to output file
     n = write(fd, word, strlen(word));

     //6. error_1
     if(n == -1) {
      perror("write error");
      exit(errno);
     }
    }

    close(fd);
}


//Compute 3:

