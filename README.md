# ble-mqtt-bridge
Daemon which facilitates Bluetooth Low Energy to MQTT interconnection

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

You need a json-library, we used nlohmann/json, installed in /usr/local

```shell
$ git clone https://github.com/nlohmann/json.git
....
$ cd json
$ mkdir build
$ cd build
$ cmake ..
....
$ make 
....
$ make test
....
$ sudo make install
...
```

### mosquitto MQTT 

You need mosquitto running on port 1833.

You can install it on ubuntu/debian with

```shell
$ apt install mosquitto mosquitto-clients libmosquittopp-dev
```

or with a docker image

```shell
$ apt install mosquitto-clients libmosquittopp-dev

$ docker run -it -p 1883:1883 -p 9001:9001  --name mosquitto eclipse-mosquitto 
``` 



