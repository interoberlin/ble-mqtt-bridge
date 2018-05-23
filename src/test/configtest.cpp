#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using nlohmann::json;
using namespace std;

namespace jConfig {

    // +++ mapping for sensors
    struct sensor_t {
        // int index;
        std::string index;
        std::string checkerboardId;
    };

    void to_json(json& j, const sensor_t& s) {
        j = json{{"index", s.index}, {"checkerboardId", s.checkerboardId}};
    }

    void from_json(const json& j, sensor_t& s) {
        // s.index          = j.at("index").get<int>();
        s.index          = j.at("index").get<std::string>();
        s.checkerboardId = j.at("checkerboardId").get<std::string>();
    }

    // +++ mapping for beacon
    struct beacon_t {
        std::string address;
        std::string owner;
        std::vector<sensor_t> sensors;
    };

    void to_json(json& j, const beacon_t& b) {
        j = json{{"address", b.address}, {"owner", b.owner}, {"sensor", b.sensors}};
    }

    void from_json(const json& j, beacon_t& b) {
        b.address = j.at("address").get<std::string>();
        b.owner   = j.at("owner").get<std::string>();
        b.sensors = j.at("sensors").get<std::vector<jConfig::sensor_t>>();
    }

    // +++ mapping for beacons
    struct beacons_t {
        std::vector<beacon_t> beacons;
    };
    
    void to_json(json& j, const beacons_t& b) {
        j = json{{"beacons", b.beacons}};
    }

    void from_json(const json& j, beacons_t& b) {
        b.beacons = j.at("beacons").get<std::vector<jConfig::beacon_t>>();
    }
}

int main()
{
    json j;
    jConfig::beacons_t b;

    ifstream stream("floorsensor_config.json");
    j = json::parse(stream);

    b = j;

    json j2 = b;
    cerr << setw(2) << j2 << endl;

    for (auto const& B: b.beacons) {
        cerr << "Address: " << B.address << endl;
        cerr << "Owner:   " << B.owner << endl;
        for (auto const& S: B.sensors) {
            cerr << "  checkerboardId: " << S.checkerboardId << endl;
            cerr << "  index:          " << S.index << endl;
        }
    }
    
    return true;
}