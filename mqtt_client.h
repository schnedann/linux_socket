#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <cstdint>
#include <string>
#include <vector>
#include "tcp_client.h"

enum class mqtt_Control_Packet_type: uint8_t{
  Reserved    = 0,
  CONNECT     = 1, // Client to Server Client request to connect to Server
  CONNACK     = 2, // Server to Client Connect acknowledgment
  PUBLISH     = 3,
  PUBACK      = 4,
  PUBREC      = 5,
  PUBREL      = 6,
  PUBCOMP     = 7,
  SUBSCRIBE   = 8,  // Client to Server Client subscribe request
  SUBACK      = 9,  // Server to Client Subscribe acknowledgment
  UNSUBSCRIBE = 10, // Client to Server Unsubscribe request
  UNSUBACK    = 11, // Server to Client Unsubscribe acknowledgment
  PINGREQ     = 12, // Client to Server PING request
  PINGRESP    = 13, // Server to Client PING response
  DISCONNECT  = 14, // Client to Server Client is disconnecting
  Forbidden   = 15  //Forbidden Reserved

};

class mqtt_client : private tcp_client
{
private:
  std::vector<uint8_t> get_mqtt_header(mqtt_Control_Packet_type cpt, uint8_t pubflags, uint32_t psize);
  std::vector<uint8_t> encode_length(uint32_t psize);
public:
  mqtt_client();
  bool connect_to(std::string broker, uint16_t port);
  bool publish(std::string data);
  bool get_state();
};

#endif // MQTT_CLIENT_H
