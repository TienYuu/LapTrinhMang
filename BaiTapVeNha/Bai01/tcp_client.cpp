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
// Khai bao socket
int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// Nhap thong tin server
char ipadd[20];
char test[11];
int portnum;
while (1) {
    printf ("Nhap lenh: \n");
    cin >> test >> ipadd >> portnum;
    if (strcmp(test,"tcp_client")==0)
    break;
    else printf("Lenh sai, thu lai\n");
}
// Khai bao dia chi cua server
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = inet_addr(ipadd);
addr.sin_port = htons(portnum);
// Ket noi den server
int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));

if (res == -1) {
printf("Khong ket noi duoc den server!");
return 1;
}
// Gui tin nhan den server
printf("Nhap noi dung can gui: \n");
char msg[60];
 cin >> msg;
 send(client, msg, strlen(msg), 0);
// Nhan tin nhan tu server
char buf[2048];
int len = recv(client, buf, sizeof(buf), 0);
buf[len] = 0;
printf("Data received: %s\n", buf);
// Ket thuc, dong socket
close(client);
return 0;
}