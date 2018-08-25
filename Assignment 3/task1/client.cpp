#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc <= 2)
	{
		cout<<"Usage: ./simpleclient <hostname> <portnumber>"<<endl;
		exit(-1);
	}


	struct hostent *he;

	struct sockaddr_in server_address;


	int client,server;
	int portnumber=atoi(argv[2]);
	bool exiting=true;
	int buflen=512;
	char buffer[buflen];
	he=gethostbyname(argv[1]);
	in_addr** address=(struct in_addr**)he->h_addr_list;

	char *ip=inet_ntoa(*address[0]);


	cout<<"Running client...."<<endl;

	cout<<"Will try to connect "<<argv[1]<<" at port "<<portnumber<<endl;
	cout<<endl;
	cout<<"hostname: "<<argv[1]<<" has ip: "<<ip<<endl;
	cout<<endl;


	client=socket(AF_INET,SOCK_STREAM,0);
	if (client<0)
	{
		cout<<"socket was not being able to be created"<<endl;
		exit(1);
	}

	bzero((char *) &server_address, sizeof(server_address));
	cout << ".. creating local connector socket"<< endl;
	server_address.sin_family = AF_INET;
	bcopy((char *)he -> h_addr,
		(char *)&server_address.sin_addr.s_addr,
		he -> h_length);





	server_address.sin_port=htons(portnumber);


	if (connect(client,(struct sockaddr *)&server_address, sizeof(server_address)) == 0){
		cout<<".. connecting socket to "<<argv[1]<<":"<<portnumber<<endl;
	}

	memset(buffer,0,buflen);

	recv(client, buffer, buflen, 0);


	cout<<"*****************************"<<endl;
	cout<<"Welcome to SIMPLE server v0.1"<<endl;
	cout<<"*****************************"<<endl;
	cout<<'\n';
	int check;
	// cout<<sizeof buffer<<endl;
	char final[]="exit";
	while(exiting){
		cout<<"c:";
		memset(buffer,0,buflen);

		cin>>buffer;
		check=send(client, buffer, buflen, 0);

		if (strcmp(final,buffer)==0)
		{
			exiting=false;
		}
		if (check<0)
		{
			cout<<"the message was not written to the socket"<<endl;
		}

		int sizercvd=0,totsize=0;
		while(1){
			memset(buffer,0,buflen);

		sizercvd=recv(client, buffer, buflen, 0);
		// cout<<sizeof buffer<<endl;
		// cout<<sizercvd<<endl;
		if (sizercvd < buflen)
		{
			// cout<<"hello2"<<endl;
						cout<<buffer<<endl;

			break;
		}
		else{
			totsize+=sizercvd;
			cout<<buffer;
		}

	}
}




	cout<<"\nlet's do this again sometime!"<<endl;

	close(client);





}