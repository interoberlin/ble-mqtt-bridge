
#include <MQTTClient.hpp>
#include <iostream>
#include <string.h>

using namespace std;


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
    connect_async(this->host, port, keepalive);

    // Start thread managing connection / publish / subscribe
    loop_start();
}


MQTTClient::MQTTClient(
                string id,
                string topic,
                string host,
                int port
                )
   :mosquittopp(id.c_str())
{
    // Mandatory initialization for mosquitto library
    mosqpp::lib_init();

    // Basic configuration setup
    this->keepalive = 60;
    this->id = id.c_str();
    this->port = port;
    this->host = host.c_str();
    this->topic = topic.c_str();
    connected = false;

    // Start non-blocking connection attempt to broker
    connect_async(this->host, port, keepalive);

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


bool MQTTClient::sendMessage(char* s, uint8_t length)
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


bool MQTTClient::sendFloat(float* f)
{
    char s[30];
    snprintf(s, 30, "%0.01f", *f);
    return sendMessage(s, strlen(s));
}


void MQTTClient::on_connect(int rc)
{
    if (rc == 0)
    {
//        cout << ">> Connected to MQTT broker" << endl;
        connected = true;
    }
    else
    {
//        cout << ">> Unable to connect to MQTT broker (return code " << rc << ")" << endl;
        connected = false;
    }
}


void MQTTClient::on_disconnect(int rc)
{
//    cout << ">> MQTT broker disconnected (" << rc << ")" << endl;
    connected = false;
}


void MQTTClient::on_publish(int mid)
{
//    cout << ">> MQTT message published (" << mid << ")" << endl;
}


//void MQTTClient::on_message(const struct mosquitto_message* message)
//{
//}

