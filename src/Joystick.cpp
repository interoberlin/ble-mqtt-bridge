
// #include <stdio.h>
#include <iostream>
#include "Joystick.hpp"


Joystick::Joystick()
    // Initialize event system
   :JoystickEventGenerator()
{
    fd = -1;

    reader_thread_id = 0;
    reader_thread_running = false;
    terminate_reader_thread = false;
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
    if (fd >= 0)
    {
        startReaderThread();
        return true;
    }
    return false;
}


void Joystick::close()
{
    // Stop reading in the other thread
    stopReaderThread();

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
    // printf("%d %d %d %d\n", e.time, e.value, e.type, e.number);

    // Without event receiver, event evaluation can be skipped.
    if (!hasEventReceiver())
        return;

    switch (e.type)
    {
        case JS_EVENT_BUTTON:
            // printf("button#%d value:%d\n", (int) e.number, e.value);
            eventReceiver->eventButton(&e);
            break;
        case JS_EVENT_AXIS:
            // printf("axis#%d value:%d\n", (int) e.number, e.value);
            eventReceiver->eventAxis(&e);
            break;
        default:
//            printf( "Init Events\n" );
            break;
    }

}


static void* joystick_reader_thread(void* argv)
{
    // Get this thread's owner object
    Joystick* joystick = (Joystick*) argv;

    while (!joystick->getReaderThreadTerminationRequested())
    {
        joystick->read();
    }

    return NULL;
}


void Joystick::startReaderThread()
{
    if (reader_thread_running)
    {
        cerr << "Reader thread already running with ID " << reader_thread_id << endl;
        return;
    }

    terminate_reader_thread = false;
    int rc = pthread_create(&reader_thread_id, NULL, &joystick_reader_thread, this);
    if (rc != 0)
    {
        cerr << "Error: Unable to start reader thread.\n";
        return;
    }

    reader_thread_running = true;
}


void Joystick::stopReaderThread()
{
    if (!reader_thread_running)
        return;

    // Signal reader thread to terminate
    terminate_reader_thread = true;

    // Wait for thread to actually terminate
    pthread_join(reader_thread_id, NULL);
}
