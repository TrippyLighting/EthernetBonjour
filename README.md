EthernetBonjour
===============

Bonjour (ZeroConf) Library for Arduino & Teensyduino

mDNS (registering services) and DNS-SD (service discovery) has been tested and works on:
Teensy++2 with WIZ81MJ, Teensy3 with WIZ820io and Sparkfun Mega Pro

Using Arduino 1.6.4 and Teensyduino 1.18, Mega Pro

The newest revised code replaces all direct hardware calls to the W5100 chip with calls to EthernetUDP methods.
This will provide much better adaptability to different Ethernet hardware. 
