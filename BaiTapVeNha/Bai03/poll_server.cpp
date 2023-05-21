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
#include <poll.h>
string convertToString(char a[])
{
    string s = a;
    return s;
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

    struct pollfd fds[64];
    int nfds = 1;

    fds[0].fd = listener;
    fds[0].events = POLLIN;
    string name[64];
    char hoten[64];
    char test[11];
    char buf[256];
    char message[256];
    string Ask = "What your client id?\n";
    

    while (1)
    {   
        for (int i = 1; i < nfds; i++)
        {
           cout << i << " - " << name[i] << "\n";
           if (name[i].size()!=0) continue;
           else send(fds[i].fd, Ask.c_str(), Ask.size() + 1, 0);

        }
        int ret = poll(fds, nfds, -1);
        if (ret < 0)
        {
            perror("poll() failed");
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            int client = accept(listener, NULL, NULL);
            if (nfds == 64)
            {
                // Tu choi ket noi
                close(client);
            }
            else
            {
                fds[nfds].fd = client;
                fds[nfds].events = POLLIN;
                nfds++;
                printf("Ket noi moi: %d\n", client);
            }
        }
        
        struct pollfd tmp[64];
        for (int i=1;i< nfds;i++) {
            tmp[i]=fds[i];
        }

        for (int i = 1; i < nfds; i++)
            if (fds[i].revents & POLLIN)
            {
                ret = recv(fds[i].fd, buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                    close(fds[i].fd);
                    // Xoa khoi mang
                    if (i < nfds - 1)
                        fds[i] = fds[nfds - 1];
                    nfds--;
                    i--;
                }
            buf[ret] = 0;
            memcpy(test, buf, 10);
            if (strcmp(test,"client_id:")==0)
            {
            int hoten_len = ret - 10;
            memcpy(hoten, buf + 10, hoten_len);
            hoten[hoten_len-1] = 0;
            name[i]=convertToString(hoten);
            
            if (name[i].size()!=0)
            {
                for (int k=0;k<nfds;k++)
                {   
                    char hellomsg[256];
                    sprintf(hellomsg, "%s joined the chat",name[i].c_str());
                    if ((tmp[k].fd!=fds[i].fd)&&(name[k].size()!=0))
                    send(tmp[k].fd,hellomsg,sizeof(hellomsg),0);
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
                for (int j=0;j<nfds;j++)
                {   
                    
                    if ((tmp[j].fd!=fds[i].fd)&&(name[j].size()!=0))
                    send(tmp[j].fd,message,sizeof(message),0);
                }
            }
            
            }
            
    }
    
    close(listener);    

    return 0;
}