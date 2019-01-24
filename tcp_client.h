#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dtypes.h"

namespace Network {

  using inet_sock_t = struct sockaddr_in;
  using inet6_sock_t = struct sockaddr_in6;
  using unix_sock_t = struct sockaddr_un;

  enum class domain:sa_family_t{
    local = AF_LOCAL ,  //Local communication
  //unix  = AF_UNIX  ,  //Local communication
    IPv4  = AF_INET  ,  //IPv4 Internet protocols
    IPv6  = AF_INET6 ,  //IPv6 Internet protocols
    raw   = AF_PACKET   //Low level packet interface
  };

  enum class soctype:int{
    tcp = SOCK_STREAM,
    udp = SOCK_DGRAM,
    raw = SOCK_RAW
  };

  constexpr static int const reterr = -1;

  class connection{
  public:
    connection();
    ~connection();

    std::string address;
    int sock;
    in_port_t port;
    inet_sock_t server;
  };

  /**
   * TCP Client class
   */
  class tcp_client{
  private:

  public:
    static void prepare_con(connection& con, std::string a, in_port_t p);
    static bool connect_to(connection& con);
    static bool listen_to(connection& con);
    static bool is_connected(connection& con);
    static bool send_data(connection& con, std::string data);
    static std::string receive(connection& con, const u32 size);
  };
}



#endif // TCP_CLIENT_H
