/**
 * C++ client example using sockets
 * Simple MQTT client
 */

#include <iostream>
#include <cstdint>

#include "mqtt_client.h"

using namespace std;

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))){
  mqtt_client mqttc;

  mqttc.connect_to("",0);

  //done
  return 0;
}
