
#ifndef BLUETOOTHSWITCH_HPP
#define BLUETOOTHSWITCH_HPP

#include "events/Event.hpp"
#include "events/EventGenerator.hpp"
#include "events/EventReceiver.hpp"

#include "endpoints/BLEClient.hpp"
#include "endpoints/MQTTClient.hpp"

/*
 * TODO:
 * For now, the beacon configuration is hard-coded.
 * Later on, this can be out-sourced, e.g. to a configuration file.
 */
#define ALARMLIGHT_BEACON_ADDRESS           "87:10:DC:E9:6D:50"
#define ALARMLIGHT_UUID_SERVICE             "00004000-0000-1000-8000-00805f9b34fb"
#define ALARMLIGHT_UUID_CHARACTERISTIC      "00004001-0000-1000-8000-00805f9b34fb"


class BluetoothEnabledSwitch:
        virtual public EventReceiver,
        virtual public EventGenerator
{
private:
    MQTTClient* mqtt;
    BLEClient* ble;

public:
    BluetoothEnabledSwitch();
    BluetoothEnabledSwitch(
        const char* mqtt_id, 
        const char* mqtt_topic, 
        const char* mqtt_host, 
        int         mqtt_port, 
        const char* ble_address, 
        const char* ble_service, 
        const char* ble_characteristic);


    void event(event_t*);
};


#endif
