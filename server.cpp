

// Server side C/C++ program to demonstrate Socket programming
#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <arpa/inet.h>    

using namespace std;

int PORT;
void* threadMain(void *);

void read_repo(void);
void read_client_info(void);
string search(string);
void share(string,string);
void delete_share(string,string);
void client_connected(string);
void client_disconnected(string);


//for repo.txt
struct row_repo{
	string file_name;
	string path_name;
	string alias_name;
};
vector<row_repo> repo;
row_repo r;

//for client_info.txt using map<alias_name,row_client>
struct row_client{
	
	string ip;
	string port;
	string down_port;
	
};

row_client c;
map<string,row_client> clientfile;
string alias_name;

map<string,row_client>::iterator it;


//struct to combine result
struct row_result{
	string file_name;
	string path_name;
	string alias_name;
	string ip;
	string port;
	string down_port;
	
};


//read repo.txt into vector repo
void read_repo()
{	repo.clear();  //clear old data

	string data;
	int j=0;
	int fipos,secpos;

    fstream  repofile;
	repofile.open("repo.txt", ios::out | ios::in );
	
//copy the repo file data  into vector by breaking column into struct
    while (getline(repofile, data))
	{
  	
  	int len=data.length();
  	int flag=0;
  	for(int i=0;i<len;i++)
  	{	
  		if(data[i]==':')
  		{
  			if(flag==0)
  				{	fipos=i;
  					flag=1;
  					r.file_name=data.substr(0,fipos);
  	//			cout<<r.file_name<<endl;
  				}
  			else{	secpos=i;
  					r.path_name=data.substr(fipos+1,secpos-fipos-1);
  	//		cout<<r.path_name<<endl;
  				}
		}
  	}
  	//cout<<file_contents<<endl;
  		r.alias_name=data.substr(secpos+1,len-secpos);
 	//	cout<<r.alias_name<<endl;
  		repo.push_back(r);

  		

	} 

	repofile.close();
}




//read repo.txt into vector repo
void read_client_info()
{	clientfile.clear();//clear client buffer


	string data;
	int j=0;
	int fipos,secpos;

	fstream  client_info;
	client_info.open("client_info.txt", ios::out | ios::in );

	string data2;
	int thirdpos;
		
    while (getline(client_info, data2))
	{
  	
  	
  	int len=data2.length();
  	int flag=0;
  	for(int i=0;i<len;i++)
  	{	
  		if(data2[i]==':')
  		{
  			if(flag==0)
  				{	fipos=i;
  					flag=1;
  					alias_name=data2.substr(0,fipos);
  	//			cout<<c.alias_name<<endl;
  				}
  			else if(flag==1){	secpos=i;
  								flag=2;
  					c.ip=data2.substr(fipos+1,secpos-fipos-1);
  	//				cout<<c.ip<<endl;
  							}
  				else if(flag==2){ thirdpos=i;
  									flag=3;
  							c.port=data2.substr(secpos+1,thirdpos-secpos-1);
  	//				cout<<c.port<<endl;				
								}

		}
  	}
  	
  		c.down_port=data2.substr(thirdpos+1,len-thirdpos);
  	//	cout<<c.down_port<<endl;
  	//	clientfile.push_back(c);
  		clientfile.insert(pair <string, row_client> (alias_name,c));
  } 

client_info.close();



}



int main(int argc, char *argv[])
{   
    PORT=atoi(argv[2]);
	char* server_ip=argv[1];
    long server_fd, new_socket;
    int valread;
    struct sockaddr_in address;
    struct sockaddr_in client_address;
 //   int opt = 1;
    int addrlen = sizeof(address);
    int client_addrlen =sizeof(client_address);
    /*char buffer[1024];
    char hello[1024];*/
     string buffer;
     string hello;


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
   /* // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }*/
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server_ip);
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
     cout << "=> Looking for clients..." << endl;
    if (listen(server_fd, 3) < 0)
    {
        cout << "=> Error on accepting..." << endl;
        perror("listen");
        exit(EXIT_FAILURE);
    }


while(1)
{  
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
        perror("accept");
        exit(EXIT_FAILURE);
        }
        cout<<"client_connected"<<endl;

        pthread_t tid;
        pthread_create(&tid,NULL,threadMain,(void *) new_socket);
       
}
   


 
    return 0;
}





//thread function
void* threadMain(void * var)
{   
    long new_socket = ((long)var);

    //to connect client and make entry
    read_repo();
	read_client_info();

    char y[BUFSIZ];
    recv(new_socket , y, BUFSIZ,0);
    string comm1="";
    for(int i=0;i<strlen(y);i++)
    {
        comm1+=y[i];
    }
    int l=comm1.length();
    int loc=comm1.find_last_of(":");
    string comm=comm1.substr(0,loc-1);
    string car=comm1.substr(loc+1,l-1);
    client_connected(comm);

    while(1)
    {
    bzero(y,BUFSIZ);
    string command="";
     recv(new_socket , y, BUFSIZ,0);
     int i=0,pos=0;
     while(y[i]!=' ')
    {   
        command+=y[i];
        pos=i;
        i++;

    }



    string query="";
    for(i=pos+2;i<strlen(y);i++)
    {
        query+=y[i];
    }
    //cout<<command<<endl;
    //cout<<query<<endl;
    int leng=query.length();
    string query2=query.substr(1,leng-3);
        string a="search";
        string b="share";
        string c="del";
        string d="disc";
        if(command.compare(a)==0)
        {   cout<<"searching"<<endl;
            //cout<<query2<<endl;
            string result=search(query2);
            //cout<<result<<endl;
            int sent = send(new_socket , result.c_str() ,BUFSIZ, 0 ); 
            //cout << sent << "\n";
        }

        else if(command.compare(b)==0)
       		 {
        		cout<<"sharing"<<endl;
        	
        		share(query2,car);

        	 }

        	else if(command.compare(c)==0)
        	{
        		cout<<"deleting"<<endl;
        		delete_share(query2,car);

        	}

        	else if(command.compare(d)==0)
        	{
        		cout<<"client_disconnected"<<endl;
        		client_disconnected(car);

        	}



    }


}







//to search the file
string search(string search_file)
{	
	repo.clear();
	read_repo();
	//cout<<repo.size();
	//read_client_info();
	row_result res;
	vector<row_result> search_res_v;
	int count=0;
	for(int i=0;i<repo.size();i++)
	{
		//repo[i].file_name==""
		string a=repo[i].file_name;
		string b=search_file;
		transform(a.begin(), a.end(), a.begin(), ::tolower);
		transform(b.begin(), b.end(), b.begin(), ::tolower);

		if(a.compare(0, search_file.length(),b)==0)
		{	
			res.file_name=repo[i].file_name;
			res.path_name=repo[i].path_name;
			res.alias_name=repo[i].alias_name;

			//search alias name in to map
			row_client sample;
			it=clientfile.find(repo[i].alias_name);
			//cout<<(it->second).port;
			
			if(it!=clientfile.end())
			sample=(*it).second;
			/*else{
				//cout<<"not use";
				break;
			}*/
			res.ip=sample.ip;
			res.port=sample.port;
			res.down_port=sample.down_port;

			search_res_v.push_back(res);
			count++;
			//cout<<repo[i].file_name<<endl;
		}
		
	}


string result="";
//stra=to_string(count);
result="FOUND:"+to_string(count)+"\n";
//cout<<"FOUND:"<<count<<endl;
for(int i=0;i<search_res_v.size();i++)
	{

result+="["+to_string(i+1)+"] "+search_res_v[i].file_name +":"+  search_res_v[i].path_name + ":"+  search_res_v[i].alias_name +":"+search_res_v[i].ip +":"+search_res_v[i].port +":"+search_res_v[i].down_port+"\n";

//cout<<"["<<(i+1)<<"] "<<search_res_v[i].file_name <<":"<<  search_res_v[i].path_name << ":"<<  search_res_v[i].alias_name <<":"<<search_res_v[i].ip <<":"<<search_res_v[i].port <<":"<<search_res_v[i].down_port<<endl;
	}
//cout<<result;

 
return result;


}



void share(string pathname,string cname)
{	
	row_repo i;
	int len=pathname.length();
	int j=len;
	while(pathname[j-1]!='/')
	{j--;
	}
	i.file_name=pathname.substr(j,len-j+1);
	i.path_name=pathname.substr(0,len);
	i.alias_name=cname;
string pathnamemodified=pathname.substr(0,len);
string fname=pathname.substr(j,len-j+1);
string entry='\n'+fname+':'+pathnamemodified+':'+cname;


ofstream out;
 // std::ios::app is the open mode "append" meaning
 // new data will be written to the end of the file.
 out.open("repo.txt",ios::app);


 //std::string str = "I am here.";
 //out << entry;
 int size=entry.length();
   out.write (entry.c_str(),size);
 // out.println(entry);
 out.close();
}



void delete_share(string pathname,string cname)
{		
	repo.clear();
	read_repo();
	int pos=-1;
for(int i=0;i<repo.size();i++)
{
	string a=repo[i].path_name;
	string b=repo[i].alias_name;
	string c=pathname;
	string d=cname;
	/*transform(a.begin(), a.end(), a.begin(), ::tolower);
	transform(b.begin(), b.end(), b.begin(), ::tolower);
	transform(c.begin(), c.end(), c.begin(), ::tolower);
	transform(d.begin(), d.end(), d.begin(), ::tolower);
	*/
	if((a.compare(c)==0) && (b.compare(d)==0))
	{	
		pos=i;
		repo.erase(repo.begin()+pos);
		cout<<"matched!!!!"<<endl;
		break;
	}

}	
  if(pos!=-1)
  {
	ofstream out;
	out.open("repo.txt",ios::trunc);

	for(int i=0;i<repo.size();i++)
	{	string fname=repo[i].file_name;
		string pathnamemodified=repo[i].path_name;
		string cname=repo[i].alias_name;
		string entry;
		if(i==0)
		entry=fname+':'+pathnamemodified+':'+cname;
		else
		entry='\n'+fname+':'+pathnamemodified+':'+cname;

		 int size=entry.length();
 	 	 out.write (entry.c_str(),size);
 	 	 pos=-1;
 		
	}

	out.close();
   }

	else{
		cout<<"not shared";
	}


}



//CLIENT CONNECTED
void client_connected(string p)
{
 string entry='\n'+p;
 ofstream out;
 out.open("client_info.txt",ios::app);

 int size=entry.length();
 out.write (entry.c_str(),size);
 // out.println(entry);
 out.close();

}


void client_disconnected(string client)
{	
	clientfile.clear();
	read_client_info();

	it=clientfile.find(client);
	if(it !=clientfile.end())
	{	
	clientfile.erase(it); 

	string ent;

	ofstream out;
 	out.open("client_info.txt",ios::trunc);


	for(it = clientfile.begin(); it !=clientfile.end(); ++it)
		{
		string client=it->first;
		string ip=it->second.ip;
		string p=it->second.port;
		string dp=it->second.down_port;


		 if(it==clientfile.begin())
			ent=client+':'+ip+':'+p+':'+dp;
		 else
		 	ent='\n'+client+':'+ip+':'+p+':'+dp;
		
	 	int size=ent.length();
     	out.write (ent.c_str(),size);
 		}
 	// out.println(entry);
 	out.close(); 

	}
	else{
		cout<<"already disconnected";
	}	
	
}












