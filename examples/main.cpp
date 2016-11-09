#include <vk/client.hpp>
#include <iostream>

int main()
{
    std::string token;
    std::cout << "Для получения токена пройдите по ссылке:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5719956&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.59" << std::endl;
    std::cout << "Токен: ";
    std::cin >> token;
    Vk::Client vk_cl({{"token", token}});

    if (vk_cl.check_connection())
        std::cout << "Подключено." << std::endl;

    return 0;
}
