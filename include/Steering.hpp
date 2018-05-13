
#ifndef STEERING_HPP
#define STEERING_HPP

#include "EventProcessor.hpp"


class Steering:
        virtual public JoystickEventProcessor
{
public:
    void eventButton(js_event_t*);
    void eventAxis(js_event_t*);
};

#endif
