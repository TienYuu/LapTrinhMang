#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
int LenhDung(char* buf) {
int trave;
printf("%s - buf sau khi dua vao\n",buf); 
char lenh[8];
char format[10];
memcpy(lenh,buf,8);
printf("%s - Day la ma lenh\n",lenh);
if (strcmp(lenh,"GET_TIME")!=0)
{trave =0;}
else {
memcpy(format,buf+9,strlen(buf)-9);
printf("%s - Day la format\n",format);
if (strcmp(format,"dd/mm/yyyy")==0){
 trave =1;
}
else if (strcmp(format,"dd/mm/yy")==0){
 trave =2;
}
else if (strcmp(format,"mm/dd/yyyy")==0){
 trave =3;
}
else if (strcmp(format,"mm/dd/yy")==0){
 trave =4;
}
else {
 trave =5;
}
}
return trave;

}
void *client_thread(void *);

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
    char buf[256];
    char message[256];
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        
        buf[ret] = 0;
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        printf("%s\n",buf);
        int thu =LenhDung(buf);
        printf("%d\n - Day la so tra ve",thu);
        switch (thu) {
        case 0:
        {char *msg = "Hay sua cho dung: GET_TIME [format]";
        send(client, msg, strlen(msg), 0);}
        break;
        case 1:
        
        {time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(message, "Current date and time: %02d/%02d/%d  %02d:%02d:%02d\n ",tm.tm_mday, tm.tm_mon + 1, tm.tm_year+1900,tm.tm_hour, tm.tm_min, tm.tm_sec);
        send(client,message,sizeof(message),0);}
        break;
        case 2:
        {time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(message, "Current date and time: %02d/%02d/%d  %02d:%02d:%02d\n ",tm.tm_mday, tm.tm_mon + 1, tm.tm_year+1900-2000,tm.tm_hour, tm.tm_min, tm.tm_sec);
        send(client,message,sizeof(message),0);}
        break;
        case 3:
        {time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(message, "Current date and time: %02d/%02d/%d  %02d:%02d:%02d\n ",tm.tm_mon + 1, tm.tm_mday, tm.tm_year+1900,tm.tm_hour, tm.tm_min, tm.tm_sec);
        send(client,message,sizeof(message),0);}
        break;
        case 4:
        {time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(message, "Current date and time: %02d/%02d/%d  %02d:%02d:%02d\n ",tm.tm_mon + 1, tm.tm_mday, tm.tm_year+1900-2000,tm.tm_hour, tm.tm_min, tm.tm_sec);
        send(client,message,sizeof(message),0);}
        break;
        default:
        {
        char *msg = "Sai cu phap hay thu lai";
        send(client, msg, strlen(msg), 0);

        }
        }
    }

    close(client);
}