
#include <BLEClient.hpp>
#include <MQTTClient.hpp>


BLEClient::BLEClient(string address, string service, string characteristic)
{
    manager = BluetoothManager::get_bluetooth_manager();
    device_address = address;
    uuid_service = service;
    uuid_characteristic = characteristic;
}


BLEClient::~BLEClient()
{}


void BLEClient::process()
{
    printf("Starting device discovery");
    fflush(stdout);
    manager->start_discovery();

    for (uint8_t i=0; i<10; i++)
    {
        // Evaluate list of known devices
        auto devices = manager->get_devices();
        for (auto it = devices.begin(); it != devices.end(); ++it)
        {
            if ((*it)->get_address() == device_address)
            {
                device = (*it).release();
                break;
            }
        }
        if (device != NULL)
        {
            break;
        }

        // Wait for 1 seconds
        this_thread::sleep_for(1s);
        printf(".");
        fflush(stdout);
    }
    manager->stop_discovery();
    printf("\n");
    if (device == NULL)
    {
        // Timeout waiting for device
        exit(1);
    }

    printf("Bluetooth daemon is aware of a device with address %s.\n", device_address.c_str());
    printf("Attempting to connect");
    fflush(stdout);

    device->disconnect();
    device->connect();
    for (uint8_t i=0; i<4; i++)
    {
        // Is the device connected?
        if (device->get_connected())
        {
            break;
        }

        // Wait for 1 second
        this_thread::sleep_for(1s);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
    if (!device->get_connected())
    {
        // Timeout waiting for connection
        exit(2);
    }

    printf("Device is connected.\n");
    printf("Discovering services...\n");
    fflush(stdout);

    for (uint8_t i=0; i<5; i++)
    {
        printf(".");
        fflush(stdout);

        if (!device->get_services_resolved())
        {
            continue;
        }

        auto services = device->get_services();
        if (services.empty())
        {
            continue;
        }

        for (auto it = services.begin(); it != services.end(); ++it)
        {
            if ((*it)->get_uuid() == uuid_service)
            {
                service = (*it).release();
                break;
            }
        }
        printf("Service with UUID %s not found.\n", uuid_service.c_str());
        exit(3);
    }

    printf("Alarmlight service found.\n");

    auto characteristics = service->get_characteristics();
    for (auto it = characteristics.begin(); it != characteristics.end(); ++it)
    {
        if ((*it)->get_uuid() == uuid_characteristic)
        {
            characteristic = (*it).release();
            break;
        }
    }
    if (characteristic == NULL)
    {
        printf("Characteristic with UUID %s not found.\n", uuid_characteristic.c_str());
        exit(4);
    }

    printf ("Alarmlight characteristic found.\n");
}


void BLEClient::write(vector<uint8_t>& value)
{
    if (characteristic == NULL)
        return;

    characteristic->write_value(value);
}
