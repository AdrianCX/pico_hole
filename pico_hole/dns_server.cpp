#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
 
#include "lwip/apps/lwiperf.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
 
#include "hardware/structs/rosc.h"

#include "dns.h"
#include "mappings.h"
#include "wifi.h"

#include <map>

#include "dns_server.h"

#define log printf

u8_t downstream_buffer[DECODE_BUFFER_SIZE];
u8_t upstream_buffer[DECODE_BUFFER_SIZE];

struct udp_pcb downstream_pcb;
struct udp_pcb upstream_pcb;

u32_t last_update = 0;

struct RequestSource
{
    RequestSource(const ip_addr_t ipaddr, u16_t port) : m_ipaddr(ipaddr), m_port(port), m_timestamp(to_ms_since_boot(get_absolute_time())) {}   

    const ip_addr_t m_ipaddr;
    u16_t m_port;
    u32_t m_timestamp;
};

std::map<int, RequestSource> m_active_requests;

void downstream(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void upstream(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void cleanup_requests();

extern "C" bool start_dns_server()
{
    const ip_addr_t any_addr = {0};
    ip_addr_t remote_addr;

    // Yes, it's weird it returns 1 on success and 0 on failure: https://www.nongnu.org/lwip/2_0_x/ip4__addr_8c.html
    if (0 == ip4addr_aton(DNS_SERVER, &remote_addr))
    {
        log("Failed decoding remote dns server address, address: %s\r\n", DNS_SERVER);
        return false;
    }

    err_t err = udp_bind(&downstream_pcb, &any_addr, 53);
    if (err != 0)
    {
        log("Failed binding downdstream on port 53, error: %d\r\n", err);
        return false;
    }
    
    err = udp_bind(&upstream_pcb, &any_addr, 5353);
    if (err != 0)
    {
        log("Failed binding downdstream on port 53, error: %d\r\n", err);
        return false;
    }

    udp_recv(&downstream_pcb, &downstream, NULL);
    udp_recv(&upstream_pcb, &upstream, NULL);
    udp_connect(&upstream_pcb, &remote_addr, DNS_SERVER_PORT);

    return true;
}


void downstream(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    //log("downstream: arg=%p, pcb=%p, pbuf=%p, data=%p, len=%d, remote_port: %d\n", arg, pcb, p, (p != NULL ? p->payload : NULL), (p != NULL ? p->len : 0), port);

    cleanup_requests();
    
    size_t downstream_buffer_size = DECODE_BUFFER_SIZE;
    int rc = dns_decode((dns_decoded_t *)downstream_buffer, &downstream_buffer_size,(const dns_packet_t *)p->payload, p->len);
        
    if (rc != 0)
    {
        log("Failed to decode input packet, error code: %d\r\n", rc);
        pbuf_free(p);
        return;
    }

    bool allowed = true;
    dns_query_t* query = (dns_query_t*)downstream_buffer;
    
    // Check all questions if allowed
    for (size_t i = 0 ; allowed && i<query->qdcount; i++)
    {
        if (query->questions[i].name != NULL)
        {
            allowed = allowed && check_dns_name(query->id, query->questions[i].name);
        }
    }
    
    // If not allowed, then just drop it, no reply
    if (!allowed)
    {
        pbuf_free(p);
        return;
    }
    
    m_active_requests.insert(std::make_pair(query->id, RequestSource(*addr, port)));

    err_t err = udp_send(&upstream_pcb, p);

    if (err != 0)
    {
        log("Failed sending request to upstream server, err: %d\r\n", err);
        m_active_requests.erase(query->id);
    }
    
    pbuf_free(p);
}

void upstream(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    //log("upstream: arg=%p, pcb=%p, pbuf=%p, data=%p, len=%d, queue_len: %d\n", arg, pcb, p, (p != NULL ? p->payload : NULL), (p != NULL ? p->len : 0), m_active_requests.size());

    size_t upstream_buffer_size = DECODE_BUFFER_SIZE;
    int rc = dns_decode((dns_decoded_t *)upstream_buffer, &upstream_buffer_size,(const dns_packet_t *)p->payload, p->len);
        
    if (rc != 0)
    {
        log("Failed to decode upstream input packet, error code: %d\r\n", rc);
        pbuf_free(p);
        return;
    }
    dns_query_t* query = (dns_query_t*)downstream_buffer;

    auto it = m_active_requests.find(query->id);
    if (it == m_active_requests.end())
    {
        log("Unknown id for packet reply: %d\r\n", query->id);
        pbuf_free(p);
        return;
    }

    ip_addr_t dst_ip = it->second.m_ipaddr;
    u16_t dst_port = it->second.m_port;

    err_t err = udp_sendto(&downstream_pcb, p, &dst_ip, dst_port);
    if (err != 0)
    {
        log("UDP send failure, err: %d\r\n", err);

        // Fall through to free p
    }
    
    pbuf_free(p);
}

void cleanup_requests()
{
    u32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time - last_update >= TIMEOUT_DNS_REQUEST/2)
    {
        for (auto it=m_active_requests.begin();it!=m_active_requests.end();)
        {
            if (current_time - it->second.m_timestamp >= TIMEOUT_DNS_REQUEST)
            {
                it = m_active_requests.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}


