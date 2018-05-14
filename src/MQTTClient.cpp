
#include <MQTTClient.hpp>
#include <BLEClient.hpp>
#include <iostream>
#include <string.h>

#include "debug.h"

MQTTClient::MQTTClient(
                const char* id,
                const char* topic,
                const char* host,
                int port
                )
   :mosquittopp(id)
{
    // Mandatory initialization for mosquitto library
    mosqpp::lib_init();

    // Basic configuration setup
    this->keepalive = 60;
    this->id = id;
    this->port = port;
    this->host = host;
    this->topic = topic;
    connected = false;

    // Start non-blocking connection attempt to broker
    connect_async(host, port, keepalive);

    // Start thread managing connection / publish / subscribe
    loop_start();
}


MQTTClient::~MQTTClient()
{
    // Kill the thread
    loop_stop(true);

    // Mosquitto library cleanup
    mosqpp::lib_cleanup();
}


bool MQTTClient::send_message(char* s, uint8_t length)
{
    if (!connected)
    {
        return false;
    }

    // Send message - depending on QoS, mosquitto lib managed re-submission this the thread
    //
    // * NULL : Message Id (int *) this allow to latter get status of each message
    // * topic : topic to be used
    // * lenght of the message
    // * message
    // * qos (0,1,2)
    // * retain (boolean) - indicates if message is retained on broker or not
    // Should return MOSQ_ERR_SUCCESS
    int ret = publish(NULL, this->topic, length, s, 1, false);
    return (ret == MOSQ_ERR_SUCCESS);
}


void MQTTClient::on_connect(int rc)
{
    if (rc == 0)
    {
        if (debug_flag > DEBUG_NONE) {
            cout << ">> Connected to MQTT broker" << endl;  
        } 
        connected = true;
    }
    else
    {
        cerr << ">> Unable to connect to MQTT broker (return code " << rc << ")" << endl;
        connected = false;
    }
}


void MQTTClient::on_disconnect(int rc)
{
    if (debug_flag > DEBUG_MORE) {
        cout << ">> MQTT broker disconnected (" << rc << ")" << endl;
    }
    connected = false;
}


void MQTTClient::on_publish(int mid)
{
    // if (debug_flag > DEBUG_NONE) {
        cout << ">> MQTT message published (" << mid << ")" << endl;
    // } 
}


void MQTTClient::on_message(const struct mosquitto_message* message)
{
    if (debug_flag > DEBUG_NONE) {
        cout << ">> MQTT message received" << endl;
    }

    if (ble_client == NULL)
        // Not BLE device connected
        return;

    // Don't forward empty messages
    if (message->payloadlen == 0)
        return;

    // Convert message payload to vector
    vector<uint8_t> v;
    for (uint8_t i=0; i<message->payloadlen; i++)
    {
        v.push_back(((uint8_t*) message->payload)[i]);
    }

    // Output generated vector via BLE
    ble_client->write(v);
}

