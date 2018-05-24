/**
 * @file
 * Reads a floorsensor configuration file and proceeds by
 * connecting to all configured BLE beacons.
 * Received values are published to an MQTT broker.
 */

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

//  https://github.com/emilk/loguru.git
#define LOGURU_IMPLEMENTATION 1
#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

#include "floorsensord/Config.hpp"
#include "floorsensord/ValueSplitter.hpp"
#include "endpoints/BLEClient.hpp"
#include "endpoints/MQTTClient.hpp"
#include "events/Event.hpp"
#include "events/EventReceiver.hpp"
#include "events/EventGenerator.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace floorsensord;



/** Print configuration check and exit? */
bool config_check_flag = false;

/** Filename from which to read configuration */
string config_filename = "floorsensor_config.json";


/*
 * Variables for device bridging
 */

/** All values are read from BLE beacons, one object per beacon. */
vector<BLEClient*> beacons;

/** The beacon and sensor configuration is read from a JSON file. */
Config config;

/** The values read from a beacon are passed to a splitter. */
vector<ValueSplitter*> splitters;

/** The values generated by the splitters are passed to one single MQTT publisher. */
MQTTClient* mqtt;

/**
 * Parse and interpret command line arguments
 */
void parseArgs(int argc, char* argv[])
{
    int option_char;

    // let loguru pic an eventually defined -v verbosity level
    loguru::init(argc, argv);

    while ((option_char = getopt (argc, argv, "cd:f:?")) != EOF) {
        switch (option_char) {
            case 'c': 
                config_check_flag = true; 
                break;
            case 'f': 
                config_filename = optarg;
                break;
            case '?':
                 cerr << "usage: " << argv[0] << " [cf<val>d<val>]" << endl; break;
        }
    }
}


int main(int argc, char* argv[])
{
    parseArgs(argc, argv);

    Config config;
    config.importJSON(config_filename);

    if (config_check_flag)
    {
        config.printCheck();
    }
    else
    {
        // Create an MQTT publisher
        mqtt = new MQTTClient(
                        "floorsensord",
                        "floorsensor",
                        "localhost",
                        1883
                        );

        // Create a BLEClient for every configured beacon
        config.generateBeacons(beacons);
        // Create a floorsensor value splitter, that publishes to MQTT
        config.generateValueSplitters(splitters, mqtt);
        // associate beacons and splitters
        config.associateBeaconsAndValueSplitters(beacons, splitters);

        // At this point all activity is event-driven

        // Sleep until a signal arrives
        pause();

        // Cleanup: Delete created objects
        for (vector<BLEClient*>::iterator it = beacons.begin(); it != beacons.end(); ++it)
        {
            delete *it;
        }
        for (vector<ValueSplitter*>::iterator it = splitters.begin(); it != splitters.end(); ++it)
        {
            delete *it;
        }
        delete mqtt;
    } // config_check_flag
}
