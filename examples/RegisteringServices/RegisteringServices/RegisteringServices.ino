#include <SPI.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include <EthernetUdp.h>

int ledPin =  13;      		 //pin 13 on Arduino Uno. Pin 6 on a Teensy++2
int resetPin = 9;			 //reset pin for WIZ820io Ethernet module
uint16_t serverPort  = 8000; //TouchOSC (incoming port)
uint16_t destPort = 9000;    //TouchOSC (outgoing port)
char mac_string[20];		 //string to hold MAC address for Bonjour name


//everything on the network needs a unique MAC
#if defined(__MK20DX128__)
// Each Teensy3 has a unique MAC burned in
static byte mac[6];
void read(uint8_t word, uint8_t *mac, uint8_t offset) {
  FTFL_FCCOB0 = 0x41;             // Selects the READONCE command
  FTFL_FCCOB1 = word;             // read the given word of read once area

// launch command and wait until complete
  FTFL_FSTAT = FTFL_FSTAT_CCIF;
  while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF));

  *(mac+offset) =   FTFL_FCCOB5;       // collect only the top three bytes,
  *(mac+offset+1) = FTFL_FCCOB6;       // in the right orientation (big endian).
  *(mac+offset+2) = FTFL_FCCOB7;       // Skip FTFL_FCCOB4 as it's always 0.
}
void read_mac() {
  read(0xe,mac,0);
  read(0xf,mac,3);
}
void print_mac()  {
  size_t count = 0;
  for(uint8_t i = 0; i < 6; ++i) {
    if (i!=0) count += Serial.print(":");
    count += Serial.print((*(mac+i) & 0xF0) >> 4, 16);
    count += Serial.print(*(mac+i) & 0x0F, 16);
  }
  sprintf(mac_string, "%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  Serial.println();
}
#else
void read_mac() {}
 byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // you can find this written on the board of some Arduino Ethernets or shields
#endif


void setup()
{
// Add your initialization code here

//////////////////////////////
//WIZ520io reset code
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  delayMicroseconds(100);
  digitalWrite(resetPin, HIGH);
  delay(500);
//WIZ520io reset code end
//////////////////////////////


  Serial.begin(115200); //9600 for a "normal" Arduino board (Uno for example). 115200 for a Teensy ++2
  Serial.println("DHCP-Bonjour-based OSC server test 12/28/12");
  Serial.println("Reading MAC from hardware...");

  read_mac();

  Serial.print("MAC: ");
  print_mac();
  Serial.println();

 // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }

// print your local IP address:
  Serial.print("Local IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
// print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();

// Initialize the Bonjour/MDNS library. You can now reach or ping this
// Arduino via the host name "arduino.local", provided that your operating
// system is Bonjour-enabled (such as MacOS X).
// Always call this before any other method!

  	if (EthernetBonjour.begin(mac_string)){
       Serial.println("Setting Bonjour Name successful");
    }
    else{
      Serial.println("Bonjour Name could not be set");
    }

// Now let's register the service we're offering (a web service) via Bonjour!
// To do so, we call the addServiceRecord() method. The first argument is the
// name of our service instance and its type, separated by a dot. In this
// case, the service type is _osc. There are many other service types, use
// google to look up some common ones, but you can also invent your own
// service type, like _mycoolservice - As long as your clients know what to
// look for, you're good to go.
// The second argument is the port on which the service is running. This could be
// port 80 here, the standard HTTP port.
// The last argument is the protocol type of the service, either TCP or UDP.
// Of course, our OSC service is a UDPservice.
// With the service registered, it will show up in the Bonjour Browser on a Mac or
// in the Connections settings in TouchOSC

  if(
    EthernetBonjour.addServiceRecord("Teensy3 OSC Server._osc",
    serverPort,
    MDNSServiceUDP)){
    Serial.println("Bonjour Service Record added successfully");
   }
   else{
      Serial.println("Bonjour Service could not be added");
   }

}

void loop()
{
// This actually runs the Bonjour module. YOU HAVE TO CALL THIS PERIODICALLY,
// OR NOTHING WILL WORK! Preferably, call it once per loop().
	EthernetBonjour.run();

}






