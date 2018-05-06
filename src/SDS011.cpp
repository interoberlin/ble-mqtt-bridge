
#include "SDS011.hpp"
#include <iostream>

using namespace std;


static const uint8_t SLEEPCMD[19] =
{
    0xAA,	// head
    0xB4,	// command id
    0x06,	// data byte 1
    0x01,	// data byte 2 (set mode)
    0x00,	// data byte 3 (sleep)
    0x00,	// data byte 4
    0x00,	// data byte 5
    0x00,	// data byte 6
    0x00,	// data byte 7
    0x00,	// data byte 8
    0x00,	// data byte 9
    0x00,	// data byte 10
    0x00,	// data byte 11
    0x00,	// data byte 12
    0x00,	// data byte 13
    0xFF,	// data byte 14 (device id byte 1)
    0xFF,	// data byte 15 (device id byte 2)
    0x05,	// checksum
    0xAB	// tail
};


SDS011::SDS011(string device_path)
{
    // Free previously created port objects
    if (port != NULL)
        delete port;

    port = new SerialPort();
    port->Open(device_path.c_str(), 9600);
}


SDS011::~SDS011()
{
    port->Close();
    delete port;
}


void SDS011::readSerialPort()
{
    // Read bytes from serial port
    uint8_t buf[42];
    int count = port->Read(buf, sizeof(buf));

    // Push them to our local queue
    for (uint8_t i=0; i<count; i++)
    {
        buffer.push(buf[i]);
    }
}


bool SDS011::read(float *p25, float *p10, uint16_t* id)
{
    readSerialPort();
    if (buffer.empty())
    {
        // Not enough data received (yet)
        printf("Not enough data received yet.\n");
        return false;
    }

    int pm10_raw = 0;
    int pm25_raw = 0;
    int checksum_reference;
    bool checksum_ok = false;
    int state = 0;
    uint16_t _id = 0;

    while (!buffer.empty())
    {
        // Grab one byte from the buffer
        uint8_t value = buffer.front();
        buffer.pop();

        // Main parser state machine
        switch (state)
        {
            case 0:
                if (value != 170)   // 0xAA
                {
                    state = -1;
                }
                break;
            case 1:
                if (value != 192)   // 0xC0
                {
                    state = -1;
                }
                break;
            case 2:
                pm25_raw = value;
                checksum_reference = value;
                break;
            case 3:
                pm25_raw += (value << 8);
                checksum_reference += value;
                break;
            case 4:
                pm10_raw = value;
                checksum_reference += value;
                break;
            case 5:
                pm10_raw += (value << 8);
                checksum_reference += value;
                break;
            case 6:
                _id = (value << 8);
                checksum_reference += value;
                break;
            case 7:
                _id += value;
                checksum_reference += value;
                break;
            case 8:
                if (value == (checksum_reference % 256))
                {
                    checksum_ok = true;
                }
                else
                {
                    state = -1;
                    cerr << "Checksum error" << endl;
                }
                break;
            case 9:
                if (value != 171)   // 0xAB
                {
                    state = -1;
                }
                else
                {
                    // Transaction complete
                    if (!checksum_ok)
                        // Checksum error
                        return false;

                    // Calculate real sensor values
                    if (p10 != NULL)
                        *p10 = (float) pm10_raw / 10.0;
                    if (p25 != NULL)
                        *p25 = (float) pm25_raw / 10.0;
                    if (id != NULL)
                        *id = _id;

                    // Success!
                    return true;
                }
                break;
        }
        state++;
    }

    // If we arrive here, parsing has failed.
    return false;
}


void SDS011::sleep()
{
    for (uint8_t i = 0; i < 19; i++)
    {
        port->Write(SLEEPCMD[i]);
    }
    port->Flush();
}


void SDS011::wakeup()
{
    port->Write(0x01);
    port->Flush();
}

