
#include "../include/parser.h"

#include <string.h>

char* parser(char* input){

    if(!strcmp(input, "PING"))   {
        return "PONG";
    }

    return '\0'   ;

}

void clearBuffer(char *buffer){

    for(int i = 0; i < strlen(buffer) ; ++i){
        buffer[i] = '\0';
    }
}