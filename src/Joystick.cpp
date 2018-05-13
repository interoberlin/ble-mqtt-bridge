
#include "Joystick.hpp"
#include <linux/joystick.h>
#include <stdio.h>


bool Joystick::open(const char* filename)
{
    fd = ::open(filename, O_RDONLY);
    return (fd >= 0);
}


void Joystick::close()
{
    if (isOpen())
    {
        ::close(fd);
        fd = 0;
    }
}


void Joystick::read()
{
    js_event_t e;
    ::read(fd, &e, sizeof(e));
    printf("%d %d %d %d\n", e.time, e.value, e.type, e.number);

    if( e.type == JS_EVENT_BUTTON || e.type == JS_EVENT_AXIS )
    {
        if( e.type == JS_EVENT_BUTTON )
            printf( "button#%d value:%d\n", (int) e.number, e.value );
        else
            printf( "axis#%d value:%d\n", (int) e.number, e.value );
    }
    else
    {
//            printf( "Init Events\n" );
    }
}

