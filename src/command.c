#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/parser.h"

#define NOMBRE_ARGS 3
#define NOMBRE_MAX_COMMANDS_ARGS 25

#define TAILLE_MAX_NVAR 15

const char *commands[NOMBRE_ARGS][NOMBRE_MAX_COMMANDS_ARGS] = {
    {"1", "PING"},
    {"2", "GET", "DEL", "INCR", "DECR"},
    {"3", "SET"}};

void free_arrayString(char **parsedStr, int nbArgs)
{

    for (int i = 0; i < nbArgs; i++)
    {
        free(parsedStr[i]);
    }
}

int rewriteFile(FILE *fd, char *nvar, char *value)
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

    char *key = malloc(strlen(nvar) + 3);
    snprintf(key, strlen(nvar) + 3, "'%s'", nvar);

    fseek(fd, 0, SEEK_SET);

    while (fgets(buffer, fsize, fd) != NULL)
    {
        char *substring = strstr(buffer, key);
        if (substring != NULL)
        {
            fprintf(tempFile, "'%s' %s\n", nvar, value);
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

    char *key = malloc(strlen(parsed[2]) + 3);
    snprintf(key, strlen(parsed[2]) + 3, "'%s'", parsed[2]);

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

int fileAppend(char *nvar, char *value)
{
    FILE *fa = fopen("../data.txt", "a");

    if (fa == NULL)
    {
        return 0;
    }

    if (!isNumber(nvar[0]))
    {
        fprintf(fa, "'%s' %s\n", nvar, value);
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
    snprintf(key, strlen(parsed[2]) + 3, "'%s'", parsed[2]);

    fseek(fd, 0, SEEK_SET);

    while (fgets(buffer, fsize, fd) != NULL)
    {
        if (strcmp(buffer, "\n") && fsize == 1)
        {
            break;
        }

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
    snprintf(key, strlen(parsed[2]) + 3, "'%s'", parsed[2]);

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

            char *response = malloc((sizeof(res) + 4) * sizeof(char));
            snprintf(response, sizeof(res) + 4, ":%d\r\n", atoi(res));
            free(res);
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
                while (commands[1][nb_commands] != NULL)
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

                            int boolKey = findKey(fd, parsed);

                            if (boolKey == 0)
                            {
                                fclose(fd);
                                return ERROR(parsed);
                            }

                            char *response = getKeyValue(fd, parsed);

                            fclose(fd);
                            int nbArgs = atoi(parsed[0]) + 1;
                            free_arrayString(parsed, nbArgs);

                            return response;
                        }
                        else if (!strcmp(parsed[1], "DEL"))
                        {
                            FILE *fd = fopen("../data.txt", "r");

                            if (fd == NULL)
                            {
                                return ERROR(parsed);
                            }

                            int boolKey = findKey(fd, parsed);

                            if (boolKey == 0)
                            {
                                fclose(fd);
                                return ERROR(parsed);
                            }

                            deleteVariable(fd, parsed);
                            fclose(fd);

                            response = malloc((sizeof(parsed[2]) + 6) * sizeof(char));
                            snprintf(response, sizeof(parsed[2]) + 6, "+Del:%s\r\n", parsed[2]);

                            int nbArgs = atoi(parsed[0]) + 1;
                            free_arrayString(parsed, nbArgs);

                            return response;
                        }
                        else if (!strcmp(parsed[1], "INCR"))
                        {
                            FILE *fd = fopen("../data.txt", "r");

                            if (fd == NULL)
                            {
                                return ERROR(parsed);
                            }

                            int boolKey = findKey(fd, parsed);

                            if (boolKey == 0)
                            {
                                if (fileAppend(parsed[2], "0"))
                                {
                                    fclose(fd);
                                    return OK(parsed);
                                }
                                else
                                {
                                    fclose(fd);
                                    return ERROR(parsed);
                                }
                            }

                            char *keyValue = getKeyValue(fd, parsed);
                            keyValue[strcspn(keyValue, "\r\n")] = 0;
                            memmove(keyValue, keyValue + 1, strlen(keyValue));

                            int varValue = atoi(keyValue);
                            varValue++;
                            free(keyValue);

                            int len = (int)((ceil(log10(varValue)) + 1) * sizeof(char));
                            char *string = malloc(len * sizeof(char));
                            sprintf(string, "%d", varValue);

                            if (rewriteFile(fd, parsed[2], string))
                            {
                                free(string);
                                fclose(fd);
                                return OK(parsed);
                            }
                            else
                            {
                                free(string);
                                fclose(fd);
                                return ERROR(parsed);
                            }
                        }
                        else if (!strcmp(parsed[1], "DECR"))
                        {
                            FILE *fd = fopen("../data.txt", "r");

                            if (fd == NULL)
                            {
                                return ERROR(parsed);
                            }

                            int boolKey = findKey(fd, parsed);

                            if (boolKey == 0)
                            {
                                if (fileAppend(parsed[2], "0"))
                                {
                                    fclose(fd);
                                    return OK(parsed);
                                }
                                else
                                {
                                    fclose(fd);
                                    return ERROR(parsed);
                                }
                            }

                            char *keyValue = getKeyValue(fd, parsed);
                            keyValue[strcspn(keyValue, "\r\n")] = 0;
                            memmove(keyValue, keyValue + 1, strlen(keyValue));

                            int varValue = atoi(keyValue);
                            varValue--;
                            free(keyValue);

                            int len = (int)((ceil(log10(varValue)) + 1) * sizeof(char));
                            char *string = malloc(len * sizeof(char));
                            sprintf(string, "%d", varValue);

                            if (rewriteFile(fd, parsed[2], string))
                            {
                                free(string);
                                fclose(fd);
                                return OK(parsed);
                            }
                            else
                            {
                                free(string);
                                fclose(fd);
                                return ERROR(parsed);
                            }
                        }
                    }
                }

                break;

            case 3:
                while (commands[2][nb_commands] != NULL)
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
                                if (fileAppend(parsed[2], parsed[3]))
                                {
                                    fclose(fd);
                                    return OK(parsed);
                                }
                                else
                                {
                                    fclose(fd);
                                    return ERROR(parsed);
                                }
                            }
                            if (rewriteFile(fd, parsed[2], parsed[3]))
                            {
                                fclose(fd);
                                return OK(parsed);
                            }
                            else
                            {
                                fclose(fd);
                                return ERROR(parsed);
                            }
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