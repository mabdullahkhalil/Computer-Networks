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
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;


vector<string> files(){
	vector<string> clientfiles;
	string dir=string(".");
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(dir.c_str())) == NULL) {
		cout << "Error() opening " << dir << endl;

	}

	while ((dirp = readdir(dp)) != NULL) {
		string s=(dirp->d_name);
		if (s != "." || s!=". .")
		{
					/* code */
			if(find(clientfiles.begin(), clientfiles.end(), s) == clientfiles.end()) {
				clientfiles.push_back((dirp->d_name));
			} 
		}


	}
	closedir(dp);
	return clientfiles;
}

int main(int argc, char *argv[])
{
	if (argc <= 2)
	{
		cout<<"Usage: ./server <hostname> <portnumber>"<<endl;
		exit(-1);
	}
	if (atoi(argv[2])<1024 || atoi(argv[2])>65535 )
	{
		cout<<" Usage: ./server <hostname> <port number between 1024 and 65535>: "<<endl;
		exit(1);
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


	cout<<"Starting to run server at port "<<portnumber<<endl;



	client=socket(AF_INET,SOCK_STREAM,0);
	if (client<0)
	{
		cout<<"socket was not being able to be created"<<endl;
		exit(1);
	}

	bzero((char *) &server_address, sizeof(server_address));
	cout << ".. creating local listener socket"<< endl;
	server_address.sin_family = AF_INET;
	bcopy((char *)he -> h_addr,
		(char *)&server_address.sin_addr.s_addr,
		he -> h_length);



    // server_address.sin_addr.s_addr = htons(inet_ntoa(*address[0]));


	server_address.sin_port=htons(portnumber);


	if (bind(client,(struct sockaddr *)&server_address, sizeof(server_address)) == 0){
		cout<<".. connecting socket to port:"<<portnumber<<endl;
	}
	else{
		cout<<"cannot connect to the port. try with a different port number."<<endl;
		exit(1);
	}




	while(1){

		cout<<".. starting to listen at the port"<<endl;
		listen(client,5);

		socklen_t size;
		size=sizeof(server_address);

		cout<<".. waiting for connection"<<endl;


		server = accept(client,(struct sockaddr *)&server_address,&size);

		cout<<"\nclient connected to server through ip: "<<inet_ntoa(server_address.sin_addr)<<" and port: "<<(int) ntohs(server_address.sin_port)<<endl;



		if (server < 0) 
			cout << "=> Error on accepting..." << endl;

	// memset(buffer,0,buflen);
		strcpy(buffer,"Welcome To SIMPLE server v0.1");

		send(server, buffer, buflen, 0);

		cout<<'\n';
		int check;
		vector<string> clientfiles;

		char listserver[]="list server";
		char createserver[]="create server";
		char deleteserver[]="delete server";

		char final[]="exit";
		while(exiting){
			memset(buffer,0,buflen);

			recv(server, buffer, buflen, 0);
			if (strcmp(buffer,final)==0)		
			{
				exiting=false;
			}


		}

		exiting=true;
		cout<<"\nclient connected to server through ip: "<<inet_ntoa(server_address.sin_addr)<<" and port: "<<(int) ntohs(server_address.sin_port)<<" has been disconnected"<<endl;
		close(server);

	}

	cout<<"\nlet's do this again sometime!"<<endl;

	close(client);





}