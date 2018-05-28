
#include "MQTTClientRobby.hpp"
#include "endpoints/BLEClient.hpp"
#include <iostream>
#include <string.h>
#include <stdlib.h>

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

// shall I write the message to the BLEClient
#define with_ble 1

#define ROBBY_TOPIC_STEER              "joystick/cyborg3d/1"
#define ROBBY_TOPIC_DRIVE              "joystick/cyborg3d/0"
#define ROBBY_STEER_THRESHOLD           999
#define ROBBY_DRIVE_THRESHOLD           999

MQTTClientRobby::MQTTClientRobby(
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

    
    LOG_S(INFO) << "MQTT connect to " << this->topic;

    // Start non-blocking connection attempt to broker
    connect_async(host, port, keepalive);

    // Start thread managing connection / publish / subscribe
    loop_start();
}


MQTTClientRobby::~MQTTClientRobby()
{
    // Kill the thread
    loop_stop(true);

    // Mosquitto library cleanup
    mosqpp::lib_cleanup();
}


bool MQTTClientRobby::send_message(char* s, uint8_t length)
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


void MQTTClientRobby::on_connect(int rc)
{
    if (rc == 0)
    {
        LOG_S(INFO) << ">> Connected to MQTT broker";
        connected = true;
    }
    else
    {
        LOG_S(WARNING) << ">> Unable to connect to MQTT broker (return code " << rc << ")";
        connected = false;
    }
}


void MQTTClientRobby::on_disconnect(int rc)
{
    LOG_S(WARNING) << ">> MQTT broker disconnected (" << rc << ")";
    connected = false;
}


void MQTTClientRobby::on_publish(int mid)
{
    LOG_S(INFO) << ">> MQTT message published (" << mid << ")";
}

void MQTTClientRobby::on_message(const struct mosquitto_message* message)
{
    LOG_S(INFO) << ">> MQTT message received";

#if with_ble
    if (ble_client == NULL)
        // Not BLE device connected
        return;
#endif

    // Don't forward empty messages
    if (message->payloadlen == 0)
        return;

    string base_topic = "joystick/cyborg3d";
    string topic = string(message->topic);

    // Default: Stop
    char cmd = Robby::Command::Stop;

    if (topic == base_topic + "/" + Robby::Subtopic::Forward)
    {
        cmd = Robby::Command::Forward;
    }
    else if (topic == base_topic + "/" + Robby::Subtopic::Backward)
    {
        cmd = Robby::Command::Backward;
    }
    else if (topic == base_topic + "/" + Robby::Subtopic::Left)
    {
        cmd = Robby::Command::Left;
    }
    else if (topic == base_topic + "/" + Robby::Subtopic::Right)
    {
        cmd = Robby::Command::Right;
    }
    else if (topic == base_topic + "/" + Robby::Subtopic::RotateLeft)
    {
        cmd = Robby::Command::RotateLeft;
    }
    else if (topic == base_topic + "/" + Robby::Subtopic::RotateRight)
    {
        cmd = Robby::Command::RotateRight;
    }
    
    // Convert message payload to vector
    vector<uint8_t> v;
    v.push_back(cmd);
    
#if with_ble    // Output generated vector via BLE
    ble_client->write(v);
#endif
    LOG_S(INFO) << "ble: wrote [" << cmd << "]";

}

