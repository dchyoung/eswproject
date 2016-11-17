/*parser set*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "parser.h"

int newFile(char *filename)
{
    int fd;
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if( fd == -1 ) {
	perror("Open");
	exit(errno);
    }

    return fd;    
}

int isWhiteChar(char c)
{
    if( c == ' ' || c == '\n' || c == '\t' ) {
	return 1;
    }
    else return 0;
}

//Get word to word buffer and return the length
int readWord(char *word, int fstream)
{
    int i = 0;
    char c;
    ssize_t n;

    //Skip first white characters
    while(1) {
	n = read(fstream, &c, 1);

	//Read Error
	if( n == -1 ) {
	    perror("read");
	    exit(errno);
	}


	//End of file
	if( n == 0 ) {
	    return 0;
	}

	//Break at the first non-white character
	if( !(isWhiteChar(c)) )
	    break;
    }

    //Get characters until the first white characters
    
    while(1) {
	
	//copy character to word
	word[i++] = c;
	 
	n = read(fstream, &c, 1);

	//read error
	if( n == -1 ) {
	    perror("read");
	    exit(errno);
	}


	if( n == 0 || isWhiteChar(c) ) {
	    word[i] = 0;
	    return i;
	}
    }
}

void fToStr(double val, int n, char *str)
{
    char format[100];
    char buf[100];
        
    sprintf(buf, "%d", n);

    //Make format string
    strcpy(format, "%.");
    strcat(format, buf);
    strcat(format, "f");

    //Convert float to str
    sprintf(str, format, val);

    //spacing
    strcat(str, " ");      
}

int parseToken(int idx, char* str, char* token, char delim)
{
    int i, j;

    //skip first delim
    for( i = idx; i < strlen(str); i++ ) {
	if( str[i] != delim )
	    break;	
    }

    //copy token
    j = 0;
    while( i < strlen(str) ) {
	token[j] = str[i];
	j++;
	i++;

	if(str[i] == delim || str[i] == '\0')
	{
	    token[j] = '\0';
	    break;
	}
	
    }
    return i;
}
