#include <iostream>
#include <string>
#include <string.h>

#include "endpoints/MQTTClient.hpp"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

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
    // this->topic = topic;
    this->defaultTopic = topic;
    connected = false;

    // Start non-blocking connection attempt to broker
    connect_async(host, port, keepalive);

    LOG_S(INFO) << "mqtt: starting loop";
    
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
        DLOG_S(7) << "Connected to MQTT broker";  
        
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
    
    LOG_S(7) << ">> MQTT broker disconnected (" << rc << ")";
    
    connected = false;
}


void MQTTClient::on_publish(int mid)
{
        DLOG_S(9) << ">> MQTT message published (" << mid << ")";
 
}


void MQTTClient::on_message(const struct mosquitto_message* message)
{
    
    LOG_S(INFO) << ">> MQTT message received";
    

    // No one is listening, so just discard the message
    if (!this->hasEventReceiver())
        return;

    // Don't forward empty messages
    if (message->payloadlen == 0)
        return;

    // Convert message payload to event
    event_t e;
    e.source = EventSource::MQTT;
    e.evt.mqttMessage = (mqtt_message_t*) message;

    // Generate the event
    getEventReceiver()->event(&e);
}


// void MQTTClient::setTopic(string topic)
// {
//     LOG_S(INFO) << "setTopic: " << topic;
//     this->topic = topic.c_str();
//     LOG_S(INFO) << "setTopic2: " << this->topic;
// }


bool MQTTClient::sendMessage(char* s, uint8_t length, char* topic)
{
    if (!connected)
        return false;
    
    // LOG_S(INFO) << "sendMessage: dTopic [" << this->defaultTopic << "]"

    int ret = publish(NULL, topic, length, s, 1, false);
    return (ret == MOSQ_ERR_SUCCESS);
}


bool MQTTClient::sendMessage(string msg, string subtopic /* = "" */)
{
    string topic = this->defaultTopic;
    
    if (subtopic.size()) {
        topic += "/";
        topic += subtopic;
    }
    
    return sendMessage((char*) msg.c_str(), (uint8_t) msg.length(), (char*) topic.c_str());
}

bool MQTTClient::sendMessage(float* f )
{
        char s[30];
    snprintf(s, 30, "%0.01f", *f);
    return sendMessage(string(s));

}

void MQTTClient::event(event_t* e)
{
    switch (e->source)
    {
        case EventSource::BLE:
            sendMessage(e->evt.ble_raw.bleData, e->evt.ble_raw.bleDataLength, NULL);
            break;
        case EventSource::SPLITTER:
            sendMessage(to_string(e->evt.ble_processed.sensorValue), e->evt.ble_processed.checkerboardId);
            break;
        default:
            break;
    }
}

void MQTTClient::setThreadName(std::string name /* = mqtt: NN */)
{
    string topic = string(this->defaultTopic);
    string threadName = ".../";
    
    threadName += topic;

    if (threadName.length() >= 16) {
        threadName = threadName.substr(threadName.length()-15, threadName.length());
    }
    
    loguru::set_thread_name(threadName.c_str());
    int rc = pthread_setname_np(pthread_self(), threadName.c_str());
    if (rc) {
        LOG_S(ERROR) << "pthread_setname_np returned " << rc << "on topic:" << this->defaultTopic;
    }
}