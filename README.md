# PicoHole - Simple DNS blocker for smart TV ads on raspberry pi pico

A simple DNS forwarder that only allows hosts from a whitelist to proceed.
This currently blocks all domains except those used by netflix/disney+.

Reason for this is the content monitoring and built in ads on my samsung TV.

How it looks
![Alt text](/documentation/pico_front.jpg "")

### How to use

#### Configure wifi credentials and compile.

1. Install pico dependencies
```
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi
```

2. Fetch submodules
```
git submodule update --init --recursive
```

3. Configure wifi in example (otherwise it won't connect):
```
hello_world/config/wifi.h
```

4. Compile

from main folder:

```
mkdir build
cd build
cmake ..
make
```

#### Copy firmware to pico

Bootup raspberry pi pico in bootsel mode and mount it.
Copy 'build/pico_hole/pico_hole.uf2' to it.
Start it up and let it connect to local wifi.

#### Configure router

Inside your router you should configure:

1. Static IP address for the pico AND the TV (should be under DHCP).

2. Firewall rules so all DNS traffic from TV is dropped except for target Pico.

The smart TV bypasses configured DNS server if it cannot connect and fetch ads. 
To prevent that from happening which would defeat this.

Example rules on openwrt -> Network -> Firewall:
![Alt text](/documentation/firewall.png "")

In example above (replace with your own):
- TV: 192.168.100.159
- Raspberry Pico: 192.168.100.216

#### Configure TV

For samsung

![Alt text](/documentation/samsung1.jpg "")
![Alt text](/documentation/samsung2.jpg "")
![Alt text](/documentation/samsung3.jpg "")
