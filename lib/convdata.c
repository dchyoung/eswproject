/*
  Module: Any data conversion functions.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>

#include "parser.h"
#include "convdata.h"

//master function that calls corresponding data conversion function
void conversion(char *option, char *file_out)
{
    //Conversion 1: conversion kilgo to gram
    if( !(strcmp(option, "ktog")) ) {
	conv_kTog(file_out);
    }

    //Conversion 2:
    
    //Conversion 3:

    //....
}


//Conversion function 1: Convert kilo to gram
//For iniitial test. Needs update
void conv_kTog(char *file_out)
{
    int fd;
    float val;
    char word[100];
    
    //1. open file
    fd = newFile(file_out);

    while(1) {
	int n;
	
	//2. read word
	if( readWord(word) == 0 )
	    break;

	//3. Convert string to float
	val = atof(word);
    
	//4. convert kilo to gram
	val = val * 1000;
    
	//5. convert float to string
	
	fToStr(val, 2, word);
	
	//6. Write to outputfile
	n = write(fd, word, strlen(word));

	//7. error
	if( n == -1 ) {
	    perror("write");
	    exit(errno);
	}
	
    }

    close(fd);
}

//Conversion function 2:


//Conversion function 3:

//Conversion function 4:

//Conversion function 5:

//Conversion function 6:
