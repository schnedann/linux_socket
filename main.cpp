/**
 * C++ client example using sockets
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cerrno>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

/**
 * TCP Client class
 */
class tcp_client{
private:
  std::string address;
  int sock;
  int port;
  struct sockaddr_in server;

  constexpr static int const socket_void = -1;

public:
  tcp_client() noexcept;
  bool conn(string, uint16_t);
  bool send_data(string data);
  string receive(const int);
};

tcp_client::tcp_client() noexcept{
  sock = socket_void;
  port = 0;
  address = "";
  return;
}

/**
 * Connect to a host on a certain port number
 */
bool tcp_client::conn(string address , uint16_t port){

  //create socket if it is not already created
  if(sock == socket_void){
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1){
      std::cout << "socket() failed: " << std::strerror(errno) << "\n";
      perror("Could not create socket");
    }
    cout << "Socket created " << "\n";
  }
  else{ /* OK , nothing */ }

  //setup address structure
  struct in_addr ipadr;
  if(0==inet_aton(address.c_str(),&ipadr)){ //domain string which needs to be resolved
    struct hostent *he;
    struct in_addr **addr_list;

    //resolve the hostname, its not an ip address
    if ( (he = gethostbyname( address.c_str() ) ) == nullptr){
      //gethostbyname failed
      herror("gethostbyname");
      cout << "Failed to resolve hostname" << "\n";
      return false;
    }

    {
      cout << "Hostname: " << he->h_name << "\n";
      if(AF_INET == he->h_addrtype)  cout << "IPv4" << " --> sizeof(" << he->h_length << ")" << "\n";
      if(AF_INET6 == he->h_addrtype) cout << "IPv6" << " --> sizeof(" << he->h_length << ")" << "\n";
      size_t ii = 0;
      for(char* adr = he->h_addr_list[ii]; adr!=nullptr; adr=he->h_addr_list[ii++]){
        /*cout << "IP: ";
        for(size_t ij=0; ij<static_cast<size_t>(he->h_length); ++ij){
          cout << uint16_t(uint8_t(adr[ij]));
          if(ii<(static_cast<size_t>(he->h_length)-1))cout << ".";
        }
        cout << "\n";*/
        cout << "IP: " << inet_ntoa(*reinterpret_cast<struct in_addr*>(adr)) << "\n";
      }
    }

    if(AF_INET == he->h_addrtype){
      //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
      addr_list = reinterpret_cast<struct in_addr **>(he->h_addr_list);
      server.sin_addr = *addr_list[0];
    }else{
      cout << "IPv6 not implemented yet" << "\n";
    }
  }else{ //plain ip address
    server.sin_addr.s_addr = ipadr.s_addr;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons( port );

  //Connect to remote server
  if (connect(sock , reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) < 0){
    perror("connect failed. Error");
    return 1;
  }

  cout << "Connected" << "\n";
  return true;
}

/**
 * Send data to the connected host
 */
bool tcp_client::send_data(string data){
  bool res = true;
  //Send some data
  if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0){
    perror("Send failed : ");
    res = false;
  }else{
    cout<<"Data send\n";
  }
  return res;
}

/**
 * Receive data from the connected host
 */
string tcp_client::receive(int const size=512){
  char* buffer = new char[size+1];
  string reply;

  //Receive a reply from the server
  if( recv(sock , buffer , sizeof(buffer) , 0) < 0){
    cout << "recv failed" << "\n";
  }

  reply = buffer;
  delete [] buffer;
  return reply;
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))){
  tcp_client c;
  string host;

  cout<<"Enter hostname : ";
  cin>>host;

  //connect to host
  c.conn(host , 80);

  //send some data
  c.send_data("GET / HTTP/1.1\r\n\r\n");

  //receive and echo reply
  cout << "----------------------------\n\n";
  cout << c.receive(1024) << "\n\n";
  cout << "----------------------------\n\n";

  //done
  return 0;
}
