#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int portnum;
    struct hostent *hostp;
    printf ("Nhap so cong: \n");
    cin >> portnum;
    char *hostaddrp;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portnum);

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    char buf[2048];
    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);
   
    while (1)
    {
        int ret = recvfrom(receiver, buf, sizeof(buf), 0,
            (struct sockaddr *)&sender_addr,(socklen_t*) &sender_addr_len);
        if (ret < sizeof(buf))
            buf[ret] = 0;
        hostp = gethostbyaddr((const char *)&sender_addr.sin_addr.s_addr, 
              sizeof(sender_addr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      perror("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(sender_addr.sin_addr);
    if (hostaddrp == NULL)
      perror("ERROR on inet_ntoa\n");
    printf("server received datagram from %s (%s)\n", 
       hostp->h_name, hostaddrp);
    }
   
}