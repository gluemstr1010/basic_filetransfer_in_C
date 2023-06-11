#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "word.c"

#define SIZE 1024


bool file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    bool is_exist = false;
    if (fp != NULL)
    {
        is_exist = true;
        fclose(fp);
    }
    return is_exist;
}

void write_file(int new_socket)
{
    int b;
    FILE *file;
    const char* filename = malloc(32);
    filename = word();
    char buffer[SIZE];
    while(true)
    {
        if(file_exists(filename))
        {
            filename = word();
        }
        else if ( !file_exists(filename) )
        {
            break;
        }
    }
    // int g = 0;
    // while(true)
    // { 
    //     b = recv(new_socket,buffer,SIZE,0);
    //     if(b > 0)
    //     {
    //         g+=b;
    //     }else
    //     {
    //         break;
    //     }
    // }
    file = fopen(filename,"w");
        while(true)
        {
            b = recv(new_socket,buffer,SIZE,0);
            if(b <= 0)
            {
                //error or connection closed
                break;
            }
            fprintf(file,"%s",buffer);
            bzero(buffer,SIZE);
        }
    
    
    return;
}


int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM,0);
    char buffer[SIZE];
    if( server_socket < 0 )
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    printf("\nSocket was created.\n");
    struct sockaddr_in server_address, new_addr;
    socklen_t addr_size;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7070);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_socket,1);
    printf("\nServer is listening ...\n");
    addr_size = sizeof(new_addr);

    int new_socket = accept(server_socket, (struct sockaddr*)&new_addr, &addr_size );
    write_file(new_socket);

    close(server_socket);
    return 0;
}