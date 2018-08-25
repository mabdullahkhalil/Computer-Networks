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
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <cstring>
#include <fcntl.h>


#include <errno.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

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
		listen(client,1);

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
		char sendd[]="send";
		char receivee[]="receive";

	// cout<<sizeof buffer<<endl;
		char final[]="exit";
		while(exiting){

			memset(buffer,0,buflen);

			recv(server, buffer, buflen, 0);


			if (!strncasecmp(buffer,listserver,strlen(listserver)))
			{
				cout<<"Client: List server"<<endl;

				clientfiles=files();
				string final;

				if (clientfiles.size()<=0)
				{
					final="no files";
					strcpy(buffer, final.c_str());
					break;

				}
				for (int i = 2; i < clientfiles.size(); ++i)
				{
					final+=clientfiles[i]+"\n";
					cout<<" > "<<clientfiles[i]<<endl;

				}

				strcpy(buffer, final.c_str());

				send(server,buffer,strlen(final.c_str()),0);
				continue;
			}
			else if (!strncasecmp(buffer,createserver,strlen(createserver))){
				istringstream string_extraction(buffer);
				string string_distribution[3];

				getline(string_extraction, string_distribution[0], ' ');
				getline(string_extraction, string_distribution[1], ' ');
				getline(string_extraction, string_distribution[2]);
				cout<<"Client: Create server "<<string_distribution[2]<<endl;
				string ch;
				string filename=string_distribution[2];
				if(find(clientfiles.begin(), clientfiles.end(), filename) == clientfiles.end()) {
					clientfiles.push_back(filename);
					ofstream out(filename.c_str());
					cout<<"Server: the file "<<filename<<" has been created"<<endl;
					ch="ok";
				} 	
				else{
					cout<<"Server: the file "<<filename<<" already exists."<<endl;
					ch="nook";
				}
				strcpy(buffer,ch.c_str());
				send(server, buffer, strlen(ch.c_str()), 0);

			}

			if (strcmp(buffer,final)==0)		
			{
			/* code */
				// strcpy(buffer,"hello");

				exiting=false;
			}

			cout<<"\n"<<endl;
		}

		exiting=true;
		cout<<"\nclient connected to server through ip: "<<inet_ntoa(server_address.sin_addr)<<" and port: "<<(int) ntohs(server_address.sin_port)<<" has been disconnected"<<endl;
		close(server);

	}

	cout<<"\nlet's do this again sometime!"<<endl;

	close(client);





}