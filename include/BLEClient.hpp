
#ifndef BLECLIENT_HPP
#define BLECLIENT_HPP


#include <stdint.h>
#include <thread>
#include <tinyb.hpp>

using namespace std;
using namespace tinyb;

class MQTTClient;


class BLEClient
{
public:
    string device_address;
    string uuid_service;
    string uuid_characteristic;

    BluetoothManager *manager = NULL;
    BluetoothDevice* device = NULL;
    BluetoothGattService* service = NULL;
    BluetoothGattCharacteristic* characteristic = NULL;

    MQTTClient* mqtt_client = NULL;

    BLEClient(string address, string service, string characteristic);
    ~BLEClient();

    void process();
    void write(vector<uint8_t>&);
};


#endif
