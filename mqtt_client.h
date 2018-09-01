#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include "tcp_client.h"

class mqtt_client : private tcp_client
{
public:
  mqtt_client();
  bool connect_to(std::string broker, uint16_t port);
  bool publish();
  bool get_state();
};

#endif // MQTT_CLIENT_H
