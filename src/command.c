#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/parser.h"

#define NOMBRE_ARGS 2
#define NOMBRE_MAX_COMMANDS_ARGS 10

const char *commands[NOMBRE_ARGS][NOMBRE_MAX_COMMANDS_ARGS] = {
    {"1", "PING"},
    {"2", "GET", "SET", "DEL"}
};

void free_arrayString(char **parsedStr, int nbArgs)
{

    for (int i = 0; i < nbArgs; i++)
    {
        free(parsedStr[i]);
    }
}

char *command(char *input) {

    char **parsed = parser(input);
    char *response = NULL;

    

    for (int i = 0; i < NOMBRE_ARGS; ++i)   {

        if (atoi(parsed[0]) == atoi(commands[i][0]))    {
            int nb_commands = 0;

            switch (atoi(commands[i][0]))
            {
            case 1:
                while (commands[0][nb_commands] != NULL) {
                    nb_commands++;
                }

                for (int j = 1; j < nb_commands; ++j)  {
                    if (!strcmp(commands[i][j], parsed[1]))   {
                        if (strcmp(parsed[1], "PING\n"))  {
                            response = malloc((strlen("PONG") + 2) * sizeof(char));
                            strcpy(response, "PONG");
                        }

                        int nbArgs = atoi(parsed[0])+1;
                        free_arrayString(parsed, nbArgs);
                        return response;
                    }
                    
                }

                break;
            
            case 2:
                while (commands[0][nb_commands] != NULL) {
                    nb_commands++;
                }

                for (int j = 1; j < nb_commands; ++j)  {
                    if (!strcmp(commands[i][j], parsed[1]))   {
                        if (strcmp(parsed[1], "GET"))  {

                        }
                        else if (strcmp(parsed[1], "SET"))  {

                        }
                        else if (strcmp(parsed[1], "DEL"))  {

                        }

                        int nbArgs = atoi(parsed[0])+1;
                        free_arrayString(parsed, nbArgs);

                        return response;
                    }
                }

                break;
            
            default:
                perror("incorrect number of arg");
                break;
            }

            break;
        }
    }

    response = malloc((strlen("NOT FOUND") + 2) * sizeof(char));
    strcpy(response, "NOT FOUND");

    int nbArgs = atoi(parsed[0])+1;
    free_arrayString(parsed, nbArgs);

    return response;
}