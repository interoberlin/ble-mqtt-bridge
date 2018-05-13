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

using namespace std;

#ifndef js_event_t
typedef struct js_event js_event_t;
#endif


class Joystick
{
private:
    /** File descriptor to joystick device file */
    int fd = -1;

public:
    Joystick() { fd = -1; }
    Joystick(const char* filename) { open(filename); }
    ~Joystick() { close(); }

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
