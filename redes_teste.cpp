#include <iostream>
#include <arpa/inet.h>
#include <string>

using namespace std;

struct sockaddr_in sa;


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



  return 0;
}