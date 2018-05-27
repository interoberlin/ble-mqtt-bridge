
#include <Bridge.hpp>


Bridge::Bridge(
            const char* mqttClientName,
            const char* mqttTopic,
            const char* mqttHost,
            int mqttPort,
            BLEClientRole::role_enum bleClientRole,
            string bleAddress,
            string bleService,
            string bleCharacteristic
            )
{
    mqtt_client = new MQTTClientRobby(
                            mqttClientName,
                            mqttTopic,
                            mqttHost,
                            mqttPort
                            );

    ble_client = new BLEClient(
                            bleClientRole,
                            bleAddress,
                            bleService,
                            bleCharacteristic
                            );

    if ((bleClientRole == BLEClientRole::READER)
     || (bleClientRole == BLEClientRole::SUBSCRIBER))
    {
        ble_client->registerReadEventReceiver(mqtt_client);
    }

    if (bleClientRole == BLEClientRole::WRITER)
    {
        mqtt_client->registerOnMessageEventReceiver(ble_client);
    }
}


Bridge::~Bridge()
{
    delete ble_client;
    delete mqtt_client;
}
