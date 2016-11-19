#ifndef PARSER_H
#define PARSER_H
#define BUFLEN 100

//Open and return new file
int newFile(char *filename);

//return 1 if c is white chracter, 0 if not.
int isWhiteChar(char c);

//Read word from file stream (string deliminated by space)
int readWord(char *word, int fstream);

//convert float to string by n decimal point
void fToStr(double val, int n, char *str);

//parse token from string, deliminated by delim character, retrun: the index of string
int parseToken(int idx, char* str, char* token, char delim);

#endif
