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

#define MAXLINE 1024
#define PORT 8080

using namespace std;

struct MachineClient {
  string name;
  int token_count, porta;
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

      while (getline(token_separator, segment, ':')) { // Separo Porta e IP

        if (iC == 0) {

          Machine.right_ip = segment;
        } else if (iC == 1) {
          Machine.porta = stoi(segment);
          cout << Machine.porta << endl;
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

  My_machine_file.close();

  return Machine;
}
// Server

void server(MachineClient My_machine) {
  int sockfd;
  char buffer[MAXLINE];
  const char *hello = "Hello from server";

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { // Cria o socket
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in servaddr, cliaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.2"); //Defino na mão o address que qeuro o servidor //INADDR_ANY   --Caso queira me ligar a todas entradas
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  socklen_t len;
  int n;

  len = sizeof(cliaddr); // len is value/result
  // recebe mensagem do client
  n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
               (struct sockaddr *)&cliaddr, &len);
  buffer[n] = '\0';
  sleep(My_machine.token_count);
  printf("Client : %s\n", buffer);
  sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
         (const struct sockaddr *)&cliaddr, len);
  std::cout << "Hello message sent." << std::endl;
}

// Client

void client(MachineClient right_machine) {
  int sockfd;
  char buffer[MAXLINE];
  const char *hello = "Hello from client";

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(right_machine.porta);
  inet_pton(AF_INET, right_machine.right_ip.c_str(),
            &servaddr.sin_addr); // Convert from string to byte array
  // servaddr.sin_addr.s_addr = INADDR_ANY;

  int n;
  socklen_t len;

  sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
         (const struct sockaddr *)&servaddr, sizeof(servaddr));
  std::cout << "Hello message sent." << std::endl;
  // Recebe mensagem do Server
  n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
               (struct sockaddr *)&servaddr, &len);
  buffer[n] = '\0';
  std::cout << "Server :" << buffer << std::endl;

  close(sockfd);
}

int main(void) {
  MachineClient my_pc;
  my_pc = machine_creation();
  cout << my_pc.right_ip << endl;
  cout << my_pc.name << endl;
  cout << my_pc.token_count << endl;
  cout << my_pc.Generated_token << endl;
   while(1){
     client(my_pc); // Envio dados para a maquina vizinha
    //---------------------------------
        
  //-----------------------------------Servidor
    server(my_pc); // Espero a entradade dados
    //----------------------------------- Cliente
    
   
   }
   
   
 
  return 0;
}