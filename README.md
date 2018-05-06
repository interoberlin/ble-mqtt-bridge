# SDS011  
  
Arduino library for dust Sensor SDS011 (Nova Fitness Co.,Ltd)  
This library uses SoftSerial or HardwareSerial to connect to the SDS011.  
  
## Usage
  
* Define SDS object:  
`SDS011(uint8_t pin_rx, uint8_t pin_tx);`  
`i.e. SDS011 mySDS(pinRX,pinTX);`  
  
* Start object:  
`void begin(void);`  
i.e. `mySDS.begin();`  
  
* Read values:  
`int read(float *p25, float *p10);`  
i.e. `error = mySDS(pm25,pm10);`  
  
Reads the PM2.5 and PM10 values, return code is 0, if new values were read, and 1 if there were no new values.  

### Alternative with HardwareSerial
* SDS object can also be initialized with a Serial object as parameter  
`SDS011(Stream serial);`  
`i.e. SDS011 mySDS(Serial1);` 
  
  
