
#ifndef JOYSTICKEVENTRECEIVER_HPP
#define JOYSTICKEVENTRECEIVER_HPP

#include <linux/joystick.h>

#ifndef js_event_t
typedef struct js_event js_event_t;
#endif


/**
 * Any class, which shall accept joystick events,
 * must be a child of this class and implement
 * the corresponding event methods.
 */
class JoystickEventReceiver
{
public:
    JoystickEventReceiver() {};
    virtual ~JoystickEventReceiver() {};

    /**
     * A joystick button has been pressed.
     */
    virtual void eventButton(js_event_t*) = 0;

    /**
     * The joystick has been moved.
     */
    virtual void eventAxis(js_event_t*) = 0;
};

#endif
