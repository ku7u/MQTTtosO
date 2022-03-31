# MQTTtosO
This software provides a means of block occupancy detection on a model railroad. It must be used in conjunction with JMRI software and an MQTT server.

## Important considerations
All configuration is done using the supplied menuing system via Bluetooth. A Bluetooth Serial app is required on your phone or tablet. I recommend BlueTooth Serial on Android by Kai Morich. Others I have tried all had issues. Connect your device to the ESP32 as soon as the software has been booted.

This software was developed using VSCode and Arduino avrisp with 'ESP32 Dev Module' board configuration. Initially it will fail to load into the host because of lack of memory. This is alleviated by modifying the partition scheme. Access that setting from the Arduino Tools menu. Select some scheme that does not use OTA (Over the Air software loading). Choosing this reduced the size from 101% of memory to 42%.

On first start the code will attemp to connect to the default WiFi SSID and will fail. At that point connect the Bluetooth Serial app to the device and enter a blank line. The menu should then display. Using the menu the WiFi SSID and password can be entered. At the same menu enter the MQTT server address. Provide a name for the node both for MQTT and Bluetooth. They can be the same.

A Bluetooth password can be supplied to prevent local hackers from tinkering with the device. However they can still connect. There is a menu choice to turn off Bluetooth completely. To turn it back on ground pin 5 briefly. This also sets the password to the default 'IGNORE' which is in effect no password so the operator must either ignore the threat, set a new password or turn off Bluetooth again.

After configuring the above, reboot the device using the menu.

The MQTT channel in the device must match the corresponding setting in JMRI. The default in JMRI is '/trains/' which is wrong. There should be no leading '/' on the channel ID. Get rid of it. The default channel ID in the code is 'trains/' which can be changed from the menu. Change the channel name to something that makes sense to you but be sure to do it in both places.

The topic string must be specified in a certain way in JMRI. The inbound sensor topic must be left as is, namely 'track/sensor/'. The outbound sensor topic must be changed to 'track/sensor/send/' to be compatible with the block zeroing (ghostbuster) feature. The system name in JMRI for a sensor must be of the form 'BOD/block/\<blockID\>'. Topic strings are case sensitive. Resulting data sent will be '\<channel\>/track/sensor/BOD/block/\<blockID\>' which is a little wordy but helps to avoid confusion when troubleshooting. It also allows for other kinds of sensors with names other than 'BOD' to be configured. 'blockID' must be a number.

Each block must be assigned one and only one 'keeper'. This is very important. Each block must have a keeper. A detector can be a keeper for 0, 1 or 2 blocks. The keeper collects all wheel counts for the block and is responsible for reporting to JMRI. Only the keeper reports to JMRI. Detectors that are not keepers send 'looseblock' messages to all other detectors when a wheel passes with data that indicates whether the affected block is increased or decreased. The keeper detector will receive such a message and affect its count for the block based on the message content. This is all setup in the code and no configuration is required of the operator. Note that a node will check all of its 8 possible blocks internally before sending out an MQTT 'looseblock' message so these messages are relatively rare.

Detectors are assigned block IDs from the menu choice 'I'. A block is assigned to both the west and the east side of the detector. If status is not required on one or the other side, enter '0' for the block ID. Whether the detector is to be the block's keeper is assigned from the same menu. Good practice requires that a schematic of the track plan be drawn showing detectors and blocks.
