//  Copyright (C) 2010 Georg Kaindl
//  http://gkaindl.com
//
//  This file is part of Arduino EthernetBonjour.
//
//  EthernetBonjour is free software: you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public License
//  as published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  EthernetBonjour is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with EthernetBonjour. If not, see
//  <http://www.gnu.org/licenses/>.
//


#include <utility/EthernetCompat.h>

#if defined(__ETHERNET_COMPAT_BONJOUR__)

#include <utility/w5100.h>
#include <Arduino.h>

#if defined(__arm__)
#include "SPIFIFO.h"
#ifdef  HAS_SPIFIFO
#define USE_SPIFIFO
#endif
#endif

#define W5200

#ifdef W5200
#define TXBUF_BASE 0x8000
#define RXBUF_BASE 0xC000
#define SMASK      0x0FFF
#else
#define TXBUF_BASE 0x4000
#define RXBUF_BASE 0x6000
#define SMASK      0x07FF
#endif


const uint8_t ECSockClosed       = SnSR::CLOSED;
const uint8_t ECSnCrSockSend     = Sock_SEND;
const uint8_t ECSnCrSockRecv     = Sock_RECV;
const uint8_t ECSnMrUDP          = SnMR::UDP;
const uint8_t ECSnMrMulticast    = SnMR::MULTI;

//updated to include Teensy, Teensy++ & Teensy3 processors
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
inline static void initSS()    { DDRB  |=  _BV(4); };
inline static void setSS()     { PORTB &= ~_BV(4); };
inline static void resetSS()   { PORTB |=  _BV(4); };
#elif defined(__AVR_ATmega32U4__) && defined(CORE_TEENSY)
inline static void initSS()    { DDRB  |=  _BV(0); };
inline static void setSS()     { PORTB &= ~_BV(0); };
inline static void resetSS()   { PORTB |=  _BV(0); };
#elif defined(__AVR_ATmega32U4__)
inline static void initSS()    { DDRB  |=  _BV(6); };
inline static void setSS()     { PORTB &= ~_BV(6); };
inline static void resetSS()   { PORTB |=  _BV(6); };
#elif defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB162__)
inline static void initSS()    { DDRB  |=  _BV(0); };
inline static void setSS()     { PORTB &= ~_BV(0); };
inline static void resetSS()   { PORTB |=  _BV(0); };
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
inline static void initSS()    { pinMode(10, OUTPUT); };
inline static void setSS()     { digitalWriteFast(10, LOW); };
inline static void resetSS()   { digitalWriteFast(10, HIGH); };
#else
inline static void initSS()    { DDRB  |=  _BV(2); };
inline static void setSS()     { PORTB &= ~_BV(2); };
inline static void resetSS()   { PORTB |=  _BV(2); };
#endif


#ifdef USE_SPIFIFO
uint16_t ethernet_compat_write_private(uint16_t addr, const uint8_t *buf, uint16_t len)
{
  uint32_t i;

#ifdef W5200
	SPIFIFO.clear();
	SPIFIFO.write16(addr, SPI_CONTINUE);
	SPIFIFO.write16(len | 0x8000, SPI_CONTINUE);
	for (i=0; i<len; i++) {
		SPIFIFO.write(buf[i], ((i+1<len) ? SPI_CONTINUE : 0));
		SPIFIFO.read();
	}
	SPIFIFO.read();
	SPIFIFO.read();
#else
    for (i=0; i<len; i++) {
	SPIFIFO.write16(0xF000 | (addr >> 8), SPI_CONTINUE);
	SPIFIFO.write16((addr << 8) | buf[i]);
	addr++;
	SPIFIFO.read();
	SPIFIFO.read();
    }

#endif
  return len;
}
#else
uint16_t ethernet_compat_write_private(uint16_t addr, const uint8_t *buf, uint16_t len)
{
#ifdef W5200
	    setSS();
	    SPI.transfer(addr >> 8);
	    SPI.transfer(addr & 0xFF);
	    SPI.transfer(((len >> 8) & 0x7F) | 0x80);
	    SPI.transfer(len & 0xFF);
	    for (uint16_t i=0; i<len; i++) {
	      SPI.transfer(buf[i]);
	    }
	    resetSS();
#else
    for (uint16_t i=0; i<len; i++) {
      setSS();
      SPI.transfer(0xF0);
      SPI.transfer(addr >> 8);
      SPI.transfer(addr & 0xFF);
      addr++;
      SPI.transfer(buf[i]);
      resetSS();
    }
#endif
  return len;
}
#endif

void ethernet_compat_write_data(int socket, uint8_t* src, uint8_t* dst, uint16_t len)
{
   uint16_t size;
   uint16_t dst_mask;
   uint16_t dst_ptr, dst_ptr_base;

   dst_mask = (uintptr_t)dst & SMASK;
   dst_ptr_base = TXBUF_BASE + socket * W5100Class::SSIZE;
   dst_ptr = dst_ptr_base + dst_mask;

   if( (dst_mask + len) > W5100Class::SSIZE ) 
   {
 	size = W5100Class::SSIZE - dst_mask;
     ethernet_compat_write_private(dst_ptr, (uint8_t *) src, size);
     src += size;
 	  ethernet_compat_write_private(dst_ptr_base, (uint8_t *) src, len - size);
   } 
   else
     ethernet_compat_write_private(dst_ptr, (uint8_t *) src, len);
}



#endif // __ETHERNET_COMPAT_BONJOUR__
