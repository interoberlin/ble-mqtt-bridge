/**
 * @file
 * Child classes can generate events
 */

#ifndef EVENTGENERATOR_HPP
#define EVENTGENERATOR_HPP

#include <stddef.h>

#include "EventReceiver.hpp"


/**
 * Any class, which generates events,
 * must be a child of this class.
 */
class EventGenerator
{
protected:
    /** Receiver for events */
    EventReceiver* eventReceiver = NULL;

public:
    EventGenerator(): eventReceiver(NULL) {};
    virtual ~EventGenerator() {};

    /**
     * Determine, whether an event receiver has been registered
     */
    bool hasEventReceiver() { return (eventReceiver != NULL); }

    /**
     * Return pointer to event receiving object
     */
    EventReceiver* getEventReceiver() { return eventReceiver; }

    /**
     * Register an object to receive events
     */
    void registerEventReceiver(EventReceiver* e) { eventReceiver = e; }

    /**
     * Remove event receiver from this object
     */
    void releaseEventReceiver() { eventReceiver = NULL; }
};

#endif
