/**
 * Read values from the SDS011 dust
 * sensors PM2.5 and PM10
 * ---------------------------------
 *
 * Based on the work of R. Zschiegner (rz@madavi.de)
 *
 * Documentation:
 *        - The iNovaFitness SDS011 datasheet
 *          https://nettigo.pl/attachments/398
 */

#ifndef SDS011_HPP
#define SDS011_HPP

#include <SerialPort.hpp>
#include <stdint.h>
#include <stdbool.h>
#include <queue>
#include <string>

using namespace std;


class SDS011
{
    private:
        SerialPort* port = NULL;
        queue<char> buffer;

    public:
        SDS011(string device_path);
        ~SDS011();

        /**
         * Read all bytes from the serial port
         * to our local input buffer
         */
        void readSerialPort();

        /**
         * Attempt to parse a sensor value
         * from the serial port buffer
         *
         * @retval true:  Successfully parsed values
         * @retval false: Data parsing failed
         */
        bool read(float *p25, float *p10, uint16_t* id);

        void sleep();
        void wakeup();
};

#endif
