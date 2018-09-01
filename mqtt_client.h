#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "tcp_client.h"

class mqtt_client : private tcp_client
{
public:
  mqtt_client();
};

#endif // MQTT_CLIENT_H
