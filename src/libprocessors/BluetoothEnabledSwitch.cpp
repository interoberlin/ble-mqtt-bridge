
#include "processors/BluetoothEnabledSwitch.hpp"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

// BluetoothEnabledSwitch::BluetoothEnabledSwitch():
BluetoothEnabledSwitch::BluetoothEnabledSwitch(
        const char* mqtt_id, 
        const char* mqtt_topic, 
        const char* mqtt_host, 
        int         mqtt_port, 
        const char* ble_address, 
        const char* ble_service, 
        const char* ble_characteristic):
        // ;BluetoothEnabledSwitch::BluetoothEnabledSwitch():
    EventReceiver(),
    EventGenerator()
{
    mqtt = new MQTTClient(
        mqtt_id,
        mqtt_topic,
        mqtt_host,
        mqtt_port
                    // "alarmlight-daemon",
                    // "alarmlight/switch",
                    // "localhost",
                    // 1883
                    );

    ble = new BLEClient(
                    BLEClientRole::WRITER,
                    ble_address,
                    ble_service,
                    ble_characteristic
                    // ALARMLIGHT_BEACON_ADDRESS,
                    // ALARMLIGHT_UUID_SERVICE,
                    // ALARMLIGHT_UUID_CHARACTERISTIC
                    );

    mqtt->subscribe(NULL, mqtt_topic, 1);
    mqtt->registerEventReceiver(this);
    this->registerEventReceiver(ble);
}


void BluetoothEnabledSwitch::event(event_t* e)
{
    LOG_S(INFO) << "got event " << string((char*)e->mqttMessage->payload);
    int val = atoi((char*)e->mqttMessage->payload);
    vector<unsigned char> v;
    v.push_back(val);
    this->ble->write(v);
}

