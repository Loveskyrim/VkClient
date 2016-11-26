#include <vk/client.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex mutex;

void get_info(const Vk::Client::json & account);
void get_audios(const Vk::Client::json & audios, int index, int offset);

int main()
{
    std::string token;
    std::cout << "Для получения токена пройдите по ссылке:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5719956&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=audio&response_type=token&v=5.60" << std::endl;
    std::cout << "Токен: ";
    std::cin >> token;
    Vk::Client vk_cl({{"token", token}});

    if (vk_cl.check_connection())
        std::cout << "Подключено." << std::endl;

    //get_info(vk_cl.get_profile_info());

    /*std::cout <<*/ vk_cl.get_audios();


    size_t n;
    std::cout << "Введите количество одновременно запускаемых потоков: " << std::endl;
    std::cin >> n;

    if (n < 1 || n > std::thread::hardware_concurrency())
	std::cout << "Неверное число потоков" << std::endl;
    else
    {
	std::vector<std::thread> threads;
	for (auto i = 0; i < n; i++)
	    threads.push_back(std::thread(get_audios, vk_cl.get_audios(), i, n));

	for (auto i = 0; i < n; i++)
	    if (threads[i].joinable())
		threads[i].join();
    }

    return 0;
}

void get_info(const Vk::Client::json & account)
{
    try
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
    }
    catch (const std::exception & error)
    {
	std::cout << error.what() << std::endl;
    }
}

void get_audios(const Vk::Client::json & audios, size_t index, bool offset)
{
    auto counter = 0;

    for (Vk::Client::json::const_iterator it = audios.begin(); it != audios.end(); ++it)
    {
	//std::lock_guard<std::mutex> lock(mutex);
	std::cout << "Thread " << index + 1 << std::endl;
	std::cout << counter + 1 << std::endl;

	if (it.value().find("artist") != it.value().end())
	{
            Vk::Client::json jsn_artist = it.value()["artist"];
            if (!jsn_artist.is_null())
                std::cout << "Artist: " << jsn_artist.begin().value() << std::endl;
	}

	if (it.value().find("title") != it.value().end())
	{
            Vk::Client::json jsn_title = it.value()["title"];
            if (!jsn_title.is_null())
                std::cout << "Title: " << jsn_title.begin().value() << std::endl;
	}

	if (it.value().find("duration") != it.value().end())
	{
	    Vk::Client::json jsn_duration = it.value()["duration"];
            if (!jsn_duration.is_null())
                std::cout << "Duration: " << jsn_duration.begin().value() << std::endl << std::endl;
	}
	counter++;
	//std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
}
