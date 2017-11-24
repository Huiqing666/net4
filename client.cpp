#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


using namespace std;

void GET(int clientDescriptors, char *URL){
	char *request;
	sprintf(request,"GET %s HTTP/1.0\r\n\r\n", URL);
	int data=send(clientDescriptors,request,strlen(request),0);

}

int main(int argc,char *argv[]){
	int clientDescriptors, proxyPort, maxDescriptors=0,clientNumber,input_bytes;
	char *proxyAddr, *URL;
	char *input, errorBuffer[1024], receiveBuffer[1000000];
	struct hostent *host;



	//check arguments
	if (argc != 4) {
        fprintf(stderr,"Missing arguments, please use the form ./client <proxy address> <proxy port> <URL to retrieve>\n");
        exit(1);
    }

	//creat socket for client.
	clientDescriptors = socket(AF_INET, SOCK_STREAM, 0);

	//socket creation failed
	if(clientDescriptors == -1){
		fprintf(stderr, "Create socket failed\n");
		char* errorMessage=strerror_r(errno,errorBuffer,256);
		//printf(errorMessage);
		return -1;
	}

	//get client username, server IP address, server port
	proxyAddr = argv[1];
	proxyPort = atoi(argv[2]);
	URL = argv[3];

    struct sockaddr_in server_sockaddr;
	bzero(&server_sockaddr, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(proxyPort);
	host = gethostbyname(proxyAddr);//getaddrinfo
	memcpy(&server_sockaddr.sin_addr.s_addr, host->h_addr, host->h_length);

	//connect to server
	if(connect(clientDescriptors,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr)) == -1){
		printf("ERROR: Cannot connect to the proxy server\n");
		exit(1);
	}else{
		printf("INFO: Connected to the proxy server\n");
	}

	//send GET request
    GET(clientDescriptors, URL);

    //sending data
	int data=0;
	if((data=recv(clientDescriptors,receiveBuffer,1000000,0))<=0){
		fprintf(stderr,"No data received. Exiting...\n");
		exit(1);
	}else{
		char recvd[1000000];
	        strncpy(recvd,receiveBuffer,data);
		cout<<recvd<<endl;
	}






	//close socket
	close(clientDescriptors);
	return 0;
}

