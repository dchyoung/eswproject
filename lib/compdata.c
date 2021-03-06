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
#include <math.h>

#include "parser.h"
#include "compdata.h"


//Master function that calls corresponding data conversion function
void computation(char *option, int floatDigit, char *file_out)
{
    int idx = 0;
    char suboption[BUFLEN];
        
    //Parse each unit of convert from/to from option string 
    idx = parseToken(idx, option, suboption, '-');
        
    //Computation 1 : Compute mean of input data
    if( !(strcmp(suboption, "mean")) ) {
	comp_mean(floatDigit, file_out);
    }

    //Computation 2 : Compute weighting of each value of input data
    else if( !(strcmp(suboption, "prob")) ) {
	comp_prob(floatDigit, file_out);
    }
    
    //Computation 3: Value Frequency Count (Histogram)
    else if( !(strcmp(suboption, "hist")) ) {
	
	idx = parseToken(idx, option, suboption, '-');

	if( idx > 4 ) {
	    comp_hist(atoi(suboption), floatDigit, file_out);
	}
	else {
	    printf("Invalid hist option. Need interval n:  hist-n\n");
	}
    }
	
	
    //Computation 4 : Comute standard deviation of input data
    else if( !(strcmp(suboption, "stdde")) ) {
	comp_stdde(floatDigit, file_out);
    }

    //Computation 5 : Comute variance of input data
    else if( !(strcmp(suboption, "vari")) ) {
	comp_vari(floatDigit, file_out);
    }


    //Computation 6: 
    
    

    //....
}

//Computation 1 : Compute mean of input data
void comp_mean(int floatDigit, char *file_out)
{
    int fd;            //File descriptor
    int i = 0;         //Data Counter
    char word[BUFLEN]; //Data Buffer
    double sum = 0, val, mean; // Random parameter
    
    //1. Open file to write
    fd = newFile(file_out);

    //2. Count and sum
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

    //3. Compute mean and write to file
    if (i != 0){
	int n;

	//Compute mean
	mean = sum / i;
	 
	//Convert mean to string
	fToStr(mean, floatDigit, word); 
	 
	//Write mean to file
	n = write(fd, word, strlen(word));

	//Write error_1 : Write error
	if(n == -1) {
	    perror("Write error\n");
	    exit(errno);
	}
    }

    //4. error_2 : Null data
    else if ( i == 0 ) {
	perror("Null data\n");
	exit(errno);
    }

    close(fd);
}

//Compute 2 : Compute weighting of each value of input data
void comp_prob(int floatDigit, char *file_out)
{
    int fd, fd_tmp, n; 	//File descriptor
    int i = 0;		//Data Counter
    double prob;	//Random parameter
    char word[BUFLEN];	//Data Buffer
    
    //1. open file to write
    fd = newFile(file_out);

    //2. open temporary file
    fd_tmp = newFile("tmp");

    //3. Count data and Copy from std input stream to temporary file
    while(1) {
    
	if( readWord(word, STDIN_FILENO) == 0 )
	    break;
	
	strcat(word, " ");//Spacing each word
	n = write(fd_tmp, word, strlen(word));

	//Write error_1 : Write error
	if(n == -1) {
	    perror("Write error_1\n");
	    exit(errno);
	}

	//Count
	i = i + 1; 	 
    }

    //When Input data exists
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

	//Write error_2 : prob write error
	if(n == -1) {
	    perror("Write error_2\n");
	    exit(errno);
	}
    }

    //close files
    close(fd);
    close(fd_tmp);

    //remove temporary file
    remove("tmp");
}


//Computation 3: Frequency count (Histogram)

void comp_hist(int distance, int floatDigit, char *file_out)
{
    char word[BUFLEN];
    int fd, fd_tmp;
    int idx, n;
    int* hist;
    double max, min, value;
    
    //1. open file to write
    fd = newFile(file_out);

    //2. open temporary file
    fd_tmp = newFile("tmp");

    
    if ( readWord(word, STDIN_FILENO) > 0 ) {

	//Max and min initial value
	max = atof(word);
	min = max;

	//Copy to tmp file
	strcat(word, " ");//Spacing each word
	if(  write(fd_tmp, word, strlen(word)) == -1 ) {
	    perror("write error");
	    exit(errno);
	}


	//3. Find max and min
	while(1) {

	    //read data value
	    if( readWord(word, STDIN_FILENO) == 0 )
		break;

	    value = atof(word);

	    //update max and min
	    if( value > max ) {
		max = value;
	    }

	    if( value < min ) {
		min = value;
	    }

	    //copy to tmp file
	    strcat(word, " ");         //Spacing each word
	    if(  write(fd_tmp, word, strlen(word)) == -1 ) {
		perror("write error");
		exit(errno);
	    }

	}
	//4. Copy finished. Close tmp file
	close(fd_tmp);

	//5. Histogram intervals based on given distance and max/min value
	n = (int) ((max - min) / distance);

	if( n == 0) 
	    n = 1;

	printf("Max value: %f Min value: %f \n", max, min);
	printf("The number of intervals : %d\n", n);
	//Array allocation for counting table. index nth =  frequency of interval nth
	hist = (int*)malloc(sizeof(int) * n); 

	//Array initialization
	for(idx = 0; idx < n; idx++) {
	    hist[idx] = 0;
	}
	
	//5. Open tmp file and Count frequency for each interval
	fd_tmp = open("tmp", O_RDONLY);

	//frequency count
	while(1) {

	    //read word
	    if( readWord(word, fd_tmp) == 0 )
		break;

	    //Convert string to float 
	    value = atof(word);

	    idx = (int) ((value - min) / distance);

	    hist[idx] = hist[idx] + 1;
	}

	//Array initialization
	for(idx = 0; idx < n; idx++) {
	    printf("%d\n", hist[idx]);
	}

	
	//6. write result
	for(idx = 0; idx < n; idx++){

	    //write interval

	    //Interval from:
	    value = min + distance * idx;
	    	    
	    fToStr(value, floatDigit, word);
	    strcat(word, " ~ ");
	    
	    if( write(fd, word, strlen(word)) == -1 ) {
	        perror("write error");
		exit(errno);
	    }

	    //Interval to:
	    value = value + distance;
	    fToStr(value, floatDigit, word);
	    strcat(word, " : ");
	    
	    if( write(fd, word, strlen(word)) == -1 ) {
	        perror("write error");
		exit(errno);
	    }

	    //Write frequency of the interval:
	    fToStr(hist[idx],0, word);
	    strcat(word, "\n");
	    if( write(fd, word, strlen(word)) == -1 ) {
		perror("write error");
		exit(errno);	
	    }
	    
	}
	
	free(hist);

	//close files
	close(fd);
	close(fd_tmp);
	
	//remove temporary file
	remove("tmp");
	
    }

    return;    
}

//Computation 4: Comute standard deviation of input data
void comp_stdde(int floatDigit, char *file_out)
{

    int fd, fd_tmp, n;						//File descriptor
    int i = 0;							//Data Counter
    double stdde, prob ,prob_1, prob_2, sum_1=0, sum_2 = 0;	//Random parameter
    char word[BUFLEN];						//Data Buffer
    

    //1. open file to write
    fd = newFile(file_out);

    //2. open temporary file
    fd_tmp = newFile("tmp");


    //3. Count data and Copy from std input stream to temporary file
    while(1) {
    
	if( readWord(word, STDIN_FILENO) == 0 )
	    break;
	
	strcat(word, " ");//Spacing each word
	n = write(fd_tmp, word, strlen(word));


	//Write error_1 : Write error
	if(n == -1) {
	    perror("Write error_1\n");
	    exit(errno);
	}

	//Counte
	i = i + 1;	 
    }

    //When Input data exists
    if( i > 0) {
	close(fd_tmp);
    }

	
    //4. Open tmporary file & Computation of total sum of each square.
    fd_tmp = open("tmp", O_RDONLY);
	
    //When Input data exists
    while(i != 0) {

	//read word
	if( readWord(word, fd_tmp) == 0 )
	    break;

	//Convert string to float 
	prob = atof(word);

	//Compute weighted value
	prob_1 = (prob * prob) / i;
	prob_2 = prob / i;
	sum_1 = sum_1 + prob_1;
	sum_2 = sum_2 + prob_2;

    }


    //5. Compute standard deviation
    if(sum_1 >= 0 && sum_2 >= 0) {

	//standard deviation formula
	stdde = sqrt(sum_1 - (sum_2 * sum_2));


	//Convert double to string
	fToStr(stdde, floatDigit, word);;

	//Write to output file
	n = write(fd, word, strlen(word));

	//Write error_2 : File Write error
	if(n == -1) {
	    perror("Write fd error_2\n");
	    exit(errno);
	}
    }
  

    //close files
    close(fd);
    close(fd_tmp);


    //remove temporary file
    remove("tem");
}



//Computation 5: Comute variance of input data
void comp_vari(int floatDigit, char *file_out)
{

    int fd, fd_tmp, n;						//File descriptor
    int i = 0;							//Data Counter
    double vari, prob , prob_1, prob_2, sum_1=0, sum_2 = 0;	//Random parameter
    char word[BUFLEN];						//Data Buffer



    //1. open file to write
    fd = newFile(file_out);

    //2. open temporary file
    fd_tmp = newFile("tmp");


    //3. Count data and Copy from std input stream to temporary file
    while(1) {
    
	if( readWord(word, STDIN_FILENO) == 0 )
	    break;
	
	strcat(word, " "); //Spacing each word
	n = write(fd_tmp, word, strlen(word));


	//Write error_1 : Write error
	if(n == -1) {
	    perror("Write error_1\n");
	    exit(errno);
	}

	//Count
	i = i + 1;	 
    }

    //When input data exists
    if( i > 0) {
	close(fd_tmp);
    }

	
    //4. Open tmporary file & Computation of total sum.
    fd_tmp = open("tmp", O_RDONLY);
	
    while(i != 0) {

	//read word
	if( readWord(word, fd_tmp) == 0 )
	    break;

	//Convert string to float 
	prob = atof(word);

	//Compute weighted value
	prob_1 = (prob * prob) / i;
	prob_2 = prob / i;
	sum_1 = sum_1 + prob_1;
	sum_2 = sum_2 + prob_2;

    }


    //5. Compute variance
    if(sum_1 >= 0 && sum_2 >= 0) {

	//Variance formula
	vari = sum_1 - (sum_2 * sum_2);


	//Convert double to string
	fToStr(vari, floatDigit, word);;

	//Write to output file
	n = write(fd, word, strlen(word));

	//Write error_2 : File Write error
	if(n == -1) {
	    perror("Write fd error\n");
	    exit(errno);
	}
    }
  

    //close files
    close(fd);
    close(fd_tmp);

    //remove temporary file
    remove("tem");
}



//Computation 6:


