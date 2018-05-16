
#include <iostream>
#include <stddef.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

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


int main()
{
    BluetoothEnabledSwitch* alarmlight = new BluetoothEnabledSwitch(
                                                "alarmlightd",
                                                "alarmlight/switch",
                                                "localhost",
                                                1883,
                                                ALARMLIGHT_BEACON_ADDRESS,
                                                ALARMLIGHT_UUID_SERVICE,
                                                ALARMLIGHT_UUID_CHARACTERISTIC
                                                );

    // Sleep until a signal arrives
    pause();

    delete alarmlight;
}
