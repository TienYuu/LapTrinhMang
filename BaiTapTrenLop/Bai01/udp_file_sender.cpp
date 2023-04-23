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
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    char ipadd[20];
    int portnum;
    char *filename ;
    printf ("Nhap ip: \n");
    cin >> ipadd;
    printf ("Nhap so cong: \n");
    cin >> portnum;
    printf ("Nhap ten file: \n");
    cin >> filename;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipadd);
    addr.sin_port = htons(portnum);
    char buf[2048];
    FILE *f = fopen(filename, "rb");
    while (!feof(f))
    {
        fread(buf, 1, sizeof(buf), f);
        sendto(sender, buf, strlen(buf), 0,
            (struct sockaddr *)&addr, sizeof(addr));
    }
    fclose(f);
    
   
}