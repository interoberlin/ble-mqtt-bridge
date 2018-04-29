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
 export LDFLAGS += -L/usr/local/lib
```
to your ~/.bashrc.
