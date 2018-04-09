
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * A struct to receive an event data into.
 */
struct js_event {
        unsigned int time;      /* event timestamp in milliseconds */
        short value;   /* value */
        unsigned char type;     /* event type */
        unsigned char number;   /* axis/button number */
};

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

// Joystick produces 2 types of events (struct js_event.type) viz, button events and axis events.

int main()
{
    int fd = open ("/dev/input/js0", O_RDONLY);
    if( fd < 0 )
        printf( "cannot open dev\n" );
    else
        printf( "opened success...:)\n" );

    struct js_event e;
    while( 1 ) //event loop
    {
        read( fd, &e, sizeof(e) );
        printf( "%d %d %d %d\n", e.time, e.value, e.type, e.number );

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
    return 0;
}
