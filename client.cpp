#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024
#define RECONNECT_DELAY 10 // seconds

bool connectToServer(int &sock, sockaddr_in &serv_addr) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        cerr << "Ошибка создания сокета" << endl;
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Неверный адрес" << endl;
        return false;
    }

    if(connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Ошибка подключения" << endl;
        return false;
    }

    return true;
}

void receiveMessages(int sock) {
    char buffer[BUFFER_SIZE];
    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(sock, buffer, BUFFER_SIZE, 0);
        
        if(bytesReceived <= 0) {
            cout << "Соединение с сервером потеряно" << endl;
            close(sock);
            break;
        }
        
        cout << "Получено сообщение от сервера: " << buffer << endl;
    }
}

int main() {
    int sock;
    sockaddr_in serv_addr;

    while(true) {
        if(connectToServer(sock, serv_addr)) {
            cout << "Успешно подключено к серверу. Ожидание сообщений..." << endl;
            
            receiveMessages(sock);
        }

        cout << "Попытка переподключения через " << RECONNECT_DELAY << " секунд..." << endl;
        this_thread::sleep_for(chrono::seconds(RECONNECT_DELAY));
    }

    return 0;
}