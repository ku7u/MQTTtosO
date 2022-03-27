# MQTTtosO
This software provides a means of block occupancy detection on a model railroad. It must be used in conjunction with JMRI software and an MQTT server.

## Important considerations
All configuration is done using the supplied menuing system via Bluetooth. A Bluetooth Serial app is required on your phone or tablet. I recommend BlueTooth Serial on Android by Kai Morich. Others I have tried all had issues. Connect your device to the ESP32 as soon as the software has been booted.

This software was developed using VSCode and Arduino avrisp with 'ESP32 Dev Module' board configuration. Initially it will fail to load into the host because of lack of memory. This is alleviated by modifying the partition scheme. Access that setting from the Arduino Tools menu. Select some scheme that does not use OTA (Over the Air software loading). Choosing this reduced the size from 101% of memory to 42%.

On first start the code will attemp to connect to the default WiFi SSID and will fail. At that point connect the Bluetooth Serial app to the device and enter a blank line. The menu should then display. Using the menu the WiFi SSID and password can be entered. At the same menu enter the MQTT server address. Provide a name for the node both for MQTT and Bluetooth. They can be the same.

A Bluetooth password can be supplied to prevent local hackers from tinkering with the device. However they can still connect. There is a menu 
choice to turn off Bluetooth completely. To turn it back on ground pin 2 briefly.

After configuring the above, reboot the device using the menu.

The MQTT channel in the device must match the corresponding setting in JMRI. The default in JMRI is '/trains/' which is wrong. There should be no leading '/' on the channel ID. Fix it. The default channel ID in the code is 'trains/' which can be changed from the menu.

The topic string must be specified in a certain way in JMRI. The inbound sensor topic must be left as is, namely 'track/sensor/'. The outbound sensor topic must be changed to 'track/sensor/send/'. The system name for a sensor must be of the form 'BOD/block/<blockID>'. Topic strings are case sensitive. Resulting data sent will be '\<channel\>/track/sensor/BOD/block/\<blockID\>' which is a little wordy but helps to avoid confusion when troubleshooting. It also allows for other kinds of sensors with names other than 'BOD' to be configured.
