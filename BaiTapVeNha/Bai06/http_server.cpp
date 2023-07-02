/*Lập trình thực hiện ứng dụng HTTP server gồm các tính năng sau:
+ Khi trình duyệt yêu cầu địa chỉ thư mục gốc (chỉ nhập tên miền và cổng ở thanh địa chỉ), trả lại kết quả cho trình duyệt một trang web
 có nội dung gồm các tập tin và thư mục con của thư mục hiện tại. Tên thư mục in đậm, tên file in nghiêng
. Mỗi tên thư mục hoặc tập tin là một hyperlink đến thư mục hoặc tập tin đó.
+ Nếu người dùng nhấn vào thư mục con thì server trả về nội dung của thư mục con đó.
+ Nếu người dùng nhấn vào tập tin thì server trả về nội dung của tập tin. Các tập tin cần xử lý gồm tệp văn bản 
(có phần mở rộng là TXT, C, CPP), tệp ảnh (phần mở rộng là JPG, PNG), tệp âm thanh (phần mở rộng là MP3).
Yêu cầu nộp bài: link GitHub chứa mã nguồn và ảnh chụp kết quả chạy thử
Hạn nộp bài: đến hết ngày 02/07/2023.

Ảnh minh họa:
+ Server trả về kết quả khi yêu cầu thư mục gốc.*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>

void *client_thread(void *);

void signal_handler(int signo)
{
    wait(NULL);
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

    signal(SIGPIPE, signal_handler);

    while (1)
    {
        int client = accept(listener, NULL, NULL);
        if (client == -1)
        {
            perror("accept() failed");
            continue;;
        }
        printf("New client connected: %d\n", client);
        
    
    
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_thread, &client);
        pthread_detach(thread_id);
    }
    
    close(listener);    

    return 0;
}

void *client_thread(void *param)
{
    int client = *(int *)param;
    char buf[2048];
    
    int ret = recv(client, buf, sizeof(buf) - 1, 0);
    if (ret <= 0)
        return NULL;
    
    buf[ret] = 0;
    printf("Received from %d: %s\n", client, buf);
    
    char method[16];
    char path[256];
    sscanf(buf, "%s %s", method, path);
    if (strcmp(path, "/test.cpp") == 0)
    {
        char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(client, response_header, strlen(response_header), 0);
        FILE *f = fopen("test.cpp", "rb");
        while (1)
        {   
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            strcat(buf,"\r\n");
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else if (strcmp(path, "/test2.txt") == 0)
    {
        char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(client, response_header, strlen(response_header), 0);
        FILE *f = fopen("test1/test2.txt", "rb");
        while (1)
        {   
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            strcat(buf,"\r\n");
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else if (strcmp(path, "/New%20Text%20Document.txt") == 0)
    {
        char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(client, response_header, strlen(response_header), 0);
        FILE *f = fopen("New Text Document.txt", "rb");
        while (1)
        {   
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            strcat(buf,"\r\n");
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else if (strcmp(path,"/test1")==0) {
        // Gui danh sach file cho client
            DIR *dir = opendir("/home/tiendung/laptrinhc/LTM/BaiTapVeNha/Bai06/test1");
            struct dirent *entry;
            char*  test[10]; 
            char* test1[10];
            
            int num_files = 0;
            int num_folders = 0;
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_type == DT_REG)
                {   
                    test[num_files]=(char*)malloc(256);
                    sprintf(test[num_files],"%s",entry->d_name);
                    num_files++;
                }
                else
                {   
                    test1[num_folders]=(char*)malloc(256);
                    sprintf(test1[num_folders],"%s",entry->d_name);
                    num_folders++;
                }
            }

            closedir(dir);
      char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(client, response_header, strlen(response_header), 0);
    for (int i=0;i<num_folders;i++)
    {
    char response_body[256];
    sprintf(response_body,"<h1> <a href=\"%s\" target = \"_self\" > <strong>  %s </strong> </a> </h1> ", test1[i],test1[i]);
    send(client, response_body, strlen(response_body), 0);
    }
    for (int i=0;i<num_files;i++)
    {
    char response_body1[256];
    sprintf(response_body1,"<h1> <a href=\"%s\" target = \"_self\"   > <em>  %s </em> </a> </h1>",test[i],test[i]);
    send(client, response_body1, strlen(response_body1), 0);
    }  
    }
    else if (strcmp(path, "/test.jpg") == 0)
    {
        char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nConnection: close\r\n\r\n";
        send(client, response_header, strlen(response_header), 0);

        FILE *f = fopen("test.jpg", "rb");
        while (1)
        {
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else if (strcmp(path, "/audio") == 0)
    {
        FILE *f = fopen("test.mp3", "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char response_header[2048];
        sprintf(response_header, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: audio/mp3\r\nConnection: keep-alive\r\n\r\n", fsize);
        send(client, response_header, strlen(response_header), 0);

        while (1)
        {
            int len = fread(buf, 1, sizeof(buf), f);
            if (len <= 0)
                break;
            send(client, buf, len, 0);
        }
        fclose(f);
    }
    else
    {
    // Gui danh sach file cho client
            DIR *dir = opendir("/home/tiendung/laptrinhc/LTM/BaiTapVeNha/Bai06");
            struct dirent *entry;
            char*  test[10]; 
            char* test1[10];
            
            int num_files = 0;
            int num_folders = 0;
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_type == DT_REG)
                {   
                    test[num_files]=(char*)malloc(256);
                    sprintf(test[num_files],"%s",entry->d_name);
                    num_files++;
                }
                else
                {   
                    test1[num_folders]=(char*)malloc(256);
                    sprintf(test1[num_folders],"%s",entry->d_name);
                    num_folders++;
                }
            }

            closedir(dir);
      char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(client, response_header, strlen(response_header), 0);
    for (int i=0;i<num_folders;i++)
    {
    char response_body[256];
    sprintf(response_body,"<h1> <a href=\"%s\" target = \"_self\" > <strong>  %s </strong> </a> </h1> ", test1[i],test1[i]);
    send(client, response_body, strlen(response_body), 0);
    }
    for (int i=0;i<num_files;i++)
    {
    char response_body1[256];
    sprintf(response_body1,"<h1> <a href=\"%s\" target = \"_self\"   > <em>  %s </em> </a> </h1>",test[i],test[i]);
    send(client, response_body1, strlen(response_body1), 0);
    }  
    }
    
    close(client);
}