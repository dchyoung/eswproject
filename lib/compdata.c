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
    //Conversion 1: conversion kilgo to gram
    if( !(strcmp(option, "mean")) ) {
	comp_mean(file_out);
    }

    //Conversion 2:
    
    //Conversion 3:

    //....
}
//Compute 1:
void comp_mean(char *file_out)
{
    int fd;
    int i = 0;
    float sum = 0;
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
	sum = sum + val;
        i = i + 1;
        
       
	

	
	
    }
if i != 0
mena = sum / i;

    close(fd);
}

//Compute 2:


//Compute 3:

