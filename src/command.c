#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/parser.h"

#define NOMBRE_ARGS 3
#define NOMBRE_MAX_COMMANDS_ARGS 10

#define TAILLE_MAX_NVAR 15

const char *commands[NOMBRE_ARGS][NOMBRE_MAX_COMMANDS_ARGS] = {
    {"1", "PING"},
    {"2", "GET", "DEL"},
    {"3", "SET"}
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

            switch (atoi(parsed[0]))
            {
            case 1:
                while (commands[0][nb_commands] != NULL) {
                    nb_commands++;
                }

                for (int j = 1; j < nb_commands; ++j)  {
                    if (!strcmp(commands[i][j], parsed[1]))   {
                        if (!strcmp(parsed[1], "PING"))  {
                            response = malloc((strlen("+PONG\r\n") + 1) * sizeof(char));
                            strcpy(response, "+PONG\r\n");
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
                        if (!strcmp(parsed[1], "GET"))  {
                            
                        }
                        else if (!strcmp(parsed[1], "DEL"))  {

                        }

                        int nbArgs = atoi(parsed[0])+1;
                        free_arrayString(parsed, nbArgs);

                        return response;
                    }
                }

                break;
            
            case 3:
                while (commands[0][nb_commands] != NULL) {
                    nb_commands++;
                }

                for (int j = 1; j < nb_commands; ++j)  {
                    if (!strcmp(commands[i][j], parsed[1]))   {
                        if (!strcmp(parsed[1], "SET"))  {
                            FILE *fd = fopen("../data.txt", "r");

                            if (fd == NULL) {
                                response = malloc((strlen("+ERROR\r\n") + 1) * sizeof(char));
                                strcpy(response, "+ERROR\r\n");

                                int nbArgs = atoi(parsed[0])+1;
                                free_arrayString(parsed, nbArgs);

                                return response;
                            }
                            
                            fseek(fd, 0, SEEK_END);
                            long fsize = ftell(fd);
                            fseek(fd, 0, SEEK_SET);

                            char *buffer = malloc((fsize + 1 + strlen(parsed[3])) * sizeof(char));
                            fread(buffer, fsize, 1, fd);
                            fclose(fd);

                            buffer[fsize] = '\0';

                            char *word = malloc(TAILLE_MAX_NVAR * sizeof(char));
                            int j = 0;
                            int search = 1;

                            for (int i = 0; i < strlen(buffer); ++i)    {
                                if (search) {
                                    if (buffer[i] == ' ')  {
                                        word[j] = '\0';

                                        if (!strcmp(parsed[2], word)) {
                                            i++;
                                            int k = i;

                                            char *newFile = malloc((fsize + 1 + strlen(parsed[3])) * sizeof(char));

                                            memcpy(newFile, buffer, k);

                                            for (int j = 0; j < strlen(parsed[3]); ++j) {
                                                newFile[k] = parsed[3][j];
                                                k++;
                                            }
                                            newFile[k] = '\n';
                                            k++;

                                            while (buffer[i] != '\n')   {
                                                i++;
                                            }

                                            i++;
                                            while (buffer[i] != '\0')   {
                                                newFile[k] = buffer[i];
                                                k++;
                                                i++;
                                            }
                                            newFile[k] = '\0';

                                            FILE *fd = fopen("../data.txt", "w");

                                            if (fd == NULL) {
                                                fclose(fd);
                                                response = malloc((strlen("+ERROR\r\n") + 1) * sizeof(char));
                                                strcpy(response, "+ERROR\r\n");

                                                int nbArgs = atoi(parsed[0])+1;
                                                free_arrayString(parsed, nbArgs);

                                                return response;
                                            }

                                            fputs(newFile, fd);
                                            fclose(fd);
                                            free(newFile);
                                            free(buffer);
                                            free(word);

                                            response = malloc((strlen("+OK\r\n") + 1) * sizeof(char));
                                            strcpy(response, "+OK\r\n");

                                            int nbArgs = atoi(parsed[0])+1;
                                            free_arrayString(parsed, nbArgs);

                                            return response;
                                        }
                                        else    {
                                            for (int k = 0; k < j; ++k) {
                                                word[k] = '\0';
                                            }
                                        }
                                        j = 0;
                                        search = 0;
                                        continue;
                                    }
                                    word[j] = buffer[i];
                                    j++;
                                }
                                else    {
                                    if (buffer[i] == '\n')  {
                                        search = 1;
                                    }
                                }
                            }
                            free(buffer);
                            free(word);

                            FILE *fp = fopen("../data.txt", "a");
                            
                            if (fp == NULL) {
                                response = malloc((strlen("+ERROR\r\n") + 1) * sizeof(char));
                                strcpy(response, "+ERROR\r\n");

                                int nbArgs = atoi(parsed[0])+1;
                                free_arrayString(parsed, nbArgs);

                                return response;
                            }

                            if (!isNumber(parsed[2][0]))    {
                                fprintf(fp, "%s %s\n", parsed[2], parsed[3]);
                                fclose(fp);

                                response = malloc((strlen("+OK\r\n") + 1) * sizeof(char));
                                strcpy(response, "+OK\r\n");

                                int nbArgs = atoi(parsed[0])+1;
                                free_arrayString(parsed, nbArgs);

                                return response;
                            }
                            else    {
                                fclose(fp);

                                response = malloc((strlen("+ERROR\r\n") + 1) * sizeof(char));
                                strcpy(response, "+ERROR\r\n");

                                int nbArgs = atoi(parsed[0])+1;
                                free_arrayString(parsed, nbArgs);

                                return response;
                            }

                            fclose(fp);
                        }
                    }
                }

            default:
                perror("incorrect number of arg");
                break;
            }

            break;
        }
    }

    response = malloc((strlen("+NOT_FOUND\r\n") + 1) * sizeof(char));
    strcpy(response, "+NOT_FOUND\r\n");

    int nbArgs = atoi(parsed[0])+1;
    free_arrayString(parsed, nbArgs);
    return response;
}