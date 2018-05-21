/**
 * @file
 * Import floorsensor daemon configuration from JSON
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <fstream>
// Library from https://github.com/nlohmann/json
#include <nlohmann/json.hpp>
#include "endpoints/BLEClient.hpp"
#include "endpoints/MQTTClient.hpp"
#include "floorsensord/ValueSplitter.hpp"

using json = nlohmann::json;
using namespace std;


namespace floorsensord
{
    const string UUID_SERVICE = "00002011-0000-1000-8000-00805f9b34fb";
    const string UUID_CHARACTERISTIC1 = "00002012-0000-1000-8000-00805f9b34fb";
    const string UUID_CHARACTERISTIC2 = "00002013-0000-1000-8000-00805f9b34fb";
    const string UUID_CHARACTERISTIC3 = "00002014-0000-1000-8000-00805f9b34fb";


    /**
     * Imports and handles all configuration information
     */
    class Config
    {
    private:
        json config;

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



    public:
        /**
         * Import configuration from JSON formatted file
         */
        bool importJSON(ifstream& stream);
        bool importJSON(string filename);

        /**
         * Print a summary of what has been imported
         */
        void printCheck();

        /**
         * Use the imported config to fill a vector
         * with newly created BLEClient objects
         */
        void generateBeacons(vector<BLEClient*>& beaconList);

        /**
         * Use the imported config to fill a vector
         * with newly created ValueSplitter objects
         */
        void generateValueSplitters(vector<ValueSplitter*>& splitterList, MQTTClient* mqtt);
    };
}

#endif