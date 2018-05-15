
#include "floorsensord/Config.hpp"
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
    // TODO
}


void Config::generateBeacons(vector<BLEClient*>& beaconList)
{
    // TODO
}


void Config::generateValueSplitters(vector<ValueSplitter*>& splitterList, MQTTClient* mqtt)
{
    // TODO
}

