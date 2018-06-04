
#ifndef STEERING_HPP
#define STEERING_HPP

#include "events/JoystickEventProcessor.hpp"

namespace Steer
{
    typedef enum
    {
        STOP,
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        ROTATE_LEFT,
        ROTATE_RIGHT
    } t;

    namespace Subtopic
    {
        const string Stop = "stop";
        const string Forward = "forward";
        const string Backward = "backward";
        const string Left = "left";
        const string Right = "right";
        const string RotateLeft = "rotateleft";
        const string RotateRight = "rotateright";
    }
}


class Steering:
        virtual public JoystickEventProcessor
{
private:
    /**
     * Timestamp of the last event
     */
    uint32_t time_last_publish = 0;
    Steer::t state = Steer::STOP;

public:
    /**
     * Handle a joystick button event
     */
    void eventButton(js_event_t*);

    /**
     * Handle a joystick movement event
     */
    void eventAxis(js_event_t*);

    /**
     * Evaluate current state and publish commands to MQTT accordingly
     */
    void process();
};

#endif
