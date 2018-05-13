
#ifndef EVENTPROCESSOR_HPP

#include "MQTTClient.hpp"
#include "EventReceiver.hpp"


class JoystickEventProcessor:
        virtual public JoystickEventReceiver
{
private:
    MQTTClient* mqtt;

public:
    JoystickEventProcessor(): JoystickEventReceiver() { mqtt = NULL; }
    ~JoystickEventProcessor() {};

    void registerMQTTClient(MQTTClient* mqtt) { this->mqtt = mqtt; }

    void eventButton(js_event_t* e)
    {
        if (mqtt == NULL)
            return;

        string s = "Button";
        char* c = (char*) s.c_str();
        mqtt->sendMessage(c, s.length());
    }

    void eventAxis(js_event_t* e)
    {
        if (mqtt == NULL)
            return;

        string s = "Axis";
        char* c = (char*) s.c_str();
        mqtt->sendMessage(c, s.length());
    }
};

#endif
