#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUF 32

int main(int argc, char *argv[]){
	int sockfd;
	struct in_addr *ptr;
	struct sockaddr_in servaddr;
	char buff[32];
	unsigned int echo;
	int receive=0;
	struct hostent *host;

	if(argc!=3){
		fprintf(stderr, "USAGE TCPecho <server_ip> <port>\n");
		exit(1);
	}
	host = gethostbyname(argv[1]);
	if(host==NULL){
		herror("gethostbyname");
		exit(-1);
	}
	printf("Official name is: %s\n", host->h_name);
	printf("IP_Address: ");
	ptr=(struct in_addr*)*(host->h_addr_list);
	printf("%u %s \n", ptr->s_addr, inet_ntoa(*ptr));

	//create tcp socket
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("failed make socket\n");
	}
	printf("success make socket\n");

	//make structure socket
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("202.46.129.82");
	servaddr.sin_port=htons(25);

	//connect
	int retval;
	retval =connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
	
	if(retval<0)
	{
		printf("failed make connection with server\n");
	}

	//send the word to the server
	char msg[]="EHLO 202.46.129.82";
	int length= strlen(msg);
	if(send(sockfd,msg,length,0)!=length){
		printf("Mismatch in number of sent bytes");
	}
	printf("success mengirim pesan\n");
	while(receive < length){
		int bytes=0;
		if((bytes=recv(sockfd, buff, BUF -1, 0 ))<1){
			printf("failed to receive bytes from server");
		}
		receive += bytes;
		buff[bytes]='\0'; 
		fprintf(stdout, buff);
		printf("a");
	}
	msg[]="EHLO 202.46.129.82";
	length= strlen(msg);
	if(send(sockfd,msg,length,0)!=length){
		printf("Mismatch in number of sent bytes");
	}
	fprintf(stdout, "\n" );
		while(receive < length){
		bytes=0;
		if((bytes=recv(sockfd, buff, BUF -1, 0 ))<1){
			printf("failed to receive bytes from server");
		}
		receive += bytes;
		buff[bytes]='\0'; 
		fprintf(stdout, buff);
		printf("b");
	}

	close(sockfd);
	printf("close");
	exit(1);
	return 0;
}
