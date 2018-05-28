
#ifndef JOYSTICKEVENTGENERATOR_HPP
#define JOYSTICKEVENTGENERATOR_HPP

#include <stddef.h>

#include "EventReceiver.hpp"


/**
 * Any class, which generates joystick events,
 * must be a child of this class.
 */
class JoystickEventGenerator
{
protected:
    /** Receiver for joystick events */
    JoystickEventReceiver* eventReceiver = NULL;

public:
    JoystickEventGenerator(): eventReceiver(NULL) {};
    virtual ~JoystickEventGenerator() {};

    /**
     * Determine, whether an event receiver has been registered
     */
    bool hasEventReceiver() { return (eventReceiver != NULL); }

    /**
     * Register an object to receive joystick events
     */
    void registerEventReceiver(JoystickEventReceiver* e) { eventReceiver = e; }
};

#endif
