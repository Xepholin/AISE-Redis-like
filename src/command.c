#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/parser.h"

void free_arrayString(char **parsedStr, int nbArgs)
{

    for (int i = 0; i < nbArgs; i++)
    {
        free(parsedStr[i]);
    }
}

char *command(char *input) {

    char **parsed = parser(input);

    // if (!strcmp(parsed[0], "PING") && parsed[1] == NULL)
    // {

    //     char *response = malloc((strlen("PONG") + 2) * sizeof(char));

    //     strcpy(response, "PONG");

    //     free_arrayString(parsed, numberOfArgs);
    //     return response;
    // }
    // else
    // {

    //     char *response = malloc((strlen(parsed) + 2) * sizeof(char));
    //     strcpy(response, parsed);
    //     free_arrayString(parsed, numberOfArgs);

    //     return response;
    // }

    int nbArgs = atoi(parsed[0])+1;

    free_arrayString(parsed, nbArgs);

    return input;
}