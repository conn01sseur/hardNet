// Поко только код спизденный с deepseek 

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024

void handleClient(int clientSocket) {
    cout << "Обработка нового клиента..." << endl;
    
    while(true) {
        string message;
        cout << "Введите сообщение для клиента (или 'exit' для завершения): ";
        getline(cin, message);
        
        if(message == "exit") {
            break;
        }
        
        send(clientSocket, message.c_str(), message.size(), 0);
        cout << "Сообщение отправлено клиенту" << endl;
    }
    
    close(clientSocket);
    cout << "Соединение с клиентом закрыто" << endl;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        cerr << "Ошибка создания сокета" << endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if(bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Ошибка привязки сокета" << endl;
        return -1;
    }

    if(listen(serverSocket, 5) < 0) {
        cerr << "Ошибка прослушивания" << endl;
        return -1;
    }

    cout << "Сервер запущен. Ожидание подключений..." << endl;

    while(true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        
        if(clientSocket < 0) {
            cerr << "Ошибка принятия подключения" << endl;
            continue;
        }
        
        cout << "Новое подключение!" << endl;
        thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
    return 0;
}