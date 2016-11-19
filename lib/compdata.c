/*
 *Module: Any data computation function.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "parser.h"
#include "compdata.h"


//Master function that calls corresponding data conversion function
void computation(char *option, int floatDigit, char *file_out)
{
    //Computation 1: Compute mean
    if( !(strcmp(option, "mean")) ) {
	comp_mean(floatDigit, file_out);
    }

    //Conversion 2: Compute weighting of each value
    if( !(strcmp(option, "prob")) ) {
	comp_prob(floatDigit, file_out);
    }
    
    //Conversion 3: Value Frequency Count (Histogram)
    

    //....
}

//Computation 1 : Compute Mean
void comp_mean(int floatDigit, char *file_out)
{
    int fd;           //File descriptor
    int i = 0;        //Data Counter
    char word[BUFLEN];//Data Buffer
    double sum = 0, val, mean;
    
    //1. open file to write
    fd = newFile(file_out);

    while(1) {
	//Read word
	if( readWord(word, STDIN_FILENO) == 0 )
	    break;

	//Convert string to float
	val = atof(word);
    
	//Accumulate sum
	sum = sum + val;

	//Count number of data
	i = i + 1;
    }

    //2. Compute mean and write to file
    if (i != 0){
	int n;

	mean = sum / i;
	 
	//Convert mean to string
	fToStr(mean, floatDigit, word); 
	 
	//write mean to file
	n = write(fd, word, strlen(word));

	//Write error_1
	if(n == -1) {
	    perror("write error");
	    exit(errno);
	}
    }

    //3. error_2
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
    int fd, fd_tmp, n;
    int i = 0;
    double prob;
    char word[BUFLEN];
    
    //1. open file to write
    fd = newFile(file_out);

    //2. open temporary file
    fd_tmp = newFile("tmp");

    //3. Count data and Copy from std input stream  to temporary file
    while(1) {
    
	if( readWord(word, STDIN_FILENO) == 0 )
	    break;
	
	strcat(word, " ");//Spacing each word
	n = write(fd_tmp, word, strlen(word));

	if(n == -1) {
	    perror("write error");
	    exit(errno);
	}
	i = i + 1;	 
    }
    if( i > 0) {
	close(fd_tmp);
    }
	
    //4. Open tmp file
    fd_tmp = open("tmp", O_RDONLY);
	
    while(i != 0) {

	//read word
	if( readWord(word, fd_tmp) == 0 )
	    break;

	//Convert string to float 
	prob = atof(word);

	//Compute weighted value
	prob = prob / i;

	//Convert double to string
	fToStr(prob, floatDigit, word);;

	//Write to output file
	n = write(fd, word, strlen(word));

	//Write error
	if(n == -1) {
	    perror("write error");
	    exit(errno);
	}
    }

    close(fd);
    close(fd_tmp);
    //remove temporary file
    remove("tmp");
}


//Compute 3:

