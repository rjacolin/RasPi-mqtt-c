RasPi-mqtt-c
============

This C sample shows how to use MQTT to communicate with AirVantage.

This source code is based on http://git.eclipse.org/c/paho/org.eclipse.paho.mqtt.embedded-c.git/ sample.

Scenario
--------

This version simulates and sends humidity values and send temperature value (static value) as well.

Configuration
-------------
Setup your favorite distribution on your Raspberry Pi (see http://www.raspberrypi.org/help/quick-start-guide/)
Edit src/stdoutsub.c:
 - deviceId global variable, line 40
 - server name in the main function, line 225
 - username (same than deviceId) and password, in the main function, line 220 - 221

Build
-----
Just use the Makefile:
make

to build greenhouse executable.

Run
---

This sample can be run with ethernet bearer or AirPi Wireless connection.

AirPi shield
------------

AirPi shield allows radio connection to internet.

# Code configuration
1. In a terminal:
cat /proc/cpuinfo | grep Serial
2. Edit src/stdout.c file and update the deviceId/serialnumber (two lines) with your raspberry Pi serial number
3. make

# Harware installation

1. Plug your shield on Raspberry Pi and plug the USB cable from the device and your shield.
2. Use shield power to have power for raspberry pi and the shield.
3. Plug the antenna.
4. Insert your simcard.

# Process:
1. Install ppp, wvdial and usb-modeswitch packages
2. reboot
3. In a terminal: lsusb to check the Sierra Wireless modem appears
4. Copy the wvdial.conf config file on /etc
5. Edit the wvdial.conf to define your APN, username and password if needed.
6. Run wvdial with this configuration:
~~~
 wvdial wvdial.conf network
~~~
7. Check the IP using ifconfig (a ppp connection must be on) 
8. Start your application


