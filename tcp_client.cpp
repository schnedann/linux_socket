#include "tcp_client.h"

#include <iostream>
#include <cstdint>
#include <cerrno>
#include <cstring>

#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

tcp_client::tcp_client() noexcept{
  sock = socket_void;
  port = 0;
  address = "";
  return;
}

tcp_client::~tcp_client() noexcept{
  if(socket_void!=sock){
    close(sock);
    sock = socket_void;
  }
  return;
}

/**
 * @brief tcp_client::connect_to - Connect to a host on a certain port number
 * @param address - IP or domain string
 * @param port    - IP Port
 * @return true on error
 */
bool tcp_client::connect_to(string address , uint16_t port){
  bool err = false;

  //create socket if it is not already created
  if(sock == socket_void){
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == socket_void){
      err = true;
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
      err = true;
      //gethostbyname failed
      herror("gethostbyname");
      cout << "Failed to resolve hostname" << "\n";
      return false;
    }

    { //Print Infos for resolved hostname
      cout << "Hostname: " << he->h_name << "\n";
      if(AF_INET == he->h_addrtype)  cout << "IPv4" << " --> sizeof(" << he->h_length << ")" << "\n";
      if(AF_INET6 == he->h_addrtype) cout << "IPv6" << " --> sizeof(" << he->h_length << ")" << "\n";
      size_t ii = 0;
      for(char* adr = he->h_addr_list[ii]; adr!=nullptr; adr=he->h_addr_list[ii++]){
        cout << "IP: " << inet_ntoa(*reinterpret_cast<struct in_addr*>(adr)) << "\n";
      }
    }

    { //set Internet Adress
      if(AF_INET == he->h_addrtype){
        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = reinterpret_cast<struct in_addr **>(he->h_addr_list);
        server.sin_addr = *addr_list[0];
      }else{
        err = true;
        cout << "IPv6 not implemented yet" << "\n";
      }
    }
  }else{ //plain ip address
    server.sin_addr.s_addr = ipadr.s_addr;
  }

  if(!err){
    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) < 0){
      close(sock);
      sock = socket_void;
      err = true;
      perror("connect failed. Error");
      return 1;
    }

    cout << "Connected" << "\n";
  }

  return err;
}

bool tcp_client::is_connected() const{
  return (sock!=socket_void);
}

/**
 * @brief tcp_client::send_data - Send data to the connected host
 * @param data - data to send
 * @return true on error
 */
bool tcp_client::send_data(string data) const{
  bool err = true;
  //Send some data
  if(is_connected()){
    err = false;
    if(send(sock , data.c_str() , data.size() , 0) < 0){
      err = true;
      perror("Send failed : ");
    }else{
      cout<<"Data send\n";
    }
  }
  return err;
}

/**
 * @brief tcp_client::receive - Receive data from the connected host
 * @param size - buffer size for message
 * @return
 */
string tcp_client::receive(int const size=512){
  string reply;
  if(is_connected()){
    char* buffer = new char[size+1];

    //Receive a reply from the server
    uint16_t timeout = 2400; // 2400*25ms = 60s
    ssize_t res = -1;
    while (timeout>0) {
      res = recv(sock , buffer , sizeof(buffer) , MSG_DONTWAIT);
      if(res > 0) break;
      if(res == -1){
        if(errno==EAGAIN) goto iter;
        if(errno==EWOULDBLOCK) goto iter;
        break;
      }
  iter:
      struct timespec const ts = {0,25000000};
      nanosleep(&ts, nullptr);
      --timeout;
    }

    //if( recv(sock , buffer , sizeof(buffer) , MSG_DONTWAIT) < 0){
    if(res==-1){
      cout << "recv failed" << "\n";
    }else{
      cout << "Bytes received: " << res << "\n";
      reply = buffer;
    }

    delete [] buffer;
  }
  return reply;
}
