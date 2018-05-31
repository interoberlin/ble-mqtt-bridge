
#ifndef BLECLIENT_HPP
#define BLECLIENT_HPP


#include <stdint.h>
#include <thread>
#include <pthread.h>
#include <tinyb.hpp>
#include "events/EventGenerator.hpp"
#include "events/EventReceiver.hpp"


using namespace std;
using namespace std::chrono_literals;
using namespace tinyb;


namespace BLEClientRole
{
    /**
     * A BLEClient object's role:
     *   Iteratively read data from a device/characteristic via BLE
     *   Write data to a device/characteristic via BLE upon request
     *   Subscribe to data incoming from a device/characteristic via BLE
     */
    typedef enum
    {
        READER,
        WRITER,
        SUBSCRIBER,
    } role_enum;
}


class BLEClient:
        virtual public EventGenerator,
        virtual public EventReceiver
{
public:
    /*
     * This object's BLE role (reader and/or writer) and the
     * target BLE device's address and characteristic configuration
     */
    BLEClientRole::role_enum role;
    string device_address;
    string uuid_service;
    string uuid_characteristic;

    /*
     * Pointers to objects handed over to us from bluez by tinyB
     */
    BluetoothManager *manager = NULL;
    BluetoothDevice* device = NULL;
    BluetoothGattService* service = NULL;
    BluetoothGattCharacteristic* characteristic = NULL;

private:
    /*
     * Bluetooth connection management is done in a separate thread
     */
    pthread_t connection_thread_id;
    bool connection_thread_running;
    bool terminate_connection_thread;

    /*
     * Data incoming via BLE is processed in an independent thread
     */
    chrono::milliseconds read_interval = 500ms;
    pthread_t reader_thread_id;
    bool reader_thread_running;
    bool terminate_reader_thread;

private:
    chrono::seconds discovery_delay = 1s;

public:
    chrono::seconds getDiscoveryDelay() {return discovery_delay; };
    void setDiscoveryDelay(chrono::seconds delay) { discovery_delay = delay; }

private:
    chrono::seconds connection_delay = 1s;

public:
    chrono::seconds getConnectionDelay() {return connection_delay; };
    void setConnectionDelay(chrono::seconds delay) { connection_delay = delay; }

public:
    BLEClient(
            BLEClientRole::role_enum role,
            string address,
            string service,
            string characteristic
            );
    ~BLEClient();

    void startConnectionThread();
    void stopConnectionThread();
    bool getConnectionThreadTerminationRequested() { return terminate_connection_thread; }

    void write(vector<uint8_t>&);

    void startReaderThread();
    void stopReaderThread();
    bool getReaderThreadTerminationRequested() { return terminate_reader_thread; }

    void setReadInterval(chrono::milliseconds milli) { read_interval = milli; }
    chrono::milliseconds getReadInterval() { return read_interval; }

    // set the Name of the thread
    void setThreadName(std::string name = "ble: NN");


    void event(event_t* e);
};


#endif
