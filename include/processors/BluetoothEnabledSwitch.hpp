
#ifndef BLUETOOTHSWITCH_HPP
#define BLUETOOTHSWITCH_HPP

#include "events/Event.hpp"
#include "events/EventGenerator.hpp"
#include "events/EventReceiver.hpp"

#include "endpoints/BLEClient.hpp"
#include "endpoints/MQTTClient.hpp"


class BluetoothEnabledSwitch:
        virtual public EventReceiver,
        virtual public EventGenerator
{
private:
    MQTTClient* mqtt;
    BLEClient* ble;

public:
    BluetoothEnabledSwitch();

    void event(event_t*);
};


#endif
