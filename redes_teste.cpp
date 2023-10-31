#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080 // Isso vai ter que ser definido depois pelo txt
#define MAXLINE 1024

int porta = 8080;

using namespace std;

struct sockaddr_in sa; // Address

// https://www.geeksforgeeks.org/udp-server-client-implementation-c/ - Como
// Implementar UDP
struct MachineClient {
  string name;
  int token_count;
  string right_ip;
  bool Generated_token;
};

MachineClient machine_creation() { // Mudar depois caso necessario
  MachineClient Machine;
  string token;
  int counter = 1;
  
  string segment;
  int iC = 0;
  ifstream My_machine_file("machine_configuration.txt");
  while (getline(My_machine_file, token, '\n')) {
    stringstream token_separator(token);
    switch (counter) {
    case 1:

      // Machine.right_ip = token;

      while (getline(token_separator, segment, ':')) { //Separo Porta e IP
          
        if (iC == 0) {
       
          Machine.right_ip = segment;
        } else if (iC == 1) {
          porta = stoi(segment);
        }
        iC++;
      }

      break;
    case 2:

      Machine.name = token;
      break;
    case 3:

      Machine.token_count = stoi(token);
      break;
    case 4:

      if (token == "true") {
        Machine.Generated_token = true;
      } else {
        Machine.Generated_token = false;
      }

      break;
    default:
      break;
    }
    counter++;
  }

  inet_pton(AF_INET, Machine.right_ip.c_str(),
            &(sa.sin_addr)); // Converte de string para ip
  // cout << &sa.sin_addr <<endl; //Ver o que sai nessa saida
  My_machine_file.close();

  return Machine;
}

void client(MachineClient MachineName) {
  int sockfd;
  char buffer[MAXLINE];
  const char *hello =
      "Hello from client"; // Isso ira receber a mensagem da file depois

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { // SOCK_DGRAM = UDP
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&sa, 0, sizeof(sa));

  // Filling server information
  sa.sin_family = AF_INET;  //Designa quais endereços meu socket pode se comunicar 
  sa.sin_port = htons(porta); // Aqui salvo a porta do PC vizinho
  sa.sin_addr.s_addr = INADDR_ANY; // Ligo em todos os sockets para receber valores de qualquer porta

  int n;
  socklen_t len;

  sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, //Envia dados para o Socket
         (const struct sockaddr *)&sa, sizeof(sa));
  std::cout << "Hello message sent." << std::endl;

  n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,  //Recebe data do Socket
               (struct sockaddr *)&sa, &len);
  buffer[n] = '\0';
  std::cout << "Server :" << buffer << std::endl;

  close(sockfd);
}



int server(){
  int sockfd; 
    char buffer[MAXLINE]; 
    const char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
       
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
       
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); //Recebo dados da file da porta do PC ao lado
       
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    socklen_t len;
  int n; 
   
    len = sizeof(cliaddr);  //len is value/result 
   
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 
    sendto(sockfd, (const char *)hello, strlen(hello),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 
    std::cout<<"Hello message sent."<<std::endl;  
       
    return 0; 
}

int main(void) {
  MachineClient my_pc;
  my_pc = machine_creation();
  cout << my_pc.right_ip << endl;
  cout << my_pc.name << endl;
  cout << my_pc.token_count << endl;
  cout << my_pc.Generated_token << endl;
  //-----------------------------------Servidor
  server(); //Espero a entradade dados
  //----------------------------------- Cliente
  client(my_pc); //Envio dados para a maquina vizinha
  //---------------------------------
  return 0;
}