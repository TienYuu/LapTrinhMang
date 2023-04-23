#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <fstream>
#include <string>
#include <iostream>

int main() 
{   
    
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) 
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5)) 
    {
        perror("listen() failed");
        return 1;
    }

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, (socklen_t*)&clientAddrLen);
    if (client == -1)
    {
        perror("accept() failed");
        return 1;
    }
    printf("New client connected: %d\n", client);
    string sub = "0123456789";
    char buf[256];
    int count{};
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;

        buf[ret] = 0;
        string str(buf);
        
    size_t last_index{ str.size() > sub.size() ? str.size() - sub.size() : 0 };
    for (size_t i{0}; i <= last_index; ++i) {
        if (str.substr(i).rfind(sub, 0) == 0) {
            count++;
        }
    }
    
       
    }
    std::cout << "So lan la: " << count;
   

    close(client);
    close(listener);   
}