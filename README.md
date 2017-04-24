EthernetBonjour
===============

Bonjour (ZeroConf) Library for Arduino & Teensyduino

mDNS (registering services) and DNS-SD (service discovery) has been tested and works on:
Mega Pro and Ethernet Shield

Using Arduino 1.6.4 and Sparkfun Mega Pro

The newest revised code replaces all direct hardware calls to the W5100 chip with calls to EthernetUDP methods.
This will provide much better adaptability to different Ethernet hardware. 

22-Dec-2015 Updated to provide compatibility with Arduino 1.6.6. Since 1.6.6. the Ethernet library provides support for multicast a pre-requisite for Bonjour to work.
