<?xml version="1.0" encoding="ISO-8859-1"?>
<app:application xmlns:app="http://www.sierrawireless.com/airvantage/application/1.0" 
        type="com.test.alfredo.myapplication" 
        name="GreenPi4Alf" 
        revision="0.0.1">
  <capabilities>
  
    <communication>
      <protocol comm-id="IMEI" type="MQTT" />
    </communication>
   
    <data>
      <encoding type="MQTT">
        <asset default-label="My Machine" id="machine">
          <variable default-label="Temperature" path="temperature" type="double"/>
          <variable default-label="Humidity" path="humidity" type="double"/>
          <variable default-label="Luminosity" path="luminosity" type="double"/>
          <setting default-label="Light" path="light" type="double"/>
          <setting default-label="Threshold" path="threshold" type="int"/>
          
          <command path="TurnOn" default-label="Turn on">
		        <parameter id="Light" default-label="Light" type="boolean" />
		      </command>
        </asset>
      </encoding>
    </data>  

  </capabilities>
</app:application>