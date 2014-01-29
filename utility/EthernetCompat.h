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

#if !defined(__ETHERNET_COMPAT_H__)
#define __ETHERNET_COMPAT_H__

#define __ETHERNET_COMPAT_BONJOUR__

#include <stdint.h>

extern const uint8_t ECSockClosed;
extern const uint8_t ECSnCrSockSend;
extern const uint8_t ECSnCrSockRecv;
extern const uint8_t ECSnMrUDP;
extern const uint8_t ECSnMrMulticast;


void ethernet_compat_write_data(int socket, uint8_t* data, uint8_t* dst, uint16_t len);

#endif // __ETHERNET_COMPAT_H__
