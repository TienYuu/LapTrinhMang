#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

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

    int num_processes = 8;
    for (int i = 0; i < num_processes; i++)
        if (fork() == 0)
        {
        char buf[256];
        while (1)
            {
        
            // Chờ kết nối
        int client = accept(listener, NULL, NULL);
        printf("New client accepted in process %d: %d\n", client, getpid());
            // Chờ dữ liệu từ client
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        continue;
        puts(buf);
            // Xử lý dữ liệu, trả lại kết quả cho client
        buf[ret] = 0;
        char *msg = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Xin chao cac ban</h1></body></html>";
        send(client, msg, strlen(msg), 0);
        
        
            // Đóng kết nối
        close(client);
            }

        }
    
    getchar();
    close(listener);    

    killpg(0, SIGKILL);

    return 0;
}



