
#include <BLEClient.hpp>
#include <MQTTClient.hpp>
#include <iostream>


BLEClient::BLEClient(
            BLEClientRole::role_enum role,
            string address,
            string service,
            string characteristic
            )
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
    mqtt_client = NULL;

    // Only in the reader role there is need for a reader thread
    if (role == BLEClientRole::READER)
    {
        // Delay this thread against connections thread intervals
        this_thread::sleep_for(500ms);
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

    // Run until termination signal received
    while (!ble_client->getConnectionThreadTerminationRequested())
    {
        if (ble_client->device == NULL)
        {
            ble_client->service = NULL;
            ble_client->characteristic = NULL;

            if (!ble_client->manager->get_discovering())
            {
                cout << ">> Starting to scan for BLE devices..." << endl << flush;
                ble_client->manager->start_discovery();
            }
            else
            {
                // Evaluate list of known devices
                auto devices = ble_client->manager->get_devices();
                for (auto it = devices.begin(); it != devices.end(); ++it)
                {
                    if ((*it)->get_address() == ble_client->device_address)
                    {
                        cout << ">> Found requested beacon in list of known devices" << endl << flush;
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
                cout << ">> Connecting..." << endl << flush;

                ble_client->manager->stop_discovery();
                ble_client->device->disconnect();
                ble_client->device->connect();
            }
            else
            {
                if (was_previously_connected)
                {
                    cout << ">> Still connected to beacon." << endl << flush;
                }
                else
                {
                    cout << ">> Connected to beacon with address " << ble_client->device_address << endl << flush;
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
                                    cout << ">> Found desired BLE service" << endl;
                                    fflush(stdout);
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
                                cout << ">> Found desired BLE characteristic" << endl;
                                fflush(stdout);
                                ble_client->characteristic = (*it).release();
                                break;
                            }
                        }
                    }
                }
            }
        }

        this_thread::sleep_for(1s);
    }

    cout << ">> BLE connection thread terminated." << endl << flush;

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
        printf("Error: Unable to start connection thread.\n");
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
}


void BLEClient::write(vector<uint8_t>& value)
{
    if (characteristic == NULL)
    {
        cerr << ">> BLE: Unable to write: Characteristic not resolved." << endl;
        return;
    }

    characteristic->write_value(value);
}


/**
 * Independent thread to iteratively read data from a device/characteristic
 * in configurable intervals, until thread termination is requested
 */
static void* bleclient_reader_thread(void* argv)
{
    // Get this thread's owner object
    BLEClient* ble_client = (BLEClient*) argv;

    // Run until termination signal received
    while (!ble_client->getReaderThreadTerminationRequested())
    {
        if (ble_client->characteristic != NULL)
        {
            cout << ">> Reading data from BLE beacon..." << endl << flush;

            vector<uint8_t> data = ble_client->characteristic->read_value(0);

//            cout << data << endl;

            MQTTClient* mqtt_client = ble_client->getReadEventReceiver();
            if (mqtt_client != NULL)
            {
                uint8_t length = data.size();
                char s[length];
//                s =
//                data.
                mqtt_client->send_message(s, length);
            }
        }

        this_thread::sleep_for(ble_client->getReadInterval());
    }

    cout << ">> BLE reader thread terminated." << endl << flush;

    // Thread terminates
    return NULL;
}


void BLEClient::startReaderThread()
{
    if (reader_thread_running)
    {
        printf("Reader thread already running with ID %d.\n", (int) reader_thread_id);
        return;
    }

    if (role != BLEClientRole::READER)
    {
        printf("Won't start reader thread: This object's role is not READER.");
        return;
    }

    terminate_reader_thread = false;
    int rc = pthread_create(&reader_thread_id, NULL, &bleclient_reader_thread, this);
    if (rc != 0)
    {
        printf("Error: Unable to start reader thread.\n");
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

