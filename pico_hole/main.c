#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
 
#include "wifi.h"
#include "dns_server.h"

#include "lwip/udp.h"

int main(void)
{
    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("failed to initialise\r\n");
        return 0;
    }

    printf("Starting main\r\n");
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        while (true) {
            printf("failed to connect to wifi, ssid: %s, password: %s\n", WIFI_SSID, WIFI_PASSWORD);
            sleep_ms(3000);
        }

        return 1;
    }

    udp_init();
    start_dns_server();

    while (true) {
        sleep_ms(5000);
    }
        
    cyw43_arch_deinit();
    return 0;
}
