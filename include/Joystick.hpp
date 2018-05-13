/**
 * @file
 * Joystick class
 *
 * See also:
 * https://www.kernel.org/doc/Documentation/input/joystick-api.txt
 */

#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "EventGenerator.hpp"

using namespace std;


class Joystick:
        virtual public JoystickEventGenerator
{
private:
    /** File descriptor to joystick device file */
    int fd;

public:
    Joystick();
    Joystick(const char* filename);
    ~Joystick();

    /**
     * Open specified joystick device file
     */
    bool open(const char* filename);

    /**
     * Returns whether the joystick device file is openend or not
     */
    bool isOpen() { return (fd > -1); }

    /**
     * Close joystick device file
     */
    void close();

    /**
     * Attempt to read an event from joystick device
     */
    void read();
};

#endif
