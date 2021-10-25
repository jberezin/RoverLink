# RoverLink


Turn 2 esp32 boards into pixhawk telemetry for use with Ardupilot/Ardurover Mission Planner

I used to use these cheap Chinese 900mhz Pixhawk Telemetry systems but burned up too many of them due to other issues and thought there must be a way to DIY this.  Also, I can´t get them cheaply or quickly anymore but ESP32s are always available for $5-$10.

This uses the 2.4ghz radios in the ESP32 chips but does not need Wifi routers, networks, or IP addresses.   We are leveraging the simple and lightweight protocol ESP-NOW by Expressif, the makers of the ESP32.  We are simply extending the USB cable between your Pixhawk and your Mission Planner PC using these radios.   We do not even look at the Mavlink protocol but just pass data along.



Preliminary Setup:

Connect the base station board to a Windows PC running Mission Planner using its USB connector (or USB to serial converter if board doesn´t have USB).  Please visit ardupilot.org for more information on Ardupilot, Ardurover, and Mission Planner.

Connect the other board to a Pixhawk compatible flight controller.  I only used three wires: Transmit, Receive, and Ground.  I powered the ESP32 in the rover with a separate power source to not draw too much power from the Pixhawk.


Set the baud rate for both the Pixhawk and the Mission Planner to 115,200 (Not 57,600),

This means you should edit the parameters as follows from mission planner while your flight controller is directly connected to your Windows PC using a direct usb connection:

    

SERIAL1_BAUD	115
SERIAL1_OPTIONS	  0	 
SERIAL1_PROTOCOL  2	
    

I set serial1 protocol to 2, for Mavlink 2, because I heard that was more efficient but it also worked ok with Mavlink 1 Protocol.

The code works under the Arduino IDE so novices can use it without having to install the whole Expressif IDE.	 

Make sure you can run simple Arduino examples like blink before you try this code.

Build process:

1. Figure out your MAC addresses for each board (hard coded in .ino file)

2. Edit them into the .ino file in place of the supplied mac addresses.

3. Compile and download into the Base Station ESP32.

4. Uncomment the #define ROVER 1 file and download into the ROVER ESP32.

5. Make sure that pin 25 and 26  (transmit and receive respectively) are available on your ESP32 dev board or change the following line of code if you need different pins:

 SerialPort.begin(115200, SERIAL_8N1, 26, 25);   // for ttgo tdisplay 2nd serial port because rover is using serial rather than usb to plug into pixhawk

My devboards (TTGO T-Displays)  have both serial and USB ports so I use the USB port on the Rover side normally just for power, but it is also valuable for debugging if you define the debugging in the script.

It will run at 57600 also but you would have to change the code in the arduino sketch.  I wanted it faster so chose 115200 and found that the system was much more responsive than using off the shelf 900 Mhz telemetry systems.  
   


Test process:

1. Plug Base Station ESP32 into your Mission Controller PC

2. Plug Rover ESP32 into your Flight Controller Telemetry 1 port (TX to RX, RX to TX, and Ground to Ground)

3. Open Mission Planner using the Serial Port of the Base Station ESP32

4. Mission planner should function normally and you can pickup your controller and rock it and see if the altitude indicator on Mission Planner rotates.

5. Check your range of operation by driving your rover around your desired work area and making sure you don´t lose connection.  Range will vary based on many factors.  Range can be increased by using dipole antennas rather than pcb antennas.


Enhancements:

I want to put in better error reporting, utilize the OLED display on the TTGO T-display to output mac addresses and other status information, and improve the documentation but do not have a timeframe for this.  I am open to having others contribute to this project.




Credits and Reading:


Based on an ESP32-NOW demo by Anas Kuzechie, and then hacked up a bit to support bidirectional full duplex communications.


https://www.youtube.com/watch?v=GzugxHEzB0g&ab_channel=AnasKuzechie

https://akuzechie.blogspot.com/2021/05/esp32-to-esp32-communication-via-esp-now.html


ESP-NOW information:

https://www.espressif.com/en/products/software/esp-now/overview

Pixhawk Information 	

https://pixhawk.org/

Ardupilot Information

https://ardupilot.org
