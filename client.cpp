// Client side C/C++ program to demonstrate Socket programming
#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

//#define PORT 8090
void client_connected(string,string,string,string);
void client_disconnected(string);
void read_client_info(void);


  //client_alias client_ip client_port server_ip server_port downloading_port client_root
const char* ip_add;
int PORT;

int main(int argc, char *argv[])
{   
    ip_add=argv[4];
    PORT=atoi(argv[5]);
    const char* c_name=argv[1];

    struct sockaddr_in client_address;
    int client_fd = 0, valread;
    struct sockaddr_in serv_addr;
    char send_data[BUFSIZ];
    char buffer[BUFSIZ];
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip_add, &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==-1)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    

    //for creating a entry in client file info
    string str1(argv[1]);
    string str2(argv[2]);
    string str3(argv[3]);
    string str6(argv[6]);
   // string temp=argv[1]+':'+argv[2]+':'+str1(argv[3])+':'+str1(argv[6]);
    string temp=str1+':'+str2+':'+str3+':'+str6+':'+c_name;
    //send_data=temp.c_str();
    send(client_fd , temp.c_str() ,BUFSIZ, 0 );
   // cout<<"client_connected"<<endl;

    while(1)
    { 
           bzero(send_data,BUFSIZ);

           read(STDIN_FILENO,send_data,BUFSIZ);
           send(client_fd , send_data ,BUFSIZ, 0 );

           bzero(send_data,BUFSIZ);
           int sent = recv(client_fd , send_data, BUFSIZ,0);
           //cout << sent << "\n";
           cout<<send_data<<endl;








    //   printf("- by client\n");
    //   read( client_fd , buffer, 1024);
    //   printf("%s\n",buffer );

    }
    return 0;
}







