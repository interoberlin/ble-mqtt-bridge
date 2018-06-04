
#ifndef EVENTPROCESSOR_HPP

#include "endpoints/MQTTClient.hpp"
#include "JoystickEventReceiver.hpp"


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
