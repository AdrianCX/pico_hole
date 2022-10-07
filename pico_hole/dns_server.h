#ifndef _DNS_SERVER_H
#define _DNS_SERVER_H

#define TIMEOUT_DNS_REQUEST 30000
#define DECODE_BUFFER_SIZE DNS_DECODEBUF_8K

#ifdef __cplusplus
extern "C" {
#endif
       
// Call this to start listener, execution will be via LWIP callbacks.
// Current thread will return, it should not end execution so do anything else or just loop waiting.
//
// Returns true if setup successful, false otherwise without callbacks configured.
bool start_dns_server();

// We require another file to have this implemented
bool check_dns_name(int id, const char *name);

#ifdef __cplusplus
}
#endif


#endif