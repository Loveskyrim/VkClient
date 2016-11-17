#include <vk/client.hpp>
#include <iostream>

void get_info(const Vk::Client::json & account);

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

    get_info(vk_cl.get_profile_info());

    return 0;
}

void get_info(const Vk::Client::json & account)
{

        Vk::Client::json jsn_fname = account["first_name"];
        if (!jsn_fname.is_null())
            std::cout << "first name: " << jsn_fname.begin().value() << std::endl;

        Vk::Client::json jsn_lname = account["last_name"];
        if (!jsn_lname.is_null())
            std::cout << "last name: " << jsn_lname.begin().value() << std::endl;

	Vk::Client::json jsn_bdate = account["bdate"];
        if (!jsn_bdate.is_null())
            std::cout << "birthday: " << jsn_bdate.begin().value() << std::endl;

        Vk::Client::json jsn_sex = account["sex"];
        if (!jsn_sex.is_null())
            std::cout << "sex: " << ((int)jsn_sex.begin().value() == 1 ? "female" : "male") << std::endl;

	Vk::Client::json jsn_hometown = account["home_town"];
        if (!jsn_hometown.is_null())
            std::cout << "home town: " << jsn_hometown.begin().value() << std::endl;

