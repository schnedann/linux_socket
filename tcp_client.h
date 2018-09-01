#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include <arpa/inet.h>

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
  ~tcp_client() noexcept;
  bool connect_to(std::string address, uint16_t port);
  bool is_connected() const;
  bool send_data(std::string data) const;
  std::string receive(const int);
};

#endif // TCP_CLIENT_H
