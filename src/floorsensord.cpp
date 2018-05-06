
#include <iostream>
#include <fstream>
#include <vector>
#include <stddef.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <getopt.h>
#include <stdlib.h>
#include <Bridge.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
using namespace std::chrono_literals;

// Dem Dingens vom Flo
// #define FLOORSENSOR_BEACON_ADDRESS1         "4D:FA:10:CC:26:0D"
// Dem Dingens vom Götz
// #define FLOORSENSOR_BEACON_ADDRESS2         "F6:56:9D:78:F0:2E"

#define FLOORSENSOR_UUID_SERVICE            "00002011-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC1    "00002012-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC2    "00002013-0000-1000-8000-00805f9b34fb"
#define FLOORSENSOR_UUID_CHARACTERISTIC3    "00002014-0000-1000-8000-00805f9b34fb"

// command line option global vars
bool config_check_flag = false;
typedef enum { DEBUG_NONE=0, DEBUG_NORMAL, DEBUG_MORE, DEBUG_MOST, DEBUG_ALL=9} debug_flags;
debug_flags debug_flag = DEBUG_NONE;

vector<Bridge*> bridges;


void bridge_floorsensor(json config)
{

    struct bridge_t {
        std::string mqttClientName;
        std::string mqttTopic;
        std::string mqttHost;
        int mqttPort;
        BLEClientRole::role_enum bleClienRole;
        std::string bleAddress;
        std::string bleOwner;
        std::string bleLocation;
        std::string bleService;
        std::string bleCharacteristic;
    } bridge_config;

    // constant bridge-values
    bridge_config.mqttClientName = "floorsensor-daemon";
    bridge_config.mqttHost = "localhost";
    bridge_config.mqttPort = 1883;

    bridge_config.bleClienRole = BLEClientRole::READER;
    bridge_config.bleService = FLOORSENSOR_UUID_SERVICE;
    bridge_config.bleCharacteristic = FLOORSENSOR_UUID_CHARACTERISTIC3;

    //  parse the json-config
    for (uint beacon = 0; beacon < config["beacons"].size(); beacon++) {

        bridge_config.bleAddress = config["beacons"][beacon]["address"];
        bridge_config.bleOwner   = config["beacons"][beacon]["owner"];
        bridge_config.bleLocation= \
                config["beacons"][beacon]["location"].empty()?\
                "NotDefined":\
                config["beacons"][beacon]["location"];

        if ( debug_flag > DEBUG_MORE ) {
            cout << " " << beacon << ": [" << bridge_config.bleAddress << "]"
                 << " " << "[" << bridge_config.bleOwner << "]"
                 << " " << "[" << bridge_config.bleLocation << "]"             << endl << flush;
        }

        for (uint sensor = 0; sensor < config["beacons"][beacon]["sensors"].size(); sensor++) {

           bridge_config.mqttTopic = config["beacons"][beacon]["sensors"][sensor]["topic"];

           if ( debug_flag > DEBUG_MORE ) {
                cout << "    " << sensor << ": [" << bridge_config.mqttTopic << "]" << endl << flush;
           }
        } // sensor

        try {
            if ( debug_flag > DEBUG_NORMAL ) {
                cout << ">> define " << bridge_config.bleAddress << "->" << bridge_config.mqttTopic << endl << flush;
            }

            if ( !config_check_flag ) {
                Bridge* bridge = new Bridge(
                                        bridge_config.mqttClientName.c_str(),
                                        bridge_config.mqttTopic.c_str(),
                                        bridge_config.mqttHost.c_str(),
                                        bridge_config.mqttPort,
                                        bridge_config.bleClienRole,
                                        bridge_config.bleAddress,
                                        bridge_config.bleService,
                                        bridge_config.bleCharacteristic
                                    );
                bridges.push_back(bridge);
            } // config_check_flag

        } catch (exception& e) {
            cerr << e.what();
        }

    } // beacon

}

int main(int argc, char* argv[])
{
    json floorsensor_config;
    std::string floorsensor_config_filename = "floorsensor_config.json"; 

    int option_char;

    while ((option_char = getopt (argc, argv, "cd:f:?")) != EOF) {
        switch (option_char) {
            case 'd': 
                debug_flag = (debug_flags) atoi(optarg); 
                break;
            case 'c': 
                config_check_flag = true; 
                debug_flag = DEBUG_ALL; 
                break;
            case 'f': 
                floorsensor_config_filename = optarg; 
                break;
            case '?':
                 cerr << "usage: " << argv[0] << " [cf<val>d<val>]" << endl; break;
        }
    }

    std::ifstream configuration_file (floorsensor_config_filename);

    try {
        floorsensor_config = json::parse(configuration_file);
    } catch (exception& e) {
        cerr << e.what() << endl;
    }

    bridge_floorsensor(floorsensor_config);

    if ( !config_check_flag ) {
        // Sleep until a signal arrives
        pause();

        // cleanup, destroy all bridges
        for (std::vector<Bridge*>::iterator it = bridges.begin(); it != bridges.end(); ++it) {
            delete *it;
        }
    } // config_check_flag

}
