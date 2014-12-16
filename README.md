RasPi-mqtt-c
============

This C sample show how to use MQTT to communicate with AirVantage.

This source code is based on http://git.eclipse.org/c/paho/org.eclipse.paho.mqtt.embedded-c.git/ sample.

Scenario
--------

This version simulates and sends humidity values and send temperature value (static value) as well.

Configuration
-------------
Setup your favorite distribution on your Raspberry Pi
Edit src/stdoutsub.c:
 - deviceId global variable, line 40
 - server name in the main function, line 219
 - username (same than deviceId) and password, in the main function, line 220

Build
-----
Just use the Makefile:
make

to build greenhouse executable.

Run
---

This sample can be run with ethernet bearer or AirPi Wireless connection.
