
#include <iostream>
#include <stddef.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <tinyb.hpp>
#include <MQTTClient.hpp>


using namespace std;
using namespace std::chrono_literals;
using namespace tinyb;

#define BEACON_ADDRESS                  "87:10:DC:E9:6D:50"
#define UUID_SERVICE_ALARMLIGHT         "00004000-0000-1000-8000-00805f9b34fb"
#define UUID_CHARACTERISTIC_ALARMLIGHT  "00004001-0000-1000-8000-00805f9b34fb"


int main()
{
    MQTTClient* mqtt_client = new MQTTClient(
                                    "alarmlight-daemon",
                                    "alarmlight",
                                    "localhost",
                                    8883
                                    );

    this_thread::sleep_for(2s);

    bool ble_connected = false;
    mqtt_client->publish(NULL, "alarmlight/connected", 1, &ble_connected, 0, true);


    BluetoothManager *manager;
    manager = BluetoothManager::get_bluetooth_manager();

    printf("Starting device discovery");
    fflush(stdout);
    manager->start_discovery();

    BluetoothDevice* device = NULL;
    for (uint8_t i=0; i<10; i++)
    {
        // Evaluate list of known devices
        auto devices = manager->get_devices();
        for (auto it = devices.begin(); it != devices.end(); ++it)
        {
            if ((*it)->get_address() == BEACON_ADDRESS)
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

    printf("Bluetooth daemon is aware of a device with address %s.\n", BEACON_ADDRESS);
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

    BluetoothGattService* service = NULL;
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
            if ((*it)->get_uuid() == UUID_SERVICE_ALARMLIGHT)
            {
                service = (*it).release();
                break;
            }
        }
        printf("Service with UUID %s not found.\n", UUID_SERVICE_ALARMLIGHT);
        exit(3);
    }

    printf("Alarmlight service found.\n");

    BluetoothGattCharacteristic* characteristic = NULL;
    auto characteristics = service->get_characteristics();
    for (auto it = characteristics.begin(); it != characteristics.end(); ++it)
    {
        if ((*it)->get_uuid() == UUID_CHARACTERISTIC_ALARMLIGHT)
        {
            characteristic = (*it).release();
            break;
        }
    }
    if (characteristic == NULL)
    {
        printf("Characteristic with UUID %s not found.\n", UUID_CHARACTERISTIC_ALARMLIGHT);
        exit(4);
    }

    printf ("Alarmlight characteristic found.\n");

    ble_connected = true;
    mqtt_client->publish(NULL, "alarmlight/connected", 1, &ble_connected, 0, true);

    mqtt_client->subscribe(NULL, "alarmlight/switch", 0);

    vector<uint8_t> value;
    while (true)
    {
        value = {1};
        characteristic->write_value(value);
        printf("Value written.\n");
        this_thread::sleep_for(1s);

        value = {0};
        characteristic->write_value(value);
        printf("Value written.\n");
        this_thread::sleep_for(1s);
    }
}
