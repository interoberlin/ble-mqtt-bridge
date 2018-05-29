
#include <iostream>
#include <exception>

#include "endpoints/BLEClient.hpp"

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>


BLEClient::BLEClient(
            BLEClientRole::role_enum role,
            string address,
            string service,
            string characteristic
            )
    :EventGenerator(),
     EventReceiver()
{
    this->role = role;
    device_address = address;
    uuid_service = service;
    uuid_characteristic = characteristic;

    manager = BluetoothManager::get_bluetooth_manager();

    terminate_connection_thread = false;
    connection_thread_id = 0;
    connection_thread_running = false;
    startConnectionThread();

    terminate_reader_thread = false;
    reader_thread_running = false;
    reader_thread_id = 0;
    read_interval = 1s;

    // Only in the reader role there is need for a reader thread
    if (role == BLEClientRole::READER)
    {
        // Delay this thread against connections thread intervals
        this_thread::sleep_for(100ms);
        startReaderThread();
    }
}


BLEClient::~BLEClient()
{
    stopReaderThread();
    stopConnectionThread();
}


static void* bleclient_connection_thread(void* argv)
{
    // Get this thread's owner object
    BLEClient* ble_client = (BLEClient*) argv;

    bool was_previously_connected = false;

    ble_client->setThreadName("ble-conn-mgr");

    // Run until termination signal received
    while (!ble_client->getConnectionThreadTerminationRequested())
    {
        if (ble_client->device == NULL)
        {
            ble_client->service = NULL;
            ble_client->characteristic = NULL;

            if (!ble_client->manager->get_discovering())
            {
                LOG_S(INFO) << "Starting to scan for BLE devices...";
                
                try {
                
                    ble_client->manager->start_discovery();
                
                // TODO Wie kann man eine tinyb::BluetoothException richtig ansprechen?
                } catch ( ... ) {
                    int delay = ble_client->getDiscoveryDelay();
                    delay = delay * 2;
                    if (delay > 512) {
                        delay = 512;
                    }
                    ble_client->setDiscoveryDelay(delay);
                    LOG_S(WARNING) << "tmout: BLE scan (" << ble_client->getDiscoveryDelay() << "s) " << ble_client->device_address;
                    this_thread::sleep_for(std::chrono::seconds(ble_client->getDiscoveryDelay()));
                    continue;
                }
            }
            else
            {
                // Evaluate list of known devices
                auto devices = ble_client->manager->get_devices();
                for (auto it = devices.begin(); it != devices.end(); ++it)
                {
                    if ((*it)->get_address() == ble_client->device_address)
                    {
                        LOG_S(INFO) << "Found beacon "<< ble_client->device_address << " in list of known devices";
                        ble_client->device = (*it).release();
                        break;
                    }
                }
            }
        }

        if (ble_client->device != NULL)
        {
            if (!ble_client->device->get_connected())
            {
                was_previously_connected = false;
                LOG_S(INFO) << "Connecting... " << ble_client->device_address;

                try {
                    ble_client->manager->stop_discovery();
                    ble_client->device->disconnect();
                    ble_client->device->connect();
                } catch ( ... ) {
                    int delay = ble_client->getConnectionDelay();
                    delay = delay * 2;
                    if (delay > 512) {
                        delay = 512;
                    }
                    ble_client->setConnectionDelay(delay);
                    LOG_S(WARNING) << "tmout: BLE conn (" << ble_client->getConnectionDelay() << "s) " << ble_client->device_address;
                    this_thread::sleep_for(std::chrono::seconds(ble_client->getConnectionDelay()));
                    continue; 
                }
            }
            else
            {
                if (was_previously_connected)
                {
                    LOG_S(9) << "Still connected to beacon " << ble_client->device_address;
                }
                else
                {
                    LOG_S(INFO) << "Connected to beacon with address " << ble_client->device_address;
                    was_previously_connected = true;
                }

                if (ble_client->service == NULL)
                {
                    if (ble_client->device->get_services_resolved())
                    {
                        auto services = ble_client->device->get_services();
                        if (!services.empty())
                        {
                            // Search for service
                            for (auto it = services.begin(); it != services.end(); ++it)
                            {
                                if ((*it)->get_uuid() == ble_client->uuid_service)
                                {
                                    LOG_S(INFO) << "Found desired BLE service";
                                    
                                    ble_client->service = (*it).release();
                                    break;
                                }
                            }
                        }
                        // else wait until services are resolved
                    }
                    // else wait until services are resolved
                }

                if (ble_client->service != NULL)
                {
                    if (ble_client->characteristic == NULL)
                    {
                        // Search for characteristic
                        auto characteristics = ble_client->service->get_characteristics();
                        for (auto it = characteristics.begin(); it != characteristics.end(); ++it)
                        {
                            if ((*it)->get_uuid() == ble_client->uuid_characteristic)
                            {
                                LOG_S(INFO) << "Found desired BLE characteristic";

                                ble_client->characteristic = (*it).release();
                                break;
                            }
                        }
                    }
                }
            }
        }

        this_thread::sleep_for(5s);
    }

    LOG_S(ERROR) << "BLE connection thread terminated.";

    // Thread terminates
    return NULL;
}

void BLEClient::startConnectionThread()
{
    if (connection_thread_running)
        return;
    
    terminate_connection_thread = false;
    int rc = pthread_create(&connection_thread_id, NULL, &bleclient_connection_thread, this);
    if (rc != 0)
    {
        LOG_S(ERROR) << "!! BLE: Error: Unable to start connection thread.";
        return;
    }

    connection_thread_running = true;
}


void BLEClient::stopConnectionThread()
{
    if (!connection_thread_running)
        return;

    // Signal thread to terminate
    terminate_connection_thread = true;

    // Wait for thread to actually terminate
    pthread_join(connection_thread_id, NULL);

    connection_thread_running = false;
}


void BLEClient::write(vector<uint8_t>& value)
{
    if (characteristic == NULL)
    {
        LOG_S(ERROR) << "BLE: Unable to write: Characteristic not resolved.";
        return;
    }

    try
    {
        characteristic->write_value(value);
    }
    catch (...)
    {
    }
}


/**
 * Independent thread to iteratively read data from a device/characteristic
 * in configurable intervals, until thread termination is requested
 */
static void* bleclient_reader_thread(void* argv)
{
    // Get this thread's owner object
    BLEClient* ble_client = (BLEClient*) argv;

    // set the name of the tread 
    string devAddr = string(ble_client->device_address);
    // process/thread names are limited to 16 chars
    string threadName = string("ble:") + devAddr.substr(devAddr.length()-11,devAddr.length());
    ble_client->setThreadName(threadName);
    

    // Run until termination signal received
    while (!ble_client->getReaderThreadTerminationRequested())
    {
        if (ble_client->characteristic != NULL)
        {
            LOG_S(9) << "Reading data from BLE beacon " << ble_client->device_address;

            vector<uint8_t> data = ble_client->characteristic->read_value(0);

            // If no one is listening, simply discard the data
            if (!ble_client->hasEventReceiver())
                continue;

            // Create an event!
            event_t e;
            e.source = EventSource::BLE;
            e.evt.ble_raw.bleDataLength = min(data.size(), sizeof(e.evt.ble_raw.bleData));
            for (uint8_t i=0; i<e.evt.ble_raw.bleDataLength; i++)
            {
                e.evt.ble_raw.bleData[i] = data[i];
            }
            ble_client->getEventReceiver()->event(&e);
        }

        this_thread::sleep_for(ble_client->getReadInterval());
    }

    LOG_S(ERROR) << "BLE reader thread terminated.";

    // Thread terminates
    return NULL;
}


void BLEClient::startReaderThread()
{
    if (reader_thread_running)
    {
        LOG_S(ERROR) << "!! Reader thread already running with ID " <<  reader_thread_id;
        return;
    }

    if (role != BLEClientRole::READER)
    {
        LOG_S(ERROR) << "!! Won't start reader thread: This object's role is not READER.";
        return;
    }

    terminate_reader_thread = false;
    int rc = pthread_create(&reader_thread_id, NULL, &bleclient_reader_thread, this);
    if (rc != 0)
    {
        LOG_S(ERROR) << "Error: Unable to start reader thread";
        return;
    }

    reader_thread_running = true;
}


void BLEClient::stopReaderThread()
{
    if (!reader_thread_running)
        return;

    // Signal reader thread to terminate
    terminate_reader_thread = true;

    // Wait for thread to actually terminate
    pthread_join(reader_thread_id, NULL);
}


void BLEClient::event(event_t* e)
{

}

void BLEClient::setThreadName(std::string name /* = ble: NN */)
{
    string threadName =  name;
    
    loguru::set_thread_name(threadName.c_str());
    int rc = pthread_setname_np(pthread_self(), threadName.c_str());
    if (rc) {
        LOG_S(ERROR) << "pthread_setname_np returned " << rc;
    }
}
