/**
 * @file
 * A MQTT client, which subscribes to alarm lamp state change messages
 */

#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <stdint.h>
#include <string>
#include <mosquittopp.h>
#include "events/Event.hpp"
#include "events/EventGenerator.hpp"
#include "events/EventReceiver.hpp"

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


class MQTTClient:
        virtual public EventGenerator,
        virtual public EventReceiver,
        public mosqpp::mosquittopp
{
  private:
    const char* host;
    const char* id;
    const char* defaultTopic;
    int port;
    int keepalive;
    bool connected = false;

    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);
    void on_message(const struct mosquitto_message* message);

  public:
    MQTTClient(const char* id, const char* topic, const char* host, int port);
    MQTTClient(string id, string topic, string host, int port);
    ~MQTTClient();

    /**
     * Treat the configured topic as parent path,
     * append a "/" and publish to the specified subtopic
     */
    bool sendMessage(char* msg, uint8_t length, char* topic);
    bool sendMessage(string msg, string topic);

    void event(event_t*);
};


#endif
