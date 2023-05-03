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
string convertToString(char a[])
{
    string s = a;
    return s;
}
void removeClient(int client[64],int n,int i) {
 if (i < n) {
        // reduce size of array and move all
        // elements one space ahead
        n = n - 1;
        for (int j = i; j < n; j++) {
            client[j] = client[j + 1];
        }
    }
 
   
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

    fd_set fdread;
    
    int clients[64];
    int num_clients = 0;
    string name[64];
    char hoten[64];
    char test[11];
    char buf[256];
    char message[256];
    string Ask = "What your client id?\n";
    
    while (1)
    {
        // Xóa tất cả socket trong tập fdread
        FD_ZERO(&fdread);

        // Thêm socket listener vào tập fdread
        FD_SET(listener, &fdread);
        int maxdp = listener + 1;

        // Thêm các socket client vào tập fdread
        for (int i = 0; i < num_clients; i++)
        {
            FD_SET(clients[i], &fdread);
            if (maxdp < clients[i] + 1)
                maxdp = clients[i] + 1;
            cout << i << " - " << name[i] << "\n";
           if (name[i].size()!=0) continue;
           else send(clients[i], Ask.c_str(), Ask.size() + 1, 0);

        }
        // Cấu trúc xác định thời gian chờ (5s)
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        // Chờ đến khi sự kiện xảy ra
        int ret = select(maxdp, &fdread, NULL, NULL, NULL);

        if (ret < 0)
        {
            perror("select() failed");
            return 1;
        }

        // Kiểm tra sự kiện có yêu cầu kết nối
        if (FD_ISSET(listener, &fdread))
        {
            int client = accept(listener, NULL, NULL);
            printf("Ket noi moi: %d\n", client);
            clients[num_clients++] = client;
        }
        int tmp[64];
        for (int i=0;i< num_clients;i++) {
            tmp[i]=clients[i];
        }
        
        
        // Hoi ten cua client
        for (int i = 0; i< num_clients;i++) {
         
         
         } 
        // Kiểm tra sự kiện có dữ liệu truyền đến socket client
        for (int i = 0; i < num_clients; i++)
            if (FD_ISSET(clients[i], &fdread))
            {
                ret = recv(clients[i], buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                    printf("Client %d disconnected\n", clients[i]);
                    removeClient(clients, num_clients, i);
                    i--;
                    continue;
                }
            buf[ret] = 0;
            memcpy(test, buf, 10);
            if (strcmp(test,"client_id:")==0)
            {
            int hoten_len = ret - 10;
            memcpy(hoten, buf + 10, hoten_len);
            hoten[hoten_len-1] = 0;
            name[i]=convertToString(hoten);
            cout << hoten << " - " << name[i] << " - " << i << "\n";
            if (name[i].size()!=0)
            {
                for (int k=0;k<num_clients;k++)
                {   
                    char hellomsg[256];
                    sprintf(hellomsg, "%s joined the chat",name[i].c_str());
                    if ((tmp[k]!=clients[i])&&(name[k].size()!=0))
                    send(tmp[k],hellomsg,sizeof(hellomsg),0);
                }
            }
            continue;
            }
            if (name[i].size()!=0)
            {   
                // Thoi gian gui
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf(message, "%d-%02d-%02d %02d:%02d:%02d\n %s: %s",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                           tm.tm_hour, tm.tm_min, tm.tm_sec,name[i].c_str(),buf);
                for (int j=0;j<num_clients;j++)
                {   
                    
                    if ((tmp[j]!=clients[i])&&(name[j].size()!=0))
                    send(tmp[j],message,sizeof(message),0);
                }
            }
            
            }
    }

    close(listener);    

    return 0;
}
