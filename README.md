# ble-mqtt-bridge

The ble-mqtt-bridge facilitates the forwarding of values
between Bluetooth Low Energy (BLE) characteristics and MQTT messages
in both directions:

The bridge can automatically connect to a BLE device
and subscribe to or iteratively read (poll) a specific characteristic.
Whenever a new value is read, the bridge will forward it
to the MQTT broker with the corresponding topic.
The broker can then distribute the value to other interested parties on the network.

The bridge can also be configured,
to subscribe to MQTT messages with a configurable topic from a MQTT broker
and forward their content to a BLE characteristic.

## Requirements

### tinyb

If you compiled libtinyb.so from the
<a href="https://github.com/interoberlin/tinyb">sources</a>,
it will by default be located in /usr/local/lib,
therefore that path must be in your LDFLAGS.
You can add it to the Makefile
```Makefile
LDFLAGS += -L/usr/local/lib
```
or configure it globally e.g. by adding
```sh
 export LDFLAGS=${LDFLAGS}:-L/usr/local/lib
```
to your ~/.bashrc.

### nlohmann/json

The configuration format of the floorsensor daemon is JSON.
In order to compile floorsensord, you need <a href="https://github.com/nlohmann/json">nlohmann/json</a>.
It will be installed to /usr/local.

```shell
$ git clone https://github.com/nlohmann/json.git
...
$ cd json
$ mkdir build
$ cd build
$ cmake ..
...
$ make 
...
$ make test
...
$ sudo make install
...
```

### mosquitto 

In order to being able to publish messages,
you need to have a MQTT broker running somewhere,
in the simplest case on your local host.

We used <a href="https://mosquitto.org/">Eclipse's mosquitto daemon</a>, running on port 1883.

You can install it on ubuntu/debian with

```shell
$ apt install mosquitto mosquitto-clients libmosquittopp-dev
```

or with a docker image

```shell
$ apt install mosquitto-clients libmosquittopp-dev

$ docker run -it -p 1883:1883 -p 9001:9001  --name mosquitto eclipse-mosquitto 
``` 

The mosquitto.conf could for example look like this:
```
listener 1883 127.0.0.1
allow_anonymous true

pid_file /var/run/mosquitto.pid

persistence false

log_type information
log_type notice
log_type warning
log_type error

log_dest stdout
log_dest file /var/log/mosquitto/mosquitto.log
```
