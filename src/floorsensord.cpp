
#include <iostream>
#include <stddef.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <BLEClient.hpp>
#include <MQTTClient.hpp>


using namespace std;
using namespace std::chrono_literals;

// Dem Dingens vom Flo
#define FLOORSENSOR_BEACON_ADDRESS1         "4D:FA:10:CC:26:0D"
// Dem Dingens vom Götz
#define FLOORSENSOR_BEACON_ADDRESS2         "F6:56:9D:78:F0:2E"
#define FLOORSENSOR_UUID_SERVICE            "00002011-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC1    "00002012-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC2    "00002013-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC3    "00002014-0000-1000-8000-00805f9b34fb"


MQTTClient* mqtt_client[2] = {NULL};
BLEClient* ble_client[2] = {NULL};


void bridge_floorsensor()
{
    mqtt_client[0] = new MQTTClient(
                            "floorsensor-daemon",
                            "floorsensor/z0",
                            "localhost",
                            1883
                            );

    ble_client[0] = new BLEClient(
                            BLEClientRole::READER,
                            FLOORSENSOR_BEACON_ADDRESS1,
                            FLOORSENSOR_UUID_SERVICE,
                            FLOORSENSOR_UUID_CHARACTERISTIC3
                            );

    ble_client[0]->registerReadEventReceiver(mqtt_client[0]);

    mqtt_client[1] = new MQTTClient(
                            "floorsensor-daemon",
                            "floorsensor/y0",
                            "localhost",
                            1883
                            );

    ble_client[1] = new BLEClient(
                            BLEClientRole::READER,
                            FLOORSENSOR_BEACON_ADDRESS2,
                            FLOORSENSOR_UUID_SERVICE,
                            FLOORSENSOR_UUID_CHARACTERISTIC3
                            );

    ble_client[1]->registerReadEventReceiver(mqtt_client[1]);
}


int main()
{
    bridge_floorsensor();

//    while (true);
//    this_thread::sleep_for(5s);

    // Sleep until a signal arrives
    pause();

    delete ble_client[0];
    delete mqtt_client[0];
    delete ble_client[1];
    delete mqtt_client[1];
}
