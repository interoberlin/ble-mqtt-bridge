
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

#define ALARMLIGHT_BEACON_ADDRESS       "87:10:DC:E9:6D:50"
#define UUID_SERVICE_ALARMLIGHT         "00004000-0000-1000-8000-00805f9b34fb"
#define UUID_CHARACTERISTIC_ALARMLIGHT  "00004001-0000-1000-8000-00805f9b34fb"


int main()
{
    MQTTClient* mqtt_client = new MQTTClient(
                                    "alarmlight-daemon",
                                    "alarmlight",
                                    "localhost",
                                    8883
                                    );

    BLEClient* ble_client = new BLEClient(
                                    ALARMLIGHT_BEACON_ADDRESS,
                                    UUID_SERVICE_ALARMLIGHT,
                                    UUID_CHARACTERISTIC_ALARMLIGHT
                                    );

    mqtt_client->ble_client = ble_client;
    ble_client->mqtt_client = mqtt_client;

    ble_client->process();


    this_thread::sleep_for(5s);
}
