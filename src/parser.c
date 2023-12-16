#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../include/parser.h"

void clearBuffer(char *buffer)
{

    for (int i = 0; i < strlen(buffer); ++i)
    {
        buffer[i] = '\0';
    }
}

int getNumberArgs(char *input)
{

    char *args = malloc(strlen(input) * sizeof(char) + 1);

    for (int i = 0; i < strlen(input); ++i)
    {
        if (input[i] == '*')
        {
            i++;
            int j = 0;
            while ((input[i] != ' ') && (input[i] != '\n') && (input[i] != '\0'))
            {
                args[j] = input[i];
                j++;
                i++;
            }
            break;
        }
    }

    int numberOfArgs = atoi(args);
    free(args);
    return numberOfArgs;
}

int isNumber(char c)
{

    if ((c >= '0') && (c <= '9'))
        return 1;
    else
        return 0;
}

void getLenArgs(int *lenArgs, char *input, int nbArgs)
{

    int size = strlen(input);

    int currentNb = 0;
    int i = 0;

    while ((currentNb < nbArgs) && (i < size))
    {
        if (input[i] == '$')
        {
            ++i;

            char *tmp = malloc((size + 1) * sizeof(char));
            int j = 0;

            while (isNumber(input[i]))
            {
                tmp[j] = input[i];
                j++;
                i++;
            }

            lenArgs[currentNb] = atoi(tmp);
            free(tmp);
            currentNb++;
        }
        i++;
    }

    return;
}

char **getStrings(int *lenArgs, int nbArgs, char *input)
{
    int size = strlen(input);
    char **strVal = malloc(sizeof(char *) * nbArgs + 1);

    int nbArgsStringLen = (int)((ceil(log10(nbArgs))+1)*sizeof(char));

    char nbArgsString[nbArgsStringLen];
    sprintf(nbArgsString, "%d", nbArgsStringLen);

    strVal[0] = malloc(sizeof(char) * nbArgsStringLen);

    for (int i = 1; i < nbArgs+1; ++i)
    {
        strVal[i] = malloc(sizeof(char) * (lenArgs[i-1] + 1));
    }

    strcpy(strVal[0], nbArgsString);

    int currentNb = 0;
    int i = 0;

    while ((currentNb < nbArgs+1) && (i < size))
    {
        if (input[i] == '$')
        {
            i++;

            char *tmp = malloc(sizeof(char) * (lenArgs[currentNb] + 1));

            while (isNumber(input[i]))
            {
                i++;
            }

            while ((input[i] == '\n' || input[i] == ' ' || input[i] == 13))
            {
                i++;
            }

            for (int k = 0; k < lenArgs[currentNb]; ++k)
            {
                tmp[k] = input[i];
                i++;
            }
            tmp[lenArgs[currentNb]] = '\0';

            strcpy(strVal[currentNb+1], tmp);

            currentNb++;
        }
        i++;
    }
    return strVal;
}

char **parser(char *input)
{

    int numberOfArgs = getNumberArgs(input);
    int lenArgs[numberOfArgs];
    getLenArgs(lenArgs, input, numberOfArgs);

    char **parsedStr = getStrings(lenArgs, numberOfArgs, input);

    return parsedStr;
}
