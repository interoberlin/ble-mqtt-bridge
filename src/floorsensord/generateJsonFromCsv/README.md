# generateJsonFromCsv

Reads CSV from `etc/BaumhausSensorDocumentation.csv` and write JSON to stdout. Could be used i.e. for 

```
$ bin/generateJsonFromCsv | mosquitto_pub  -t "floorsensor/config" -l
``` 