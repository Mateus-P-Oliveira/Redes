#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <queue>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#define MAXLINE 2048
#define PORT 8080

using namespace std;
//<token>;<apelido de origem>:<apelido do destino>:<controle de erro>:<CRC>:<mensagem>
queue<string> Messages; // Cria Lista de mensagens

struct MachineClient {
  string name;
  int token_count, porta;
  string right_ip;
  bool Generated_token;
};

string messageList(string AddMessage) { // Lista de mensagens
  string messagereturn;
  if (AddMessage.empty()) {
    messagereturn = Messages.front();
    Messages.pop();
    return messagereturn;
  } else if (Messages.size() <= 11) {
    Messages.push(AddMessage);
    return "Mensagem Adicionada";
  } else {
    return "Fila Cheia";
  }
}

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

  // Força a reusar o mesmo socket
  const int enable = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  struct sockaddr_in servaddr, cliaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = inet_addr(
      "127.0.0.2"); // Defino na mão o address que qeuro o servidor //INADDR_ANY
                    // --Caso<apelido de origem>:<apelido do destino>:<controle de erro>:<CRC>:<mensagem> queira me ligar a todas entradas
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

  string aviso, retorno;
  aviso = messageList("Ola estou na lista");

  // Verifica se a maquina recebeu o token
  string mensagemCompleta = buffer;
  string token_received;
  //Split String Value
  token_received = strtok(mensagemCompleta,';')
  int tokenValue = stoi(buffer);
  
  if (tokenValue == 1000) { // Mudar para 2000
    // Checa se existe mensagem para enviar
    if (Messages.size() != 0) {
      retorno = messageList("");
      sleep(My_machine.token_count);
      printf("Client : %s\n", buffer);
      sendto(sockfd, (const char *)retorno.c_str(), strlen(retorno.c_str()),
             MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
             len); // Retorna valor
      std::cout << "Hello message sent." << std::endl;
    } else { // Manda token para proxima maquina
      sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM,
             (const struct sockaddr *)&cliaddr,
             len); // Retorna valor
    }

  
  }
  else if (tokenValue == 2000){
  
  }

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

  // Teste se ela é maquina responsavel por emitir o token
  if (right_machine.Generated_token == true) {
    const char *token = "1000";
    sendto(sockfd, (const char *)token, strlen(token), MSG_CONFIRM,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
  }

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

int menu(MachineClient my_pc) {
  int escolha;
  cout << "1-Server \n2-Client" << endl;
  cin >> escolha;
  if (escolha == 1) {
    cout << "Server" << endl;
    server(my_pc);
  } else if (escolha == 2) {
    cout << "Client" << endl;
    client(my_pc);
  } else {
    return 1;
  }
  return 0;
}

int main(void) {
  int loopControl = 0;
  MachineClient my_pc;
  my_pc = machine_creation();
  cout << "Configuração" << endl;
  cout << my_pc.right_ip << " | " << my_pc.name << " | " << my_pc.token_count
       << " | " << my_pc.Generated_token << endl;

  while (1) {
    loopControl = menu(my_pc);
    if (loopControl == 1)
      break;
  }

  return 0;
}
