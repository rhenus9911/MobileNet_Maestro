#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <time.h>

#define PING_PKT_S 64

struct ping_pkt {
    struct icmphdr hdr;
    char msg[PING_PKT_S - sizeof(struct icmphdr)];
};

unsigned short checksum(void *b, int len) {    
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char*)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    const char *hostname = argv[1];
    struct hostent *hname = gethostbyname(hostname);
    if (hname == NULL) {
        perror("gethostbyname");
        return 1;
    }

    struct sockaddr_in addr_con;
    addr_con.sin_family = hname->h_addrtype;
    addr_con.sin_port = 0;
    addr_con.sin_addr.s_addr = *(long*)hname->h_addr;

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct ping_pkt pckt;
    struct sockaddr_in r_addr;
    int addr_len = sizeof(r_addr);
    int msg_count = 0;
    int msg_received_count = 0;
    int flag = 1;
    struct timespec time_start, time_end, tfs, tfe;
    long double rtt_msec = 0, total_msec = 0;

    while (msg_count < 5) {
        memset(&pckt, 0, sizeof(pckt));
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();
        for (int i = 0; i < sizeof(pckt.msg) - 1; i++) {
            pckt.msg[i] = i + '0';
        }
        pckt.msg[sizeof(pckt.msg) - 1] = 0;
        pckt.hdr.un.echo.sequence = msg_count++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

        usleep(1000000);
        clock_gettime(CLOCK_MONOTONIC, &tfs);
        if (sendto(sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&addr_con, sizeof(addr_con)) <= 0) {
            perror("sendto");
            flag = 0;
        }
        clock_gettime(CLOCK_MONOTONIC, &tfe);

        if (recvfrom(sockfd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &addr_len) <= 0 && msg_count > 1) {
            perror("recvfrom");
        } else {
            clock_gettime(CLOCK_MONOTONIC, &time_end);
            double timeElapsed = ((double)(tfe.tv_nsec - tfs.tv_nsec)) / 1000000.0;
            rtt_msec = (time_end.tv_sec - tfs.tv_sec) * 1000.0 + (time_end.tv_nsec - tfs.tv_nsec) / 1000000.0;

            if (flag) {
                printf("Ping to %s, rtt = %Lf ms\n", hostname, rtt_msec);
                msg_received_count++;
            }
        }
    }
    close(sockfd);
    return 0;
}
