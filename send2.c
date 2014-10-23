#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUF 1024

void encodeblock( unsigned char in[], char b64str[], int len );
void b64_encode(char *clrstr, char *dst);


int main(int argc, char *argv[]){
	int 	sockfd, retval;
	struct 	in_addr *ptr;
	struct 	sockaddr_in servaddr;
	char 	buff[BUF];
	char 	IP_Address[16];
	unsigned int echo;
	unsigned short port;
	int 	receive=0;
	struct 	hostent *host;
	char 	myuse[BUF];
	char 	mypas[BUF];
  	char 	myu64[BUF] = "";
  	char 	myp64[BUF] = "";

	if(argc<2){fprintf(stderr, "USAGE: <hostname> <port>\n");exit(1);	}
	host = gethostbyname(argv[1]);
	port = htons(atoi(argv[2]));

	if(host==NULL){herror("gethostbyname");exit(-1);}
																	//printf("Official name is: %s\n", host->h_name);
																	//printf("IP_Address: ");
	ptr=(struct in_addr*)*(host->h_addr_list);
																	//printf("%u %s \n", ptr->s_addr, inet_ntoa(*ptr));
																	//create tcp socket
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0){printf("failed make socket\n");}
	printf("Success\n");

																	//make structure socket
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=ptr->s_addr;
	servaddr.sin_port=port;

																	//connect
	retval =connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
	if(retval<0){printf("failed make connection with server\n");}
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s\n", buff);
															//send request to the server
	char msg[]="EHLO ";
	strcpy(IP_Address,argv[1]);
	strcat(msg,IP_Address);
	strcat(msg,"\r\n");
	printf("%s",msg);
	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);
	
	strcpy(msg,"AUTH LOGIN\r\n");
	printf("%s",msg);
	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);


	char username[32], password[32];
	printf("Username : ");
	scanf("%s", username);
	printf("Password : ");
	scanf("%s", password);

	strcpy(myuse,username);
	b64_encode(myuse, myu64);
	sprintf(msg,"%s\r\n",myu64);
	
	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);

	strcpy(mypas,password);
	b64_encode(mypas, myp64);
	sprintf(msg,"%s\r\n",myp64);

	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);

	
	strcpy(msg,"MAIL FROM:tes@its-sby.edu\r\n");
	printf("%s",msg);
	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);


	char send[64];
	printf("Enter Email: ");
	scanf("%s", send);
	sprintf(msg,"RCPT TO:%s\r\n",send);
	printf("%s",msg);
	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);

	strcpy(msg,"data\r\n");
	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);

	char subject[16];
	printf("Subject: ");
	scanf("%s", subject);
	printf("Message: ");
	scanf("%s", send);
	sprintf(msg,"subject:%s\n%s\r\n.\r\n",subject,send);
	 

	write(sockfd,msg,strlen(msg));
	retval=read(sockfd,buff, sizeof(buff)-1);
	buff[retval]='\0';
	printf("%s", buff);

	close(sockfd);
	printf("close");
	exit(1);
	return 0;
}

char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* encodeblock - encode 3 8-bit binary bytes as 4 '6-bit' characters */
void encodeblock( unsigned char in[], char b64str[], int len ) {
    unsigned char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
             ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, out, sizeof(out));
}

/* encode - base64 encode a stream, adding padding if needed */
void b64_encode(char *clrstr, char *dst) {
  unsigned char in[3];
  int i, len = 0;
  int j = 0;

  b64dst[0] = '\0';
  while(clrstr[j]) {
    len = 0;
    for(i=0; i<3; i++) {
     in[i] = (unsigned char) clrstr[j];
     if(clrstr[j]) {
        len++; j++;
      }
      else in[i] = 0;
    }
    if( len ) {
      encodeblock( in, b64dst, len );
    }
  }
}
