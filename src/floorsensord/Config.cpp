
#include "floorsensord/Config.hpp"
#include "debug.h"
#include <iostream>

using namespace floorsensord;


bool Config::importJSON(ifstream& stream)
{
    bool success = false;
    try
    {
        config = json::parse(stream);
        success = true;
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
    }
    return success;
}


bool Config::importJSON(string filename)
{
    ifstream stream(filename);
    return importJSON(stream);
}


void Config::printCheck()
{
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
                 << " " << "[" << bridge_config.bleLocation << "]"  << endl << flush;
        }

        for (uint sensor = 0; sensor < config["beacons"][beacon]["sensors"].size(); sensor++) {

           bridge_config.mqttTopic = config["beacons"][beacon]["sensors"][sensor]["topic"];

           if ( debug_flag > DEBUG_MORE ) {
                cout << "    " << sensor << ": [" << bridge_config.mqttTopic << "]" << endl << flush;
           }
        } // sensor
    }

}


void Config::generateBeacons(vector<BLEClient*>& beaconList)
{
    // TODO
}


void Config::generateValueSplitters(vector<ValueSplitter*>& splitterList, MQTTClient* mqtt)
{
    // TODO
}

