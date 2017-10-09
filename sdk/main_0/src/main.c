#include <stdio.h>

#include "xparameters.h"
#include "netif/xadapter.h"
#include "platform.h"
#include "platform_config.h"

#include "config.h"
#include "pcie_to_tcp.h"

// missing declaration in lwIP
void lwip_init(void);

void print_ip(char *msg, struct ip_addr *ip) 
{
    print(msg);
    xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip), ip4_addr3(ip), ip4_addr4(ip));
}

int main(void)
{
    struct netif *netif, server_netif;
    struct ip_addr ipaddr, netmask, gw;
    NET_CONFIG *netcfg;
    u16 port;

    // the mac address of the board. this should be unique per board
    unsigned char mac_ethernet_address[] = SERVER_MAC;

    netif = &server_netif;
    netcfg = (NET_CONFIG *)FLASH_IMAGE_BASEADDR;

    init_platform();

    if (netcfg->magic == NET_CONFIG_MAGIC)
    {
    	xil_printf("Loading settings from flash...\r\n");

    	#define LOAD_ADDR(_a_, _b_) IP4_ADDR((_a_), (_b_)[0], (_b_)[1], (_b_)[2], (_b_)[3])

    	LOAD_ADDR(&ipaddr, netcfg->address);
    	LOAD_ADDR(&netmask, netcfg->netmask);
    	LOAD_ADDR(&gw, netcfg->gateway);

    	port = netcfg->port;
    }
    else
    {
    	xil_printf("Loading default settings...\r\n");

    	// initliaze IP addresses to be used
	    SERVER_ADDRESS(&ipaddr);
	    SERVER_NETMASK(&netmask);
	    SERVER_GATEWAY(&gw);

	    port = SERVER_PORT;
    }    

    print_ip("[+] Address: ", &ipaddr);
    print_ip("[+] Netmask: ", &netmask);
    print_ip("[+] Gateway: ", &gw);

    lwip_init();

     // add network interface to the netif_list, and set it as default
    if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) 
    {
        xil_printf("xemac_add() ERROR\r\n");
        return -1;
    }

    netif_set_default(netif);

    // enable interrupts
    platform_enable_interrupts();

    // specify that the network if is up
    netif_set_up(netif);

    // start the application
    start_application(port);

    // receive and process packets
    while (1) 
    {
        xemacif_input(netif);
        transfer_data();
    }
  
    // never reached
    cleanup_platform();

    return 0;
}
