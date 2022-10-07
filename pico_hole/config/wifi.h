#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// Include from out of git repo to avoid leaking my own ssid/password if I submit by mistake
//#include "/home/adrian/wifi.h"

//#define WIFI_SSID "ssid"
//#define WIFI_PASSWORD "password"

#if !defined(WIFI_SSID) || !defined(WIFI_PASSWORD)
#error "Please define WIFI_SSID/WIFI_PASSWORD in this file" 
#endif

// Should be the usual gateway, code can be added to auto-detect but I did not dig enough in lwip for that.
//#define DNS_SERVER "192.168.100.1"
//#define DNS_SERVER_PORT 53

#if !defined(DNS_SERVER) || !defined(DNS_SERVER_PORT)
#error "Please define DNS_SERVER and DNS_SERVER_PORT in this file"
#endif

// Right now we only decode queries. To decode all fields use ifdef below. (not tested)
//#define DNS_FULL_PARSING

#endif //WIFI_CONFIG_H