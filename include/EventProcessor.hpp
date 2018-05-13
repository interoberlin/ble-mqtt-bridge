
#ifndef EVENTPROCESSOR_HPP

#include "MQTTClient.hpp"
#include "EventReceiver.hpp"


class JoystickEventProcessor:
        virtual public JoystickEventReceiver
{
protected:
    MQTTClient* mqtt;

public:
    JoystickEventProcessor(): JoystickEventReceiver() { mqtt = NULL; }

    void registerMQTTClient(MQTTClient* mqtt) { this->mqtt = mqtt; }
};

#endif
