#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using nlohmann::json;
using namespace std;

namespace ns {

    // +++ mapping for sensors
    struct sensor {
        // int index;
        std::string index;
        std::string checkerboardId;
    };

    void to_json(json& j, const sensor& s) {
        j = json{{"index", s.index}, {"checkerboardId", s.checkerboardId}};
    }

    void from_json(const json& j, sensor& s) {
        // s.index          = j.at("index").get<int>();
        s.index          = j.at("index").get<std::string>();
        s.checkerboardId = j.at("checkerboardId").get<std::string>();
    }

    // +++ mapping for beacon
    struct beacon {
        std::string address;
        std::string owner;
        std::vector<sensor> sensors;
    };

    void to_json(json& j, const beacon& b) {
        j = json{{"address", b.address}, {"owner", b.owner}, {"sensor", b.sensors}};
    }

    void from_json(const json& j, beacon& b) {
        b.address = j.at("address").get<std::string>();
        b.owner   = j.at("owner").get<std::string>();
        b.sensors = j.at("sensors").get<std::vector<ns::sensor>>();
    }

    // +++ mapping for beacons
    struct beacons {
        std::vector<beacon> beacons;
    };
    
    void to_json(json& j, const beacons& b) {
        j = json{{"beacons", b.beacons}};
    }

    void from_json(const json& j, beacons& b) {
        b.beacons = j.at("beacons").get<std::vector<ns::beacon>>();
    }
}

int main()
{
    json j;
    ns::beacons b;

    ifstream stream("floorsensor_config.json");
    j = json::parse(stream);

    b = j;

    json j2 = b;
    cerr << setw(2) << j2 << endl;

    return true;
}