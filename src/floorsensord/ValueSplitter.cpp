
#include "floorsensord/ValueSplitter.hpp"

using namespace floorsensord;


void ValueSplitter::event(event_t* e)
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

