/**
 * @file
 * A MQTT client, which subscribes to alarm lamp state change messages
 */

#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <stdint.h>
#include <mosquittopp.h>
#include "events/Event.hpp"
#include "events/EventGenerator.hpp"
#include "events/EventReceiver.hpp"

using namespace std;



class MQTTClient:
        virtual public EventGenerator,
        virtual public EventReceiver,
        public mosqpp::mosquittopp
{
  private:
    const char* host;
    const char* id;
    const char* topic;
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

    /** Update topic for next call to sendMessage() */
    void setTopic(string s);

    bool sendMessage(char* msg, uint8_t length);
    bool sendMessage(string msg);

    /**
     * Treat the configured topic as parent path,
     * append a "/" and publish to the specified subtopic
     */
    bool sendMessage(char* subtopic, char* msg, uint8_t length);
    bool sendMessage(string subtopic, string msg);

    void event(event_t*);
};


#endif
