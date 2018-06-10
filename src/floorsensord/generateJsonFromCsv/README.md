# generateJsonFromCsv

This program read the [CSV file](etc/BaumhausSensorDocumentation.csv) saved from the central repository [spreadsheet](etc/BaumhausSensorDocumentation.ods) and dumps the JSON to `stdout`.

```
$ bin/generateJsonFromCsv | mosquitto_pub  -t "floorsensor/config" -l
``` 