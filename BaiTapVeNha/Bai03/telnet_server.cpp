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
#include <iostream>
#include <sys/select.h>
#include <fstream>
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

    fd_set fdread;
    int clients[64];
    int loginclients[64];
    char buf[11];
    int num_clients = 0;
    
    string filename("telnet_pass.txt");
    vector<string> lines;
    string line;
 
    //Mở file bằng ifstream
    ifstream input_file(filename);
    //Kiểm tra file đã mở thành công chưa
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    //Đọc từng dòng trong
    while (getline(input_file, line)){
        lines.push_back(line);//Lưu từng dòng như một phần tử vào vector lines.
    }

    //Xuất từng dòng từ lines và in ra màn hình
    for (const auto &i : lines)
        cout << i << endl;

    //Đóng file
    
    
    
    
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
        
        
         // Kiểm tra sự kiện có dữ liệu truyền đến socket client
        for (int i = 0; i < num_clients; i++)
            if (FD_ISSET(clients[i], &fdread))
            {
                ret = recv(clients[i], buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                continue;
                }
                else 
                {
                buf[ret] = 0;
                char them[11] = " > out.txt";
                strcat(buf,them);
                cout << buf;
                }
                }
                
            }     
    

    input_file.close();
    close(listener);    

    return 0;
}