/**
 * @file
 * A MQTT client, which subscribes to alarm lamp state change messages
 */

#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <stdint.h>
#include <string>
#include <mosquittopp.h>

using namespace std;

namespace Robby
{
    namespace Command
    {
        const char Stop = 'S';
        const char Forward = 'F';
        const char Backward = 'B';
        const char Left = 'L';
        const char Right = 'R';
        const char RotateLeft = '<';
        const char RotateRight = '>';
    }

    namespace Subtopic
    {
        /*
         * This list must be equivalent to the
         * one in joystick-mqtt/include/Steering.hpp
         */
        const string Stop = "stop";
        const string Forward = "forward";
        const string Backward = "backward";
        const string Left = "left";
        const string Right = "right";
        const string RotateLeft = "rotateleft";
        const string RotateRight = "rotateright";
    }
}

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
