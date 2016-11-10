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
void computation(char *option, char *file_out)
{
    //Computation 1: computation selecting data
    if( !(strcmp(option, "mean")) ) {
	comp_mean(file_out);
    }

    //Conversion 2:
    
    //Conversion 3:

    //....
}

//Computation 1 : Compute selecting data
//For iniitial test. Needs update
void comp_mean(char *file_out)
{
    int fd;
    int mean;
    int i = 0;
    float sum = 0;
    float val;
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
     sprintf(word, "%f", val);

     //6. Write to output file
     strcat(word, "");
     mean = sum / i;
     n = write(fd, word, strlen(word));

     //7. eroor_1
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

//Compute 2:


//Compute 3:

