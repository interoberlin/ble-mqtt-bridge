
#include <iostream>
#include <fstream>
#include <vector>
#include <stddef.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <Bridge.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
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


vector<Bridge*> bridges;


void bridge_floorsensor(json config)
{
    try {
        Bridge* bridge = new Bridge(
                               "floorsensor-daemon",
                               "floorsensor/z0",
                               "localhost",
                               1833,
                               BLEClientRole::READER,
                               FLOORSENSOR_BEACON_ADDRESS2,
                               FLOORSENSOR_UUID_SERVICE,
                               FLOORSENSOR_UUID_CHARACTERISTIC3 
                            );
        bridges.push_back(bridge);

    } catch (exception& e) {
        cerr << e.what();
    }
}


int main()
{
    json config;

    std::ifstream configuration_file ("config.json");
    
    try {
        configuration_file >> config;
    } catch (exception& e) {
        cerr << e.what() << endl;
    }

    bridge_floorsensor(config);

    // Sleep until a signal arrives
    pause();

    // cleanup, destroy all bridges
    for (std::vector<Bridge*>::iterator it = bridges.begin(); it != bridges.end(); ++it) {
        delete *it;
    }
}
