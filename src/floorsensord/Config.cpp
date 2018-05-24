
#include <iostream>
#include "floorsensord/Config.hpp"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

using namespace floorsensord;


namespace jConfig {

    void to_json(json& j, const sensor_t& s) {
        j = json{{"index", s.index}, {"checkerboardId", s.checkerboardId}};
    }

    void from_json(const json& j, sensor_t& s) {
        s.index          = j.at("index").get<int>();
        // s.index          = j.at("index").get<std::string>();
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
        LOG_S(INFO) << "Look at my custom object: " << e.what();
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
        
        LOG_S(INFO) << "Address: " << B.address;
        LOG_S(INFO) << "Owner:   " << B.owner;
    
        for (auto const& S: B.sensors) {
    
            LOG_S(INFO) << "  checkerboardId: " << S.checkerboardId;
            LOG_S(INFO) << "  index:          " << S.index;
    
        }
    }
}


void Config::generateBeacons(vector<BLEClient*>& beaconList)
{
    for (auto const& B: jconfig.beacons) {

        BLEClient* b = new BLEClient(
            BLEClientRole::READER, 
            B.address, 
            UUID_SERVICE,
            UUID_CHARACTERISTIC3);     

        beaconList.push_back(b);
    }
}


void Config::generateValueSplitters(vector<ValueSplitter*>& splitterList, MQTTClient* mqtt)
{
    for (auto const& B: jconfig.beacons) {

        ValueSplitter *v = new ValueSplitter();
        v->registerEventReceiver(mqtt);

        for (auto const& S: B.sensors) {
    
             v->setCheckerboardId( S.index, S.checkerboardId );
             v->enableSensor( S.index );
        }
        
        splitterList.push_back(v);
    }

}

void Config::associateBeaconsAndValueSplitters(vector<BLEClient*>& beaconList, vector<ValueSplitter*>& splitterList) 
{
    for (uint8_t i = 0; i < beaconList.size(); i++) {
        beaconList[i]->registerEventReceiver(splitterList[i]);
    }
}