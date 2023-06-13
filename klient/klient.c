#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#define SIZE 1024

struct mmsghdr
{
    struct msghdr msg_hdr;
    unsigned int msg_len;
};

long get_file_size(char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp==NULL)
        return -1;

    if (fseek(fp, 0, SEEK_END) < 0) {
        fclose(fp);
        return -1;
    }

    long size = ftell(fp);
    
    fclose(fp);
    return size;
}

void send_file(FILE *file, int net_socket)
{
    int n;
    char data[SIZE] = {0};
 
  while(fgets(data, SIZE, file) != NULL)
   {
    if (send(net_socket, data, sizeof(data), 0) == -1)
    {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
   }
}

int main(int argc, char *argv[])
{
    char *filename = argv[2];
    char buffer[SIZE];
    if( argc > 2 )
    {
        if( strcmp(filename,"--file") == 1 )
        {
            perror("\nNo file given! (argument:[--file FILE])\n");
            exit(EXIT_FAILURE);
        }
        else if( strcmp(filename,"--file") == 0 )
        {
            if(sizeof(argv[2]) > 32)
            {
                perror("\nFile name is too long!\n");
                exit(EXIT_FAILURE);
            }
            
        }
    }else
    {
        printf("\n No file given! (argument:[--file FILE]) \n");
        exit(EXIT_FAILURE);
    }

    int net_socket = socket(AF_INET,SOCK_STREAM,0);

    FILE *file;

    if( net_socket < 0 )
    {
        perror("\nsocket failed. exiting ...\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address, new_addr;
    socklen_t addr_size;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7070);
    server_address.sin_addr.s_addr = INADDR_ANY;

   

    int conn_status = connect(net_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if( conn_status == -1 )
    {
        perror("\nerror when connecting to remote socket. exiting ...\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(filename,"r");
    if(file == NULL)
    {
        perror("\nerror when reading file, exiting ...\n");
        exit(EXIT_FAILURE);
    }

    long fileSize = get_file_size(filename);
    send_file(file, net_socket);
    // addr_size = sizeof(new_addr);
    // int acc = accept(net_socket, (struct sockaddr*)&new_addr,&addr_size);
    // char server_res[256];
    // int b = recv(acc,&server_res,sizeof(server_res),0);
    // if(b > 0)
    // {
    //     printf("%s",server_res);
    // }else
    // {
    //     printf("\nFile sent successfully.\n");
    // }

    printf("\nClosing connection ... \n");
    close(net_socket);

    return 0;
}