#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

struct MachineClient {
  string name;
  int token_count;
  string right_ip;
  bool Generated_token;
};

MachineClient machine_creation() { // Mudar depois caso necessario
  MachineClient Machine;
  cin >> Machine.right_ip;
  cin >> Machine.name;
  cin >> Machine.token_count;
  cin >> Machine.Generated_token;

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