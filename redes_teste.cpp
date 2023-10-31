#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080 // Isso vai ter que ser definido depois pelo txt
#define MAXLINE 1024

using namespace std;

struct sockaddr_in sa; // Address

// https://www.geeksforgeeks.org/udp-server-client-implementation-c/ - Como
// Implementar UDP
struct MachineClient {
  string name;
  int token_count, right_ip_data;
  string right_ip;
  bool Generated_token;
};

MachineClient machine_creation() { // Mudar depois caso necessario
  MachineClient Machine;
  string token;
  int counter = 1;
  ifstream My_machine_file("machine_configuration.txt");
  while (getline(My_machine_file, token, '\n')) {
    switch (counter) {
    case 1:

      Machine.right_ip = token;
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

  My_machine_file.close();
  return Machine;
}

void client() {
  int sockfd;
  char buffer[MAXLINE];
  const char *hello = "Hello from client";

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&sa, 0, sizeof(sa));

  // Filling server information
  sa.sin_family = AF_INET;
  sa.sin_port = htons(PORT); // Aqui salvo a porta do arquivo de configuração
  sa.sin_addr.s_addr = INADDR_ANY; // Aqui salvo o IP do PC a direita

  int n;
  socklen_t len;

  sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
         (const struct sockaddr *)&sa, sizeof(sa));
  std::cout << "Hello message sent." << std::endl;

  n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
               (struct sockaddr *)&sa, &len);
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
  //-----------------------------------Servidor

  //----------------------------------- Cliente
  client();
  //---------------------------------
  return 0;
}