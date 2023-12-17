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
    {"3", "SET"}};

void free_arrayString(char **parsedStr, int nbArgs)
{

    for (int i = 0; i < nbArgs; i++)
    {
        free(parsedStr[i]);
    }
}

int rewriteFile(FILE *fd, char **parsed)
{

    FILE *tempFile = fopen("../temp.txt", "w");
    if (tempFile == NULL)
    {
        return 0;
    }

    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char buffer[fsize];
    fread(buffer, fsize, 1, fd);

    // Regarde si la clef est dans le fichier
    int findKey = 0;

    char *key = malloc(strlen(parsed[2]) + 3);
    snprintf(key, sizeof(key), "'%s'", parsed[2]);

    fseek(fd, 0, SEEK_SET);

    while (fgets(buffer, fsize, fd) != NULL)
    {
        char *substring = strstr(buffer, key);
        if (substring != NULL)
        {
            fprintf(tempFile, "'%s' %s\n", parsed[2], parsed[3]);
        }
        else
        {
            fprintf(tempFile, "%s", buffer);
        }
    }

    fclose(tempFile);
    free(key);

    if (rename("../temp.txt", "../data.txt") != 0)
    {
        printf("Erreur rewrite\n");
        return 0;
    }

    return 1;
}

int deleteVariable(FILE *fd, char **parsed)
{

    FILE *tempFile = fopen("../temp.txt", "w");
    if (tempFile == NULL)
    {
        return 0;
    }

    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char buffer[fsize];
    fread(buffer, fsize, 1, fd);

    // Regarde si la clef est dans le fichier
    int findKey = 0;

    char *key = malloc(strlen(parsed[2]) + 3);
    snprintf(key, sizeof(key), "'%s'", parsed[2]);

    fseek(fd, 0, SEEK_SET);

    while (fgets(buffer, fsize, fd) != NULL)
    {
        char *substring = strstr(buffer, key);
        if (substring != NULL)
        {
            continue;
        }
        else
        {
            fprintf(tempFile, "%s", buffer);
        }
    }

    fclose(tempFile);
    free(key);

    if (rename("../temp.txt", "../data.txt") != 0)
    {
        printf("Erreur rewrite\n");
        return 0;
    }

    return 1;
}

int fileAppend(char **parsed)
{
    FILE *fa = fopen("../data.txt", "a");

    if (fa == NULL)
    {
        return 0;
    }

    if (!isNumber(parsed[2][0]))
    {
        fprintf(fa, "'%s' %s\n", parsed[2], parsed[3]);
    }
    fclose(fa);
    return 1;
}
char *ERROR(char **parsed)
{

    char *response = malloc((strlen("+ERROR\r\n") + 1) * sizeof(char));
    strcpy(response, "+ERROR\r\n");
    int nbArgs = atoi(parsed[0]) + 1;
    free_arrayString(parsed, nbArgs);
    return response;
}

char *OK(char **parsed)
{
    char *response = malloc((strlen("+OK\r\n") + 1) * sizeof(char));
    strcpy(response, "+OK\r\n");
    int nbArgs = atoi(parsed[0]) + 1;
    free_arrayString(parsed, nbArgs);
    return response;
}

int findKey(FILE *fd, char **parsed)
{

    // char *buffer = malloc((fsize + strlen(parsed[2]) + strlen(parsed[3])) * sizeof(char));

    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char buffer[fsize];
    fread(buffer, fsize, 1, fd);

    char *key = malloc(strlen(parsed[2]) + 3);
    snprintf(key, sizeof(key), "'%s'", parsed[2]);

    fseek(fd, 0, SEEK_SET);

    while (fgets(buffer, fsize, fd) != NULL)
    {

        char *substring = strstr(buffer, key);
        if (substring != NULL)
        {
            free(key);
            return 1;
        }
    }
    free(key);
    return 0;
}

char *getKeyValue(FILE *fd, char **parsed)
{

    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char buffer[fsize];
    fread(buffer, fsize, 1, fd);

    char *key = malloc(strlen(parsed[2]) + 3);
    snprintf(key, sizeof(key), "'%s'", parsed[2]);

    fseek(fd, 0, SEEK_SET);
    char *res;

    while (fgets(buffer, fsize, fd) != NULL)
    {

        char *substring = strstr(buffer, key);
        if (substring != NULL)
        {
            res = malloc(strlen(buffer) - strlen(key));
            int i = strlen(key) + 1;
            int j = 0;
            while (isNumber(buffer[i]))
            {
                res[j] = buffer[i];
                j++;
                i++;
            }
            free(key);

            char * response = malloc((sizeof(res) + 6) * sizeof(char));
            snprintf(response, sizeof(res) + 6, ":%d\r\n\0", atoi(res));
            return response;
        }
    }

    free(key);

    res = malloc((strlen("+NOT_FOUND\r\n") + 1) * sizeof(char));
    strcpy(res, "+NOT_FOUND\r\n");

    return res;
}

char *command(char *input)
{

    char **parsed = parser(input);
    char *response = NULL;

    for (int i = 0; i < NOMBRE_ARGS; ++i)
    {

        if (atoi(parsed[0]) == atoi(commands[i][0]))
        {
            int nb_commands = 0;

            switch (atoi(parsed[0]))
            {
            case 1:
                while (commands[0][nb_commands] != NULL)
                {
                    nb_commands++;
                }

                for (int j = 1; j < nb_commands; ++j)
                {
                    if (!strcmp(commands[i][j], parsed[1]))
                    {
                        if (!strcmp(parsed[1], "PING"))
                        {
                            response = malloc((strlen("+PONG\r\n") + 1) * sizeof(char));
                            strcpy(response, "+PONG\r\n");
                        }

                        int nbArgs = atoi(parsed[0]) + 1;
                        free_arrayString(parsed, nbArgs);

                        return response;
                    }
                }

                break;

            case 2:
                while (commands[0][nb_commands] != NULL)
                {
                    nb_commands++;
                }

                for (int j = 1; j <= nb_commands; ++j)
                {


                    if (!strcmp(commands[i][j], parsed[1]))
                    {

                        if (!strcmp(parsed[1], "GET"))
                        {

                            FILE *fd = fopen("../data.txt", "r");

                            if (fd == NULL)
                            {
                                return ERROR(parsed);
                            }
                            char *response = getKeyValue(fd, parsed);

                            fclose(fd);
                            return response;
                        }
                        else if (!strcmp(parsed[1], "DEL"))
                        {
                            FILE *fd = fopen("../data.txt", "r");
                            deleteVariable(fd, parsed);
                            fclose(fd);
                            response = malloc((sizeof(parsed[2]) + 6) * sizeof(char));
                            snprintf(response, sizeof(parsed[2]) + 6, "+Del:%s\r\n", parsed[2]);
                            return response;
                        }

                        int nbArgs = atoi(parsed[0]) + 1;
                        free_arrayString(parsed, nbArgs);

                        return response;
                    }
                }

                break;

            case 3:
                while (commands[0][nb_commands] != NULL)
                {
                    nb_commands++;
                }

                for (int j = 1; j < nb_commands; ++j)
                {
                    if (!strcmp(commands[i][j], parsed[1]))
                    {
                        if (!strcmp(parsed[1], "SET"))
                        {

                            FILE *fd = fopen("../data.txt", "r");

                            if (fd == NULL)
                            {
                                return ERROR(parsed);
                            }

                            int boolKey = findKey(fd, parsed);

                            if (boolKey == 0)
                            {
                                if (fileAppend(parsed))
                                {
                                    return OK(parsed);
                                }
                                else
                                {
                                    return ERROR(parsed);
                                }
                            }
                            else
                            {
                                if (rewriteFile(fd, parsed))
                                {
                                    return OK(parsed);
                                }
                                else
                                {
                                    return ERROR(parsed);
                                }
                            }
                            fclose(fd);
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

    int nbArgs = atoi(parsed[0]) + 1;
    free_arrayString(parsed, nbArgs);
    return response;
}