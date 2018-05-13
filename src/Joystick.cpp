
#include "Joystick.hpp"
#include <stdio.h>


Joystick::Joystick()
    // Initialize event system
   :JoystickEventGenerator()
{
    fd = -1;
}


Joystick::Joystick(const char* filename)
    // Call the above default constructor
   :Joystick()
{
    open(filename);
}


Joystick::~Joystick()
{
    // Close device file, if open
    close();
}


bool Joystick::open(const char* filename)
{
    // Open device file for reading
    fd = ::open(filename, O_RDONLY);
    return (fd >= 0);
}


void Joystick::close()
{
    // Close device file, if open
    if (isOpen())
    {
        ::close(fd);
        fd = 0;
    }
}


void Joystick::read()
{
    // Attempt to read a joystick event from the device file
    js_event_t e;
    ::read(fd, &e, sizeof(e));
    printf("%d %d %d %d\n", e.time, e.value, e.type, e.number);

    // Without event receiver, event evaluation can be skipped.
    if (!hasEventReceiver())
        return;

    switch (e.type)
    {
        case JS_EVENT_BUTTON:
            printf("button#%d value:%d\n", (int) e.number, e.value);
            eventReceiver->eventButton(&e);
            break;
        case JS_EVENT_AXIS:
            printf("axis#%d value:%d\n", (int) e.number, e.value);
            eventReceiver->eventAxis(&e);
            break;
        default:
//            printf( "Init Events\n" );
            break;
    }
}

