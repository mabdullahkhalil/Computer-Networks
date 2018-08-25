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

int main(int argc, char *argv[])
{
  // define your variables here
struct hostent *he;
int i;
//char** addr;
  // check that there are enough parameters
  if (argc != 2)
    {
      fprintf(stderr, "Usage: mydns <hostname>\n");
      exit(-1);
    }
he=gethostbyname(argv[1]);
if (he == NULL)
{
  /* code */
        std::cout<<"invalid input"<<std::endl;
        return 0;


}


printf("Official name is: %s\n", he->h_name);
in_addr** address=(struct in_addr**)he->h_addr_list;
i=0;

char* destip=inet_ntoa(*address[0]);

printf("Address:           %s\n", destip);



  // Address resolution stage by using gethostbyname()
  // Write your code here!
  // Print to standard output

}
// THIS IS A DEPRECATED METHOD OF GETTING HOST NAMES
// use getaddrinfo() instead!

