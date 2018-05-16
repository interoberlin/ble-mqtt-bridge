
#include "processors/BluetoothEnabledSwitch.hpp"


BluetoothEnabledSwitch::BluetoothEnabledSwitch():
    EventGenerator(),
    EventReceiver()
{
    mqtt = new MQTTClient(
                    "alarmlight-daemon",
                    "alarmlight/switch",
                    "localhost",
                    1883
                    );

    ble = new BLEClient(
                    BLEClientRole::WRITER,
                    ALARMLIGHT_BEACON_ADDRESS,
                    ALARMLIGHT_UUID_SERVICE,
                    ALARMLIGHT_UUID_CHARACTERISTIC
                    );

    mqtt->registerEventReceiver(this);
    this->registerEventReceiver(ble);
}


void BluetoothEnabledSwitch::event(event_t* e)
{
    // We arrive here, when a switch signal was received via MQTT
    // TODO
}

