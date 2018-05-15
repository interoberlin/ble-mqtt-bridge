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

        void event(event_t* e)
        {
            // Only accept events originating from BLE
            if (e->source != EventSource::BLE)
                return;

            // Input from a floorsensor beacon is expected to be 30 bytes in length
            if (e->bleDataLength != 30)
                return;

            // Stop processing, if no one is listening anyway
            if (!this->hasEventReceiver())
                return;

            for (uint8_t i=0; i<15; i++)
            {
                if (sensorEnabled[i])
                {
                    // Extract 16-bit word
                    uint16_t sensorValue = (e->bleData[i*2] << 8) | e->bleData[(i*2)+1];

                    // Pass an event to the next one in the event chain
                    event_t e;
                    e.source = EventSource::SPLITTER;
                    e.checkerboardId = checkerboardId[i];
                    e.sensorValue = sensorValue;
                    getEventReceiver()->event(&e);
                }
            }
        }
    };
}

#endif
