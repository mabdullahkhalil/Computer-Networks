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
	char sendd[]="send";
	char receivee[]="receive";

	vector<string> clientfiles;
	vector<string> serverfiles;



	while(exiting){
		cout<<"\nclient:";
		memset(buffer,0,buflen);
		string str;
		str.clear();
		getline(cin,str);
		strcpy(buffer, str.c_str());

		if (!strncasecmp(buffer,listclient,strlen(listclient)))
		{

			clientfiles=files();
			for (int i = 2; i < clientfiles.size(); ++i)
			{
				cout<<" > "<<clientfiles[i]<<endl;
			}

			continue;
		}
		else if (!strncasecmp(buffer,listserver,strlen(listserver)))
		{
			check=send(client, buffer, buflen, 0);
			if (check<0)
			{
				cout<<"the message was not written to the socket"<<endl;
			}
			int sizercvd=0,totsize=0;
			while(1){
                //memset(buffer,0,buflen);

				sizercvd=recv(client, buffer, buflen, 0);
                // cout<<sizeof buffer<<endl;
				if (sizercvd <= buflen)
				{
                    // cout<<"hello2"<<endl;
					//cout<<buffer<<endl;
					break;
				}
				else{
					totsize+=sizercvd;
					//cout<<buffer;
				}

			}
			string x;
			for (int i = 0;buffer[i]!='\0'; ++i)
			{

				if (buffer[i]!='\n'|| buffer[i]!='\0')
				{
					x+=buffer[i];
				}
				if (buffer[i]=='\n'|| buffer[i]=='\0'){
					cout<<" > "<<x;
					serverfiles.push_back(x);
					x.clear();
				}

			}

		}

		else if (!strncasecmp(buffer,final,strlen(final)))
		{
			send(client, buffer, buflen, 0);

			exiting=false;

		}
		else if (!strncasecmp(buffer,createclient,strlen(createclient)) || !strncasecmp(buffer,createserver,strlen(createserver)))
		{
			istringstream string_extraction(buffer);
			string string_distribution[3];

			getline(string_extraction, string_distribution[0], ' ');
			getline(string_extraction, string_distribution[1], ' ');
			getline(string_extraction, string_distribution[2]);

			if (strlen(string_distribution[2].c_str())==0)
			{
				cout<<"wrong input : Enter the filename too. format 'create server/client <filename>' "<<endl;
			}

			else if ((string_distribution[1]=="client"))
			{
				string filename=string_distribution[2];

				if(find(clientfiles.begin(), clientfiles.end(), filename) == clientfiles.end()) {
					clientfiles.push_back(filename);
					ofstream out(filename.c_str());
					cout<<"Client: the file "<<filename<<" has been created"<<endl;
				}
				else{
					cout<<"Client: the file "<<filename<<" already exists."<<endl;
				}

			}
			else if((string_distribution[1]=="server")){
				string filename=string_distribution[2];
				send(client, buffer, buflen, 0);
				recv(client, buffer, buflen, 0);
				string str;
				for (int i = 0; i<4; ++i)
				{
					str+=buffer[i];
				}
				if (str!="nook")
				{
					cout<<"Server: the file "<<filename<<" has been created"<<endl;
					serverfiles.push_back(filename);

				}
				else{
					cout<<"Server: the file "<<filename<<" already exists."<<endl;
				}

			}

		}
		else if (!strncasecmp(buffer,deleteclient,strlen(deleteclient)) || !strncasecmp(buffer,deleteserver,strlen(deleteserver)))
		{
			istringstream string_extraction(buffer);
			string string_distribution[3];

			getline(string_extraction, string_distribution[0], ' ');
			getline(string_extraction, string_distribution[1], ' ');
			getline(string_extraction, string_distribution[2]);

			if (strlen(string_distribution[2].c_str())==0)
			{
				cout<<"wrong input : Enter the filename too. format 'delete server/client <filename>' "<<endl;
			}

			else if ((string_distribution[1]=="client"))
			{
				clientfiles=files();
				string filename=string_distribution[2];

				if(find(clientfiles.begin(), clientfiles.end(), filename) != clientfiles.end()) {
					if( remove( filename.c_str() ) != 0 )
						cout<<"error deleting file. may be open file. close it before."<<endl;
					else
						clientfiles.erase(remove(clientfiles.begin(), clientfiles.end(), filename), clientfiles.end());
					cout<<"Client: the file "<<filename<<" has been deleted"<<endl;

				} 	
				else{
					cout<<"Client: the file "<<filename<<" does not exist."<<endl;
				}

			}
			else if((string_distribution[1]=="server")){
				string filename=string_distribution[2];
				send(client, buffer, buflen, 0);
				recv(client, buffer, buflen, 0);
				string str;
				for (int i = 0; i<4; ++i)
				{
					str+=buffer[i];
				}
				if (str!="nook")
				{
					cout<<"Server: the file "<<filename<<" has been deleted"<<endl;
					serverfiles.erase(remove(serverfiles.begin(), serverfiles.end(), filename), serverfiles.end());

				}
				else{
					cout<<"Server: the file "<<filename<<" does not exist."<<endl;
				}

			}

		}

		else if (!strncasecmp(buffer,sendd,strlen(sendd))) {
			istringstream string_extraction(buffer);
			string string_distribution[2];

			getline(string_extraction, string_distribution[0], ' ');
			getline(string_extraction, string_distribution[1]);

			if (strlen(string_distribution[1].c_str())==0)
			{
				cout<<"wrong input : Enter the filename too. format 'send <filename>' "<<endl;
			}

			else {//////////////////////////////////////////
				string filename=string_distribution[1];
				struct stat file_s;
				send(client,buffer, sizeof(buffer), 0);

				memset(buffer,0,buflen);

				int file=open(filename.c_str(),O_RDONLY);
				fstat(file,&file_s);
				int filesize=file_s.st_size;
				snprintf(buffer, sizeof(buffer), "%d", filesize);
				
				send(client,buffer, sizeof(buffer), 0);

				memset(buffer,0,buflen);

				off_t checkset=0;

				int senddata=0;
				while (((senddata = sendfile(client,file,&checkset,buflen)) > 0) && (filesize>0)){
					filesize-=senddata;
					//memset(buffer,0,buflen);

				}
				recv(client, buffer, buflen, 0);
				string str;
				for (int i = 0; i<4; ++i)
				{
					str+=buffer[i];
				}
				if (str!="nook")
				{
					cout<<"Server: the file "<<filename<<" received."<<endl;
					serverfiles.push_back(filename);

				}
				else{
					cout<<"Server: the file "<<filename<<" already exists on server."<<endl;
				}
			}
		}



		else if (!strncasecmp(buffer,receivee,strlen(receivee))) {

			istringstream string_extraction(buffer);
			string string_distribution[2];

			getline(string_extraction, string_distribution[0], ' ');
			getline(string_extraction, string_distribution[1]);

			if (strlen(string_distribution[1].c_str())==0)
			{
				cout<<"wrong input : Enter the filename too. format 'send <filename>' "<<endl;
			}
			else {

				clientfiles=files();
				string filename=string_distribution[1];
				string ch;
				char pres1[]="r";				char pres[]="r";
				ch="nook";
				if(find(clientfiles.begin(), clientfiles.end(), filename) != clientfiles.end()) {
					cout<<"Server: the file "<<filename<<" already exists."<<endl;
					cout<<"Do you want to keep(k) or replace(r) it ?";
					getline(cin,str);
					strcpy(pres,str.c_str());
				}
				if (!strncasecmp(pres,pres1,strlen(pres1)))		
				{
					send(client,buffer, sizeof(buffer), 0);
					
					recv(client, buffer, buflen, 0);
					string str;
					for (int i = 0; i<4; ++i)
					{
						str+=buffer[i];
					}
					if (str!="nook")
					{
						FILE *received_file;
					int filesize=atoi(buffer);
					ofstream out(filename.c_str());

					received_file=fopen(filename.c_str(),"w");

					int length;
					int data=filesize;
					while((data>0)){
						length=recv(client,buffer,buflen,0);
						fwrite(buffer,sizeof(char),length,received_file);
						data-=length;

					}
					fclose(received_file);
						cout<<"Server: the file "<<filename<<" has been sent."<<endl;
						if(find(clientfiles.begin(), clientfiles.end(), filename) == clientfiles.end()){
						clientfiles.push_back(filename);
				}

					}
					else{
						cout<<"Server: the file "<<filename<<" does not exist on the server."<<endl;
					}

				}
				else{
					cout<<"The file is not changed.";
				}
			}
		}
		else{
			cout<<"-----ERROR INPUT--------"<<endl;
			cout<<"Choose from below: \n";
			cout<<" List server \n List client \n Create client <filename> \n Create server <filename> \n delete client <filename> \n delete server <filename> \n send <filename> \n receive <filename> \n"<<endl;
		}

	}

	cout<<"\nlet's do this again sometime!"<<endl;

	close(client);





}

