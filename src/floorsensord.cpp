
#include <iostream>
#include <stddef.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <Bridge.hpp>


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


int main()
{
    Bridge* bridge = new Bridge(
                            "floorsensord",
                            "floorsensor/z0/value",
                            "localhost",
                            1883,
                            BLEClientRole::READER,
                            FLOORSENSOR_BEACON_ADDRESS2,
                            FLOORSENSOR_UUID_SERVICE,
                            FLOORSENSOR_UUID_CHARACTERISTIC3
                            );

    // Sleep until a signal arrives
    pause();

    delete bridge;
}
