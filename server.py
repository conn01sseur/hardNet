import socket
import threading

HOST = '0.0.0.0'  # Принимаем подключения со всех интерфейсов
PORT = 8080       # Порт для прослушивания

def handle_client(conn, addr):
    print(f"Подключен клиент {addr}")
    try:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            print(f"Получено от {addr}: {data.decode()}")
            
            # Отправляем ответ
            response = input("Введите ответ для клиента: ")
            conn.sendall(response.encode())
    except ConnectionResetError:
        print(f"Клиент {addr} отключился неожиданно")
    finally:
        conn.close()
        print(f"Соединение с {addr} закрыто")

def start_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        print(f"Сервер запущен на {HOST}:{PORT}")
        
        try:
            while True:
                conn, addr = s.accept()
                thread = threading.Thread(target=handle_client, args=(conn, addr))
                thread.start()
                print(f"Активных подключений: {threading.active_count() - 1}")
        except KeyboardInterrupt:
            print("\nСервер останавливается...")

if __name__ == "__main__":
    start_server()