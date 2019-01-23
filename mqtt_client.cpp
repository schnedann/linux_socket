#include "mqtt_client.h"

using namespace std;

std::vector<uint8_t> mqtt_client::get_mqtt_header(mqtt_Control_Packet_type cpt,uint8_t pubflags,uint32_t psize){
  std::vector<uint8_t> res;
  uint8_t fhb = 0; //first header byte
  fhb |= uint8_t(cpt)<<4;
  if(cpt==mqtt_Control_Packet_type::UNSUBSCRIBE) fhb |= 2;
  if(cpt==mqtt_Control_Packet_type::PUBREL)      fhb |= 2;
  if(cpt==mqtt_Control_Packet_type::SUBSCRIBE)   fhb |= 2;
  if(cpt==mqtt_Control_Packet_type::PUBLISH)     fhb |= pubflags;
  std::vector<uint8_t> tmp = encode_length(psize);
  res.insert(res.end(),tmp.begin(),tmp.end());
  return res;
}

std::vector<uint8_t> mqtt_client::encode_length(uint32_t psize){
  u_int8_t size = 1 +                           \
                  ((psize>16383    )?(1):(0)) + \
                  ((psize>2097151  )?(1):(0)) + \
                  ((psize>268435455)?(1):(0));
  std::vector<uint8_t> res(size);
  uint8_t ii=0;
  do{
    res[ii] = psize % 128;
    psize /= 128;
    // if there are more data to encode, set the top bit of this byte
    if ( psize > 0 ){
      res[ii] |= 128;
    }
    ++ii;
  }while( psize > 0 );
  return res;
}

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
  Network::tcp_client::prepare_con(ncon,broker,port);
  bool err = Network::tcp_client::connect_to(ncon);
  if(!err){
    string data;
    Network::tcp_client::send_data(ncon,data);
    string answer = Network::tcp_client::receive(ncon,1024);
  }
  return err;
}

bool mqtt_client::publish(string data)
{
  Network::tcp_client::send_data(ncon,data);
  string answer = Network::tcp_client::receive(ncon,1024);
  return false;
}

bool mqtt_client::get_state()
{
  return false;
}
