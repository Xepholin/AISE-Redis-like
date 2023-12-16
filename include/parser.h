#ifndef PARSER_H
#define PARSER_H

void clearBuffer(char *buffer);
int getNumberArgs(char *input);
int isNumber(char c);
void getLenArgs(int *lenArgs, char *input, int nbArgs);
char **getStrings(int *lenArgs, int nbArgs, char *input);
char **parser(char *input);

#endif