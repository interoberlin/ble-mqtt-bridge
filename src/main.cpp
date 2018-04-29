
#include <iostream>
#include <stddef.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <BLEClient.hpp>
#include <MQTTClient.hpp>


using namespace std;
using namespace std::chrono_literals;

/*
 * TODO:
 * For now, the beacon configuration is hard-coded.
 * Later on, this can be out-sourced, e.g. to a configuration file.
 */
#define ALARMLIGHT_BEACON_ADDRESS           "87:10:DC:E9:6D:50"
#define ALARMLIGHT_UUID_SERVICE             "00004000-0000-1000-8000-00805f9b34fb"
#define ALARMLIGHT_UUID_CHARACTERISTIC      "00004001-0000-1000-8000-00805f9b34fb"

// Dem Dingens vom Flo
//#define FLOORSENSOR_BEACON_ADDRESS          "4D:FA:10:CC:26:0D"
// Dem Dingens vom Götz
#define FLOORSENSOR_BEACON_ADDRESS          "F6:56:9D:78:F0:2E"
#define FLOORSENSOR_UUID_SERVICE            "00002011-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC1    "00002012-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC2    "00002013-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC3    "00002014-0000-1000-8000-00805f9b34fb"


MQTTClient* mqtt_client = NULL;
BLEClient* ble_client = NULL;


void bridge_alarmlight()
{
    mqtt_client = new MQTTClient(
                            "alarmlight-daemon",
                            "alarmlight/switch",
                            "localhost",
                            1883
                            );

    ble_client = new BLEClient(
                            BLEClientRole::WRITER,
                            ALARMLIGHT_BEACON_ADDRESS,
                            ALARMLIGHT_UUID_SERVICE,
                            ALARMLIGHT_UUID_CHARACTERISTIC
                            );
}


void bridge_floorsensor()
{
    mqtt_client = new MQTTClient(
                            "floorsensor-daemon",
                            "floorsensor/z0",
                            "localhost",
                            1883
                            );

    ble_client = new BLEClient(
                            BLEClientRole::READER,
                            FLOORSENSOR_BEACON_ADDRESS,
                            FLOORSENSOR_UUID_SERVICE,
                            FLOORSENSOR_UUID_CHARACTERISTIC3
                            );

    ble_client->registerReadEventReceiver(mqtt_client);
}


int main()
{
    bridge_floorsensor();
//    bridge_alarmlight();

    while (true);
//    this_thread::sleep_for(5s);

    delete ble_client;
    delete mqtt_client;
}
