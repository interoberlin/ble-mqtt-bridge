
#include "Steering.hpp"


void Steering::eventButton(js_event_t* e)
{
    if (mqtt == NULL)
        return;

    string s = "Button";
    char* c = (char*) s.c_str();
    mqtt->sendMessage(c, s.length());
}


void Steering::eventAxis(js_event_t* e)
{
    if (mqtt == NULL)
        return;

    string s = "Axis";
    char* c = (char*) s.c_str();
    mqtt->sendMessage(c, s.length());
}
