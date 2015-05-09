EthernetBonjour
===============

Bonjour (ZeroConf) Library for Arduino & Teensyduino

mDNS (registering services) and DNS-SD (service discovery) has been tested and works on:
Mega Pro and Ethernet Shield

Using Arduino 1.6.4 and Sparkfun Mega Pro

The newest revised code replaces all direct hardware calls to the W5100 chip with calls to EthernetUDP methods.
This will provide much better adaptability to different Ethernet hardware. 
