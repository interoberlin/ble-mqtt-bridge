
#include <MQTTClient.hpp>
#include <BLEClient.hpp>
#include <iostream>
#include <string.h>


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
    loop_stop();

    // Mosquitto library cleanup
    mosqpp::lib_cleanup();
}


bool MQTTClient::send_message(const char * _message)
{
    // Send message - depending on QoS, mosquitto lib managed re-submission this the thread
    //
    // * NULL : Message Id (int *) this allow to latter get status of each message
    // * topic : topic to be used
    // * lenght of the message
    // * message
    // * qos (0,1,2)
    // * retain (boolean) - indicates if message is retained on broker or not
    // Should return MOSQ_ERR_SUCCESS
    int ret = publish(NULL, this->topic, strlen(_message), _message, 1, false);
    return (ret == MOSQ_ERR_SUCCESS);
}


void MQTTClient::on_connect(int rc)
{
    if (rc == 0)
    {
        std::cout << ">> Connected to MQTT broker" << std::endl;
        connected = true;
        bool ble_connected = false;
        publish(NULL, "alarmlight/connected", 1, &ble_connected, 0, true);
        subscribe(NULL, "alarmlight/switch", 0);
    }
    else
    {
        std::cout << ">> Unable to connect to MQTT broker (return code " << rc << ")" << std::endl;
        connected = false;
    }
}


void MQTTClient::on_disconnect(int rc)
{
    std::cout << ">> MQTT broker disconnected (" << rc << ")" << std::endl;
    connected = false;
}


void MQTTClient::on_publish(int mid)
{
    std::cout << ">> MQTT message published (" << mid << ")" << std::endl;
}


void MQTTClient::on_message(const struct mosquitto_message* message)
{
    if (ble_client == NULL)
        return;

    vector<uint8_t> v;

    ble_client->write(v);
}

