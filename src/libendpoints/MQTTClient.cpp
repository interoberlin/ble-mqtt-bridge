
#include "endpoints/MQTTClient.hpp"
#include <iostream>
#include <string.h>

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>


MQTTClient::MQTTClient(
                const char* id,
                const char* topic,
                const char* host,
                int port
                )
   :EventReceiver(),
    mosquittopp(id)
{
    // Mandatory initialization for mosquitto library
    mosqpp::lib_init();

    // Basic configuration setup
    this->keepalive = 60;
    this->id = id;
    this->port = port;
    this->host = host;
    this->topic = topic;
    this->defaultTopic = topic;
    connected = false;

    // Start non-blocking connection attempt to broker
    connect_async(host, port, keepalive);

    // Start thread managing connection / publish / subscribe
    loop_start();
}


MQTTClient::MQTTClient(
                string id,
                string topic,
                string host,
                int port
                )
   :MQTTClient(
           id.c_str(),
           topic.c_str(),
           host.c_str(),
           port
           )
{}


MQTTClient::~MQTTClient()
{
    // Kill the thread
    loop_stop(true);

    // Mosquitto library cleanup
    mosqpp::lib_cleanup();
}


void MQTTClient::on_connect(int rc)
{
    if (rc == 0)
    {
        DLOG_S(INFO) << "Connected to MQTT broker";  
        
        connected = true;
    }
    else
    {
        LOG_S(ERROR) << ">> Unable to connect to MQTT broker (return code " << rc << ")";
        connected = false;
    }
}


void MQTTClient::on_disconnect(int rc)
{
    
    LOG_S(INFO) << ">> MQTT broker disconnected (" << rc << ")";
    
    connected = false;
}


void MQTTClient::on_publish(int mid)
{
        DLOG_S(9) << ">> MQTT message published (" << mid << ")";
 
}


void MQTTClient::on_message(const struct mosquitto_message* message)
{
    
    DLOG_S(INFO) << ">> MQTT message received" << endl;
    

    // No one is listening, so just discard the message
    if (!this->hasEventReceiver())
        return;

    // Don't forward empty messages
    if (message->payloadlen == 0)
        return;

    // Convert message payload to event
    event_t e;
    e.source = EventSource::MQTT;
    e.mqttMessage = (mqtt_message_t*) message;

    // Generate the event
    getEventReceiver()->event(&e);
}


void MQTTClient::setTopic(string topic)
{
    this->topic = topic.c_str();
}


bool MQTTClient::sendMessage(char* s, uint8_t length)
{
    if (!connected)
        return false;

    LOG_S(INFO) << "dTopic [" << this->defaultTopic << "] topic [" << this->topic << "]";

    int ret = publish(NULL, this->topic, length, s, 1, false);
    return (ret == MOSQ_ERR_SUCCESS);
}


bool MQTTClient::sendMessage(string msg)
{
    return sendMessage((char*) msg.c_str(), (uint8_t) msg.length());
}


bool MQTTClient::sendMessage(char* subtopic, char* msg, uint8_t length)
{
    bool ret;

    setTopic(string(defaultTopic) + "/" + string(subtopic));
    
    ret = sendMessage(msg, length);

    setTopic(string(defaultTopic));

    return ret;
}


bool MQTTClient::sendMessage(string subtopic, string msg)
{
    bool ret;

    
    setTopic(string(defaultTopic) + "/" + subtopic);
    
    ret = sendMessage(msg);

    setTopic(string(defaultTopic));

    return ret;
}


void MQTTClient::event(event_t* e)
{
    switch (e->source)
    {
        case EventSource::BLE:
            sendMessage(e->bleData, e->bleDataLength);
            break;
        case EventSource::SPLITTER:
            sendMessage(e->checkerboardId, to_string(e->sensorValue));
            break;
        default:
            break;
    }
}

