
#include <stdint.h>
#include <string>
#include <iostream>
#include "Joystick.hpp"
#include <linux/joystick.h>

using namespace std;

string devicePath = "/dev/input/js0";


int main()
{
    Joystick* joystick = new Joystick();
    if (!joystick->open(devicePath.c_str()))
    {
        cerr << "Fatal: Failed to open joystick device file." << endl;
        exit(1);
    }

    // Infinite event loop
    while (1)
    {
        joystick->read();
    }

    delete joystick;
    return 0;
}
