#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
int users[64];      // Mang socket client da dang nhap
char *user_ids[64]; // Mang id client da dang nhap
int num_users = 0;  // So luong client da dang nhap


bool validname (char* str) {
int length = strlen(str);
int dem=0;
    for (int i = 0; i < length; i++) {
        int c = str[i];
        if (islower(c) && isalnum(c) )
        dem++;         
    }
    printf("%d\n",dem);
    printf("%d\n",length);
    if (dem==length) 
    {return true;}
    else return false;
}
void *client_thread(void *param)
{
    int client = *(int *)param;
    char buf[256];
    
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        else {
            
                    
                        buf[ret] = 0;
                        printf("Received from %d: %s\n", client, buf);

                       

                        // Kiem tra trang thai dang nhap
                        int j = 0;
                        for (; j < num_users; j++)
                            if (users[j] == client)
                                break;
                        
                        if (j == num_users) // Chua dang nhap
                        {
                            // Xu ly cu phap lenh dang nhap
                          
                            char cmd[32], id[32], tmp[32];
                            ret = sscanf(buf, "%s%s%s", cmd, id, tmp);
                            if (ret == 2)
                            {   int login=1;
                                if (strcmp(cmd, "JOIN") == 0)
                                {   
                                    if (validname(id)==true) {
                                    for(int i = 0; i < j; i++){
                                    if(j==0) 
                                    {login = 1;
                                     break;
                                    }
                                    else 
                                    {if(strcmp(id,user_ids[i])==0){
                                    login=0;
                                    break;
                                    }
                                    else login=1;
                                    }
                                    }
                                    if (login==1)
                                    {
                                    char *msg = "100 OK\n";
                                    send(client, msg, strlen(msg), 0);

                                    // Luu vao mang user
                                    users[num_users] = client;
                                    user_ids[num_users] =  (char*) malloc(strlen(id) + 1);
                                    strcpy(user_ids[num_users], id);
                                    num_users++;
                                char sendbuf[256];
                                sprintf(sendbuf,"JOIN %s",user_ids[j]);

                            // Forward du lieu cho cac user
                                 for (int k = 0; k < num_users; k++)
                                    if (users[k] != client)
                                    send(users[k], sendbuf, strlen(sendbuf), 0);
                                    }
                                    else {
                                    char *msg = "200 NICKNAME IN USE\n";
                                    send(client, msg, strlen(msg), 0);   
                                    }
                                    }
                                    else {
                                    char *msg = "201 INVALID NICK NAME\n";
                                    send(client, msg, strlen(msg), 0);    
                                    }
                                }
                                else
                                {
                                    char *msg = "999 UNKNOWN ERROR\n";
                                    send(client, msg, strlen(msg), 0);
                                }
                            }
                            else
                            {
                                char *msg = "999 UNKNOWN ERROR\n";
                                send(client, msg, strlen(msg), 0);
                            }
                        }
                        else // Da dang nhap
                        {   printf("ret= %d\n",ret);
                            // id: user_ids[j]
                            // data: buf
                            char cmd[32], tmp1[32], tmp2[32];
                            ret = sscanf(buf, "%s %s %s", cmd, tmp1, tmp2);
                            if (ret==2)
                            {if (strcmp(cmd,"MSG")==0)
                            {char sendbuf[256];
                            sprintf(sendbuf,"MSG %s %s",user_ids[j],tmp1);
                            char *msg = "100 OK\n";
                            send(client, msg, strlen(msg), 0);
                            // Forward du lieu cho cac user
                            for (int k = 0; k < num_users; k++)
                                if (users[k] != client)
                                    send(users[k], sendbuf, strlen(sendbuf), 0);
                            }
                            }
                            else if (ret==3) {
                            if(strcmp(cmd,"PMSG")==0) {
                            printf("%s\n",cmd);
                            printf("%s\n",tmp1);
                            printf("%s\n",tmp2);
                            for(int i = 0; i < j; i++){
                            if(strcmp(tmp1,user_ids[i])==0){
                            char sendbuf[256];
                            sprintf(sendbuf,"PMSG %s %s",user_ids[j],tmp2);
                            char *msg = "100 OK\n";
                            send(client, msg, strlen(msg), 0);
                            send(users[i],sendbuf,strlen(sendbuf),0);
                            
                            }
                            else {
                            char *msg = "202 UNKNOWN NICKNAME\n";
                            send(client, msg, strlen(msg), 0);    
                            }
                            }
                            }
                            else {
                            char *msg = "999 UNKNOWN ERROR\n";
                            send(client, msg, strlen(msg), 0);    
                            }
                            }
                        }
        
    }
    }
    close(client);
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

