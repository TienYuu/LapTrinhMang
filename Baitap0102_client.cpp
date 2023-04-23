#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000); 

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("connect() failed");
        return 1;
    }
        
     char buf[256];
     ifstream file("Baitap0102.txt");
     string s;
    while (getline(file, s))
    {
        strcpy(buf, s.c_str());
        send(client, buf, strlen(buf), 0);
    }
    cout << "Da gui!";
    file.close();
    // Ket thuc, dong socket
    close(client);
}