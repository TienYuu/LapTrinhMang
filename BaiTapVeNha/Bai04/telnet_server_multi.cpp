#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


void signalHandler(int signo)
{
int stat;
printf("signo = %d\n", signo);
int pid = wait(&stat);
printf("child %d terminated.\n", pid);
return;
}

void process_request(int client, char *buf);


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
    
    signal(SIGCHLD, signalHandler); 
    while (1)
    {
    printf("Waiting for new client...\n");
    int client = accept(listener, NULL, NULL);
    if (fork() == 0) {
        // Tien trinh con
        close(listener);

        // Xu ly ket noi tu client
        int found = 0;
        
        
        while (1)
            {
                char buf[256];
                char tmp[256];
                int rev = recv(client, buf, sizeof(buf), 0);
                if (rev <= 0)
                    break;
                buf[rev]=0;
                if (found == 1) {
                // Da dang nhap
               
               
               if (buf[strlen(buf) - 1] == '\n')
                   buf[strlen(buf) - 1] = '\0';
                
                sprintf(tmp, "%s > out.txt", buf);
                
                 // Thuc hien lenh
                
                system(tmp);

                 // Tra ket qua cho client
                FILE *f = fopen("out.txt", "rb");
                while (!feof(f))
                {
                 
                 int ret = fread(tmp, 1, sizeof(tmp), f);
                 
                 if (ret <= 0)
                 break;
                
                send(client, tmp, ret, 0);
                }
               fclose(f);

                }
                else {
            // Xu ly yeu cau tu client
   
            // Chua dang nhap
        char user[32], pass[32], tmp[65], line[65];
        int ret = sscanf(buf, "%s%s%s", user, pass, tmp);
        if (ret == 2)
        {
            
            sprintf(tmp, "%s %s\n", user, pass);
            FILE *f = fopen("telnet_pass.txt", "r");
            while (fgets(line, sizeof(line), f) != NULL)
            {
                if (strcmp(line, tmp) == 0)
                {
                    found = 1;
                    
                    break;
                }                    
            }
            fclose(f);

            if (found)
            {
                char *msg = "Dang nhap thanh cong. Hay nhap lenh de thuc hien.\n";
                send(client, msg, strlen(msg), 0);
                
            }
        else
        {
                char *msg = "Nhap sai tai khoan. Hay nhap lai.\n";
                send(client, msg, strlen(msg), 0);
        }
        }
        else
        {
            char *msg = "Nhap sai cu phap. Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        }
        }
    
        
    
            }
            close(client);
            exit(0);
            }
            close(client);
        }
    close(listener);    
    killpg(0, SIGKILL);
    return 0;

    }

    


