NexaGw
======

Use an Arduino and a cheap 433 board as a gateway for sending commands to Nexa, HomeEasy etc. devices.
The Arduino listens for commands on the serial port, and sends them after receiving a full message.

There is little magic involved here, I'm using Arduino Pro but pretty much any device with an output pin would do fine.
My sender is a super cheap device marked with FS1000A/XY-FST but most of the cheap 433 transmitters should be fine since we just use OOK.

Its designed to be flexible, so the message should be created on the computer side and the Arduino will just do coding and sending of the data.

I got some protocol information from here:
http://tech.jolowe.se/home-automation-rf-protocols/
But it didn't all fit, so I had to to some simple measurements with SDR and I also used rtl_433 from https://github.com/merbanan/rtl_433 to do some analyzis.

Packets are sent by sending messages to the serial port with the following format:
SNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNP
Where:
S = Sync, start of message
N = 1 or 0, a bit of the message
P = Pause, the end of the message

Example:
To Turn on a whole group I would issue this command
echo "S00100110000010110010111110110000P" > /dev/ttyUSB0

The message itself is 32 bit (+4 if dimming, but I haven't looked into that) consists of the following:
SSSS SSSS SSSS SSSS SSSS SSSS SSGP CCDD
Where:
S = Sender id, 26 bit identifying the sender. This should be a clone of your remote id if you want them to coexist
G = Group command, marks if its a group command
P = Power state, 1 for on 0 for off
C = The site mentioned above says this is the channel, I'm not sure about it. I leave it as 00
D = The device, 00 = device 1, 01 = device 2, 10 = device 3

You can use the Arduino app found here to get your remote ID, if you want to clone it:
http://www.telldus.se/forum/viewtopic.php?f=12&t=4072
