#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include <sys/types.h>

void check_bluetooth_connection() {
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    char addr[19] = {0};
    char name[248] = {0};
    int connected_devices = 0;

    dev_id = hci_get_route(NULL);
    if (dev_id < 0) {
        perror("hci_get_route");
        exit(1);
    }

    sock = hci_open_dev(dev_id);
    if (sock < 0) {
        perror("hci_open_dev");
        exit(1);
    }

    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info *)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("hci_inquiry");
        exit(1);
    }

    for (int i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");

        // Check if the device is connected
        struct sockaddr_rc addr_rc = {0};
        int status;
        int rfcomm_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
        addr_rc.rc_family = AF_BLUETOOTH;
        addr_rc.rc_channel = 1;
        str2ba(addr, &addr_rc.rc_bdaddr);

        status = connect(rfcomm_sock, (struct sockaddr *)&addr_rc, sizeof(addr_rc));
        if (status == 0) {
            printf("Device Address: %s\n", addr);
            printf("Device Name: %s\n", name);
            printf("Device is connected.\n");
            connected_devices++;
        }
        close(rfcomm_sock);
    }

    if (connected_devices == 0) {
        printf("No connected devices.\n");
    }

    free(ii);
    close(sock);
}

int main(int argc, char **argv) {
    check_bluetooth_connection();
    return 0;
}
