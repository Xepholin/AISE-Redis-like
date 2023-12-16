#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/types.h>
#include <unistd.h>

#include "../include/parser.h"
#include "../include/command.h"

int main(int argc, char const *argv[])
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *result = NULL;

    int ret = getaddrinfo(NULL, argv[1], &hints, &result);

    if (ret < 0)
    {
        herror("getaddrinfo");
        return 1;
    }

    int sock = 0;
    int server_ready = 0;

    struct addrinfo *tmp;
    for (tmp = result; tmp != NULL; tmp = tmp->ai_next)
    {
        sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

        if (sock < 0)
        {
            perror("socket");
            continue;
        }

        if (bind(sock, tmp->ai_addr, tmp->ai_addrlen) < 0)
        {
            perror("bind");
            continue;
        }

        if (listen(sock, 5) < 0)
        {
            perror("listen");
            continue;
        }

        server_ready = 1;
        break;
    }

    if (server_ready == 0)
    {
        fprintf(stderr, "PAS SERVER READY :-'(");
        return 1;
    }

    int client_fd = -1;

    struct sockaddr client_addr;
    socklen_t client_addr_len;

    int i = 0;

    while (1)
    {
        client_fd = accept(sock, &client_addr, &client_addr_len);

        if (client_fd < 0)
        {
            perror("accept");
            break;
        }

        i++;
        pid_t pid = fork();

        if (pid == 0)
        {
            while (1)
            {

                char buffer[128];

                int size = recv(client_fd, buffer, 127, 0);
                printf("size: %d\n", size);

                buffer[size] = '\0';

                char *response = command(buffer);

                if (response != NULL)   {
                    
                    if (write(client_fd, response, size) != -1) {
                        printf("sucess\n");
                    }
                    else    {
                        printf("failed\n");
                    }

                    free(response);
                }

                // sleep(5);
                if (!strcmp(buffer, "exit"))
                {
                    printf("Socket bye");
                    close(client_fd);
                    exit(0);
                }
            }
        }
    }

    close(sock);

    return 0;
}
