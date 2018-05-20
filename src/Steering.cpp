
#include <iostream>
#include "Steering.hpp"


void Steering::eventButton(js_event_t* e)
{
    if (mqtt == NULL)
        return;

    string s = "Button";

    mqtt->sendMessage(s);
}


void Steering::eventAxis(js_event_t* e)
{
    if (mqtt == NULL)
        return;

    if ((lasttime+200) > e->time) {
        // cerr << lasttime << "/" << e->time << " " << endl;
        return;
    }
    
    string message;
    string subtopic;
    subtopic = to_string(e->number);
    message = to_string(e->value);
    
    
    mqtt->sendMessage(message, subtopic);

    lasttime = e->time;
}
