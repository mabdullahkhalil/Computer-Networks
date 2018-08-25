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
		cout<<"Usage: ./client <hostname> <portnumber>"<<endl;
		exit(-1);
	}
	if (atoi(argv[2])<1024 || atoi(argv[2])>65535)
	{
		cout<<" Usage: ./client <hostname> <port number between 1024 and 65535>: "<<endl;
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
	cout<<buffer<<endl;
	cout<<"*****************************"<<endl;
	cout<<'\n';
	int check;
    // cout<<sizeof buffer<<endl;
	char final[]="exit";
	char listclient[]="LIST client";
	char listserver[]="LIST server";
	char createclient[]="create client";
	char createserver[]="create server";
	char deleteclient[]="delete client";
	char deleteserver[]="delete server";

	vector<string> clientfiles;
	vector<string> serverfiles;



	while(exiting){
		cout<<"client:";
		memset(buffer,0,buflen);
		string str;
		getline(cin,str);
		strcpy(buffer, str.c_str());

		if (!strncasecmp(buffer,final,strlen(final)))
		{
			send(client, buffer, buflen, 0);

			exiting=false;

		}
		
		

		recv(client, buffer, buflen, 0);
		cout<<buffer<<endl;


		cout<<"\nlet's do this again sometime!"<<endl;

		close(client);





	}

}