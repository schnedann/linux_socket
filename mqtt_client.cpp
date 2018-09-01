#include "mqtt_client.h"

mqtt_client::mqtt_client()
{

}

/**
 * @brief mqtt_client::connect
 * @param broker - IP Adress or domain-string
 * @param port   - IP Port
 * @return true on error
 */
bool mqtt_client::connect_to(std::string broker, uint16_t port)
{
  return tcp_client::connect_to(broker,port);
}

bool mqtt_client::publish()
{

  return false;
}

bool mqtt_client::get_state()
{
  return false;
}
