#ifndef PARSER_H
#define PARSER_H

int newFile(char *filename);
int isWhiteChar(char c);
int readWord(char *word);
void fToStr(double val, int n, char *str);
int parseToken(int idx, char* str, char* token, char delim);

#endif
