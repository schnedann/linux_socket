#include "tcp_client.h"

#include <iostream>
#include <cstdint>
#include <cerrno>
#include <cstring>
#include <algorithm>

#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "debug_print.h"

using namespace std;

/**
 * @brief emerg_exit - This function is called when a system call fails.
 * It displays a message about the error on stderr and then aborts the program.
 * @param trigger
 */
void emerg_exit(bool trigger, std::string msg){
  if(trigger){
    perror(msg.c_str());
    exit(1);
  }
  return;
}

/**
 * @brief Network::connection::connection
 */
Network::connection::connection():address(),sock(0),port(0)
{
  //Zero mngt struct
  u8* bptr = reinterpret_cast<u8*>(&server);
  u8* eptr = &bptr[sizeof(inet_sock_t)];
  std::fill(bptr,eptr,0);
}

/**
 * @brief Network::connection::~connection
 */
Network::connection::~connection()
{
  if(reterr!=sock){
    close(sock);
    sock = reterr;
  }
  return;
}

//-----

/**
 * @brief Network::tcp_client::connect_to - Connect to a host on a certain port number
 * @param address - IP or domain string
 * @param port    - IP Port
 * @return true on error
 */
void Network::tcp_client::prepare_con(Network::connection &con, string a, in_port_t p)
{
  //Zero mngt struct
  u8* bptr = reinterpret_cast<u8*>(&con.server);
  u8* eptr = &bptr[sizeof(inet_sock_t)];
  std::fill(bptr,eptr,0);

  con.sock = reterr;
  con.address = a;
  con.port = p;
  return;
}

bool Network::tcp_client::connect_to(connection& con){
  bool err = false;

  //create socket if it is not already created
  if(con.sock == reterr){
    //Create nonblocking socket
    con.sock = socket(AF_INET , SOCK_NONBLOCK | SOCK_STREAM , 0);
    err = (con.sock == reterr);
  }
  emerg_exit(err,"Network::tcp_client::connect_to - Could not create socket");
  if(!err){
    DBGOUT("Socket created")

    //setup address structure
    struct in_addr ipadr;
    if(0==inet_aton(con.address.c_str(),&ipadr)){ //domain string which needs to be resolved
      struct hostent *he;
      struct in_addr **addr_list;

      //resolve the hostname, its not an ip address
      if ( (he = gethostbyname( con.address.c_str() ) ) == nullptr){
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
          con.server.sin_addr = *addr_list[0];
        }else{
          err = true;
          cout << "IPv6 not implemented yet" << "\n";
        }
      }
    }else{ //plain ip address
      con.server.sin_addr.s_addr = ipadr.s_addr;
    }
  }

  if(!err){
    con.server.sin_family = AF_INET;
    con.server.sin_port = htons( con.port );

    //Connect to remote server
    if (connect(con.sock , reinterpret_cast<struct sockaddr *>(&con.server), sizeof(inet_sock_t)) < 0){
      close(con.sock);
      con.sock = reterr;
      err = true;
    }
    emerg_exit(err,"Network::tcp_client::connect_to - connect failed. Error");
    cout << "Connected" << "\n";
  }

  return err;
}

bool Network::tcp_client::listen_to(connection& con)
{
  bool err = true;
  listen(con.sock, 128);
  return err;
}

bool Network::tcp_client::is_connected(connection& con){
  return (con.sock!=reterr);
}

/**
 * @brief Network::tcp_client::send_data - Send data to the connected host
 * @param data - data to send
 * @return true on error
 */
bool Network::tcp_client::send_data(connection& con, string data){
  bool err = true;
  //Send some data
  if(is_connected(con)){
    err = false;
    if(send(con.sock , data.c_str() , data.size() , 0) < 0){
      err = true;
    }else{
      cout << "Data send" << "\n";
    }
  }
  emerg_exit(err,"Network::tcp_client::connect_to - Send failed: ");
  return err;
}

/**
 * @brief Network::tcp_client::receive - Receive data from the connected host
 * @param size - buffer size for message
 * @return
 */
string Network::tcp_client::receive(connection& con, u32 const size=512){
  string reply;
  if(is_connected(con)){
    char* buffer = new char[size+1];

    //Receive a reply from the server
    uint16_t timeout = 2400; // 2400*25ms = 60s
    ssize_t res = reterr;
    while (timeout>0) {
      res = recv(con.sock , buffer , sizeof(buffer) , MSG_DONTWAIT);
      if(res > 0) break;
      if(res == reterr){
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
    if(res==reterr){
      cout << "recv failed" << "\n";
    }else{
      cout << "Bytes received: " << res << "\n";
      reply = buffer;
    }

    delete [] buffer;
  }
  return reply;
}
