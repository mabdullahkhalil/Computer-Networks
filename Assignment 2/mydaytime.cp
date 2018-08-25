#include <iostream>
#include <stdio.h> // basic I/O
#include <stdlib.h>
#include <sys/types.h> // standard system types
#include <netinet/in.h> // Internet address structures
#include <sys/socket.h> // socket API
#include <arpa/inet.h>
#include <netdb.h> // host to IP resolution
#include <string.h>
#include <unistd.h>

#define HOSTNAMELEN 40 // maximal host name length; can make it variable if you want
#define BUFLEN 1024 // maximum response size; can make it variable if you want
#define PORT 13 // port of daytime server

int main(int argc, char *argv[])
{
  // define your variables here
struct hostent *he;

  // check that there are enough parameters
  if (argc != 2)
    {
      fprintf(stderr, "Usage: mydaytime <hostname>\n");
      exit(-1);
    }

  // Write your code here
    he=gethostbyname(argv[1]);
if (he == NULL)
{
  /* code */
        std::cout<<"invalid input"<<std::endl;
        return 0;


}
    printf("Official name is:  %s\n", he->h_name);
in_addr** address=(struct in_addr**)he->h_addr_list;

char* destip=inet_ntoa(*address[0]);

sockaddr_in socket_addr;
socket_addr.sin_family	= AF_INET;
socket_addr.sin_port	= htons(PORT);
socket_addr.sin_addr.s_addr=  inet_addr(destip); 
sockaddr* socket_addrn = (sockaddr*)&socket_addr;
    memset(&(socket_addr.sin_zero), '\0', 8);  // zero the rest of the struct


int sock = socket(AF_INET,SOCK_STREAM,0);

std::cout<<"Address:           "<< inet_ntoa(*address[0]) <<std::endl;


   int mconnect= connect(sock, (struct sockaddr *)&socket_addr, sizeof(struct sockaddr));


if(sock == -1){
	std::cout<<"no Socket"<<std::endl;
}

if(mconnect == -1){
	std::cout<<"NO CONNECTION"<<std::endl;
}

char time[BUFLEN -1];
int receiveing = recv(sock, time, BUFLEN, 0);
if(receiveing == -1){
	std::cout<<"no get"<<std::endl;
}

if(receiveing == 0){
	std::cout<<"remote conn. closed"<<std::endl;
}

std::cout<<"time of server:    "<< time <<std::endl;
close(sock);

  return 0;

  return 0;
}
