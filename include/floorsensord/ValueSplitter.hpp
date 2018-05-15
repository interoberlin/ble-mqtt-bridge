/**
 * @file
 */

#ifndef VALUESPLITTER_HPP
#define VALUESPLITTER_HPP

#include <stdint.h>
#include <stdbool.h>
#include <string>
#include "events/Event.hpp"
#include "events/EventReceiver.hpp"
#include "events/EventGenerator.hpp"

using namespace std;


// The ValueSplitter class is intended only for the floorsensor daemon.
namespace floorsensord
{
    /**
     * Receives an event from a BLEClient,
     * splits the value into 15 or fewer 16-bit words
     * and publishes the values to an MQTT broker
     */
    class ValueSplitter:
            virtual public EventReceiver,
            virtual public EventGenerator
    {
    private:
        bool sensorEnabled[15] = {false};
        string checkerboardId[15] = {"xx"};

    public:
        void enableSensor(uint8_t index) { sensorEnabled[index] = true; }
        void disableSensor(uint8_t index) { sensorEnabled[index] = false; }

        void setCheckerboardId(uint8_t index, string id) { checkerboardId[index] = id; }

        void event(event_t* e);
    };
}

#endif
