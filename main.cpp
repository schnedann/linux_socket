/**
 * C++ client example using sockets
 */

#include <iostream>
#include <cstdint>

#include "tcp_client.h"

using namespace std;

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))){
  tcp_client c;
  string host;

  cout<<"Enter hostname : ";
  cin>>host;

  //connect to host
  c.conn(host , 80);

  //send some data
  c.send_data("GET /index.html HTTP/1.1\r\n\r\n");

  //receive and echo reply
  cout << "----------------------------\n\n";
  cout << c.receive(1024) << "\n\n";
  cout << "----------------------------\n\n";

  //done
  return 0;
}
