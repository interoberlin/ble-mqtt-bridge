
#include <iostream>
#include "Steering.hpp"


void Steering::eventButton(js_event_t* e)
{
    if (mqtt == NULL)
        return;

    string s = "Button";

    switch (e->number)
    {
        case 4:
            // F1
            mqtt->sendMessage((char*) "1", 1,  (char*) "alarmlight/switch");
            break;

        case 5:
            // F2
            mqtt->sendMessage((char*) "0", 1,  (char*) "alarmlight/switch");
            break; 
    }
}


void Steering::eventAxis(js_event_t* e)
{
    // Without an MQTT sink, we don't need to continue
    if (mqtt == NULL)
        return;

    /*
     * Evaluate joystick position and set steering state accordingly
     */
    if (e->number == 1)
    {
        if (e->value < -5000)
        {
            state = Steer::FORWARD;
        }
        else if (e->value > 5000)
        {
            state = Steer::BACKWARD;
        }
        else
        {
            if (state == Steer::FORWARD || state == Steer::BACKWARD)
            {
                state = Steer::STOP;
            }
        }
    }
    else if (e->number == 0)
    {
        if (e->value > 5000)
        {
            state = Steer::RIGHT;
        }
        else if (e->value < -5000)
        {
            state = Steer::LEFT;
        }
        else
        {
            if (state == Steer::RIGHT || state == Steer::LEFT)
            {
                state = Steer::STOP;
            }
        }
    }
    else if (e->number == 2)
    {
        if (e->value < -5000)
        {
            state = Steer::ROTATE_LEFT;
        }
        else if (e->value > 5000)
        {
            state = Steer::ROTATE_RIGHT;
        }
        else
        {
            if (state == Steer::ROTATE_LEFT || state == Steer::ROTATE_RIGHT)
            {
                state = Steer::STOP;
            }
        }
    }

}


void Steering::process()
{
    // Without an MQTT sink, we don't need to continue
    if (mqtt == NULL)
        return;

    // A dummy message to send to the broker
    string message = "1";

    switch (state)
    {
        case Steer::STOP:
            mqtt->sendMessage(message, Steer::Subtopic::Stop);
            break;

        case Steer::FORWARD:
            mqtt->sendMessage(message, Steer::Subtopic::Forward);
            break;

        case Steer::BACKWARD:
            mqtt->sendMessage(message, Steer::Subtopic::Backward);
            break;

        case Steer::LEFT:
            mqtt->sendMessage(message, Steer::Subtopic::Left);
            break;

        case Steer::RIGHT:
            mqtt->sendMessage(message, Steer::Subtopic::Right);
            break;

        case Steer::ROTATE_LEFT:
            mqtt->sendMessage(message, Steer::Subtopic::RotateLeft);
            break;

        case Steer::ROTATE_RIGHT:
            mqtt->sendMessage(message, Steer::Subtopic::RotateRight);
            break;

        default:
            cerr << "Encountered invalid state" << endl;
            break;
    }
}
