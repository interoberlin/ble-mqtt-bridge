
#include "floorsensord/Config.hpp"
#include "debug.h"
#include <iostream>

using namespace floorsensord;


namespace jConfig {

    void to_json(json& j, const sensor_t& s) {
        j = json{{"index", s.index}, {"checkerboardId", s.checkerboardId}};
    }

    void from_json(const json& j, sensor_t& s) {
        // s.index          = j.at("index").get<int>();
        s.index          = j.at("index").get<std::string>();
        s.checkerboardId = j.at("checkerboardId").get<std::string>();
    }

    void to_json(json& j, const beacon_t& b) {
        j = json{{"address", b.address}, {"owner", b.owner}, {"sensor", b.sensors}};
    }

    void from_json(const json& j, beacon_t& b) {
        b.address = j.at("address").get<std::string>();
        b.owner   = j.at("owner").get<std::string>();
        b.sensors = j.at("sensors").get<std::vector<jConfig::sensor_t>>();
    }

    void to_json(json& j, const beacons_t& b) {
        j = json{{"beacons", b.beacons}};
    }

    void from_json(const json& j, beacons_t& b) {
        b.beacons = j.at("beacons").get<std::vector<jConfig::beacon_t>>();
    }
} // namespace jConfig

bool Config::importJSON(ifstream& stream)
{
    bool success = false;
    try
    {
        config = json::parse(stream);
        jconfig = config;
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
    for (auto const& B: jconfig.beacons) {
    
        cerr << endl;
        cerr << "+++++++++" << endl;
        cerr << "Address: " << B.address << endl;
        cerr << "Owner:   " << B.owner << endl;
    
        for (auto const& S: B.sensors) {
    
            cerr << "  checkerboardId: " << S.checkerboardId << endl;
            cerr << "  index:          " << S.index << endl;
            cerr << endl;
    
        }
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

