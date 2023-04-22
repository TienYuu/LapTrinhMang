#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstring>
// Lay ngay gio hien tai
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       thoigian[80];
    tstruct = *localtime(&now);
    strftime(thoigian, sizeof(thoigian), "%Y-%m-%d %X", &tstruct);
    return thoigian;
}
string convertToString(char* a)
{
    string s = a;
    return s;
}
int main() {
// Khai bao socket
int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// Nhap thong tin server
char ipadd[20];
char test[11];
int portnum;
string no;
printf ("Nhap dia chi ip: ");
cin >> ipadd;
printf ("Nhap so cong: ");
cin >> portnum;
   
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
string ten;
string ngaysinh;
string diem;
string mssv;
string ngaygios = currentDateTime();
string ip = convertToString(ipadd);
printf("Nhap ten :\n");
getline(cin >> ws ,ten);
printf("Nhap MSSV :\n");
getline(cin,mssv);
printf("Nhap ngay sinh (Y-M-D) : \n");
getline(cin,ngaysinh);
printf("Nhap diem trung binh mon: \n");
getline(cin,diem);
string tonghop = ip + " " + ngaygios + " " + mssv + " " + ten +" "+ngaysinh+" "+ diem;
const char* msg = tonghop.c_str();
 send(client, msg,2000, 0);
// Nhan tin nhan tu server
char buf[2048];
int len = recv(client, buf, sizeof(buf), 0);
buf[len] = 0;
printf("Data received: %s\n", buf);
// Ket thuc, dong socket
close(client);
return 0;
}

// char thong tin ...
// co the dung while de nhap thong tin, khi mssv = 0000 thi ngung

// sprintf (buf,"%s %s ...", mssv,...)
// sau khi dung co the nhap thi dung fflush(stdin) neu bi nhay co the dung getchar()
// send (client,buf,strlen(buf),0)