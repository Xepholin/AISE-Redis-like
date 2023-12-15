#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/parser.h"

char* parser(char *input)   {

    if (!strcmp(input, "PING\n"))   {
        printf("here\n");

        char * response = malloc((strlen("PONG") + 2) * sizeof(char));

        strcpy(response, "PONG");

        return response;
    }
    else    {
        printf("la\n");

        char * response = malloc((strlen(input) + 2) * sizeof(char));
        strcpy(response, input);

        return response;
    }

    return NULL;

}

void clearBuffer(char *buffer)  {

    for(int i = 0; i < strlen(buffer) ; ++i){
        buffer[i] = '\0';
    }
}