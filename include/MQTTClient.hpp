/**
 * @file
 * A MQTT client, which subscribes to alarm lamp state change messages
 */

#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <stdint.h>
#include <mosquittopp.h>

using namespace std;

class BLEClient;


class MQTTClient: public mosqpp::mosquittopp
{
  private:
    const char* host;
    const char* id;
    const char* topic;
    int port;
    int keepalive;
    bool connected = false;

    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);
    void on_message(const struct mosquitto_message* message);

    BLEClient* ble_client = NULL;

  public:
    MQTTClient(const char* id, const char* topic, const char* host, int port);
    ~MQTTClient();

    bool send_message(char* s, uint8_t length);

    void registerOnMessageEventReceiver(BLEClient* ble) { ble_client = ble; };
};


#endif
