
#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include <BLEClient.hpp>
#include <MQTTClient.hpp>
#include <string>

using namespace std;


class Bridge
{
private:
    BLEClient* ble_client;
    MQTTClient* mqtt_client;

public:
    Bridge(
        const char* mqttClientName,
        const char* mqttTopic,
        const char* mqttHost,
        int mqttPort,
        BLEClientRole::role_enum bleClientRole,
        string bleAddress,
        string bleService,
        string bleCharacteristic
        );
    ~Bridge();
};


#endif
