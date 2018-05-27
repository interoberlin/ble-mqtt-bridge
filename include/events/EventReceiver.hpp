/**
 * @file
 * Child classes can receive events
 */

#ifndef EVENTRECEIVER_HPP
#define EVENTRECEIVER_HPP

#include <string>
#include "Event.hpp"

using namespace std;


/**
 * Any class, which shall accept sensor events,
 * must be a child of this class and implement
 * the corresponding event methods.
 */
class EventReceiver
{
public:
    EventReceiver() {};
    virtual ~EventReceiver() {};

    /**
     * Notify the event receiver about an event
     */
    virtual void event(event_t*) = 0;
};

#endif
