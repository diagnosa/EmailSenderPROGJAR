#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int i;
    struct hostent *he;
    struct addr_in **addr_list;

    if(argc != 2){
        fprintf(stderr, "usage: ./gethost hostname\n");
        return 1;
    }

    if((he = gethostbyname(argv[1]))== NULL){//get the host info
        herror("gethostbyname");
        return 2;
    }
    
    //print information
    printf("Official name is: %s\n", he->h_name);
    printf("    IP Address: ");
    addr_list = (struct in_addr**)he->h_addr_list;
    for(i=0; addr_list[i] != NULL; i++)
    {
        printf("%s ", inet_ntoa(*addr_list[i]));
    }
    return 0;
}
