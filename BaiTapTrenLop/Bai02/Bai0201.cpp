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
#include <sys/select.h>

string delUnnecessary(string &str)
{
    int size = str.length();
    for(int j = 0; j<=size; j++)
    {
        for(int i = 0; i <=j; i++)
        {
            if(str[i] == ' ' && str[i+1] == ' ')
            {
                str.erase(str.begin() + i);
            }
            else if(str[0]== ' ')
            {
                str.erase(str.begin());
            }
            else if(str[i] == '\0' && str[i-1]== ' ')
            {
                str.erase(str.end() - 1);
            }
        }
    }
    return str;
}
string convertToString(char a[])
{
    string s = a;
    return s;
}
void toUpper(std::string &str) {
    if (str.length() == 0) {
        return;
    }
for (int i=0;i<str.length();i++) {
    str[i] = std::tolower(str[i]);
}
str[0] = std::toupper(str[0]);
}
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

    fd_set fdread, fdtest;
    
    // Xóa tất cả socket trong tập fdread
    FD_ZERO(&fdread);
    
    // Thêm socket listener vào tập fdread
    FD_SET(listener, &fdread);

    char buf[256];
    struct timeval tv;
    
    int users[64];      // Mang socket client da dang nhap
    char *user_ids[64]; // Mang id client da dang nhap
    int num_users = 0;  // So luong client da dang nhap

    while (1)
    {
        fdtest = fdread;

        // Thiet lap thoi gian cho
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        // Chờ đến khi sự kiện xảy ra
        int ret = select(FD_SETSIZE, &fdtest, NULL, NULL, NULL);

        if (ret < 0)
        {
            perror("select() failed");
            return 1;
        }

        if (ret == 0)
        {
            printf("Timed out!!!\n");
            continue;
        }

        for (int i = listener; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &fdtest))
            {
                if (i == listener)
                {
                    int client = accept(listener, NULL, NULL);
                    if (client < FD_SETSIZE)
                    {
                        FD_SET(client, &fdread);
                        printf("New client connected: %d\n", client);
                        num_users ++;
                        char msg[256] ;
                        sprintf(msg,"Xin chao! Hien co %d client dang dang nhap",num_users);
                        send(client, msg, strlen(msg), 0);
                    }
                    else
                    {
                        // Dang co qua nhieu ket noi
                        close(client);
                    }
                }
                else
                {
                    int ret = recv(i, buf, sizeof(buf), 0);
                    if (ret <= 0)
                    {
                        FD_CLR(i, &fdread);
                        close(i);
                    }
                    else
                    {
                        buf[ret] = 0;
                        printf("Received from %d: %s\n", i, buf);
                        string tmp = convertToString(buf);
                        delUnnecessary(tmp);
                        toUpper(tmp);
                        cout << tmp;
                        char *guidi;
                        guidi=tmp.data();
                        send(i,guidi,strlen(guidi),0);
                        
                    }
                }
            }
        }
    }

    close(listener);    

    return 0;
}