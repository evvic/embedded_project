# embedded_project


### 2 Way Serial Communication between ESP8266 and Arduino Mega

Using TX -> RX and RX -> TX pins to connect data transfer between the ESP8266 module and Arduino Mega.

(show connected pins here)<br/>
Arduino Mega    ESP8266 <br/>
pin   14 (TX3)   ->  14 (D5)<br/>
pin   0 (RX0)   <-  TX<br/>
ground ground  -- ground<br/>
power 3.3V -- 3.3V

##### ESP8266

The ESP module connects to a network through WiFi and displays a user interface on its webpage. This user interface is how commadns can be sent to the car which the Arduino manages.

##### Arduino Mega

The Arduino keeps checking if a command has been sent through the Serial from the ESP. If a command is read, i.e. `MOVE:20`, the car moves forward 20 units (cm). The Arduino is also sending Lidar data through Serial to the ESP, which displays current distance Lidar records (cm) onto the website. 
