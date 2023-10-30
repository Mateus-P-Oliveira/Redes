#include <iostream>
#include <arpa/inet.h>
#include <string>
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 


#define PORT     8080  //Isso vai ter que ser definido depois pelo txt
#define MAXLINE 1024 

using namespace std;

struct sockaddr_in sa; //Address

//https://www.geeksforgeeks.org/udp-server-client-implementation-c/ - Como Implementar UDP
struct MachineClient {
  string name;
  int token_count,right_ip_data;
  string right_ip;
  bool Generated_token;
};

MachineClient machine_creation() { // Mudar depois caso necessario
  MachineClient Machine;
  cin >> Machine.right_ip;
  cin >> Machine.name;
  cin >> Machine.token_count;
  cin >> Machine.Generated_token;
  inet_pton(AF_INET, Machine.right_ip.c_str(), &(sa.sin_addr)); //Converte de string para ip
  return Machine;
}

int main(void) {
  MachineClient my_pc;
  my_pc = machine_creation();
  cout << my_pc.right_ip << endl;
  cout << my_pc.name << endl;
  cout << my_pc.token_count << endl;
  cout << my_pc.Generated_token << endl;

  //-----------------------------------
    int sockfd; 
    char buffer[MAXLINE]; 
    const char *hello = "Hello from client"; 
   
   
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
   
    memset(&sa, 0, sizeof(sa)); 
       
    // Filling server information 
    sa.sin_family = AF_INET; 
    sa.sin_port = htons(PORT); 
    sa.sin_addr.s_addr = INADDR_ANY; 
       
    int n;
    socklen_t len; 
       
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &sa,  
            sizeof(sa)); 
    std::cout<<"Hello message sent."<<std::endl; 
           
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &sa, 
                &len); 
    buffer[n] = '\0'; 
    std::cout<<"Server :"<<buffer<<std::endl; 
   
    close(sockfd); 
    





  //---------------------------------
  return 0;
}