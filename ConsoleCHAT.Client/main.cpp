#include <iostream>

#include "Chat.h"
#include "Socket.h"

using namespace std;

int socket_file_descriptor, connection;
struct sockaddr_in serveraddress, client;
char messageCS[MESSAGE_LENGTH];

int main()
{
	try {

        // Создадим сокет
        socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_file_descriptor == -1)
        {
            std::cout << "Creation of Socket failed!" << std::endl;
            exit(1);
        }

        // Установим адрес сервера
        serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        // Зададим номер порта
        serveraddress.sin_port = htons(PORT);
        // Используем IPv4
        serveraddress.sin_family = AF_INET;
        // Установим соединение с сервером
        connection = connect(socket_file_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
        if (connection == -1)
        {
            std::cout << "Connection with the server failed.!" << std::endl;
            exit(1);
        }


        // Запустим чат
		std::shared_ptr<bool> isRun = std::make_shared<bool>(true);
		Chat::getInstance()->attach(isRun);

		while (*isRun) {
			Chat::getInstance()->process();
		}
	}
	catch (std::bad_alloc& error) {
		std::cerr << "Ошибка выделения памяти: " << error.what() << std::endl;
	}
	catch (std::exception& error) {
		std::cerr << error.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Неопределённое исключение" << std::endl;
	}


    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);

	return EXIT_SUCCESS;
}