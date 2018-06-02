#include <iostream>
#include <fstream>
#include <vector>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
 
using namespace std;

/*
 * A class to read data from a csv file.
 */
class CSVReader
{
	string fileName;
	string delimiter;
 
public:
	CSVReader(string filename, string delm = ",") :
			fileName(filename), delimiter(delm)
	{ }
 
	// Function to fetch data from a CSV File
	vector<vector<string> > getData();
};
 
/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
vector<vector<string> > CSVReader::getData()
{
    bool firstLine = true;
	ifstream file(fileName);
 
	vector<vector<string> > dataList;
 
	string line = "";
	// Iterate through each line and split the content using delimiter
	while (getline(file, line))
	{
        if (firstLine) {
            firstLine = false;
            continue;
        }
	    vector<string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimiter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();
 
	return dataList;
}

using nlohmann::json;
using namespace std;

namespace jConfig {

    // +++ mapping for sensors
    struct sensor_t {
        int nr; // number from floorplan
        int index;
        std::string typ;
        std::string checkerboardId;
        std::string service;
        std::string characteristic;
    };

    void to_json(json& j, const sensor_t& s) {
        j = json{{"nr", s.nr}, 
                 {"index", s.index}, 
                 {"typ", s.typ}, 
                 {"checkerboardId", s.checkerboardId},
                 {"service", s.service}, 
                 {"characteristic", s.characteristic}};
    }

    void from_json(const json& j, sensor_t& s) {
        s.nr             = j.at("nr").get<int>();
        s.index          = j.at("index").get<int>();
        s.typ            = j.at("typ").get<int>();
        s.checkerboardId = j.at("checkerboardId").get<std::string>();
        s.service        = j.at("service").get<std::string>();
        s.characteristic = j.at("characteristic").get<std::string>();
    }


    // +++ mapping for beacon
    struct beacon_t {
        std::string address;
        std::string owner;
        std::string cable;
        std::string location;
        std::vector<sensor_t> sensors;
    };

    void to_json(json& j, const beacon_t& b) {
        j = json{{"address", b.address}, 
                 {"owner", b.owner}, 
                 {"cable", b.cable}, 
                 {"location", b.location}, 
                 {"sensor", b.sensors}};
    }

    void from_json(const json& j, beacon_t& b) {
        b.address = j.at("address").get<std::string>();
        b.owner   = j.at("owner").get<std::string>();
        b.cable   = j.at("cable").get<std::string>();
        b.location= j.at("location").get<std::string>();
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

enum csvIndices {
    NR,
    ADDRESS,
    CABLE,
    OWNER,
    LOCATION,
    CHECKERBOARDID,
    INDEX,
    TYP,
    SERVICE,
    CHARCTERISTIC
};


int main()
{
#define DEFINE_BEACON beacon.address  = bAddress; \
            beacon.owner    = bOwner; \
            beacon.cable    = bCable; \
            beacon.location = bLocation; \

#define DEFINE_SENSOR sensor.nr = sNr; \
            sensor.index = sIndex; \
            sensor.typ = sTyp; \
            sensor.checkerboardId = sCheckerboardId; \
            sensor.service = sService; \
            sensor.characteristic = sCharacteristic;

	// Creating an object of CSVWriter
	CSVReader reader("etc/BaumhausSensorDocumentation.csv");
 
	// Get the data from CSV File
	vector<vector<string> > dataList = reader.getData();
 
    json j;
    jConfig::beacons_t beacons;
    jConfig::beacon_t beacon;
    jConfig::sensor_t sensor;
    
    string lastBeacon = "";
    


    for ( vector<string> vec : dataList ) {
        string bAddress = vec[ADDRESS];
        string bOwner   = vec[OWNER];
        string bCable   = vec[CABLE];
        string bLocation= vec[LOCATION];

        int    sNr             = stoi(vec[NR]); 
        string sCheckerboardId = vec[CHECKERBOARDID];
        int    sIndex          = stoi(vec[INDEX]);
        string sTyp            = vec[TYP];
        string sService        = vec[SERVICE];
        string sCharacteristic = vec[CHARCTERISTIC];

        if (bAddress.length() == 0){

            if (beacon.sensors.size()) {
                beacons.beacons.push_back(beacon);
                beacon.sensors.clear();
            }

            continue;
        }

        if (lastBeacon.length() == 0) {

            lastBeacon = bAddress;
            
            DEFINE_BEACON;
        } 

        if (lastBeacon != bAddress) {

            lastBeacon = bAddress;
            beacons.beacons.push_back(beacon);
            beacon.sensors.clear();
            
            DEFINE_BEACON;
        }    

        DEFINE_SENSOR;

        beacon.sensors.push_back(sensor);

    } // for

    j = beacons;

    cout << setw(2) << j << endl;
}
