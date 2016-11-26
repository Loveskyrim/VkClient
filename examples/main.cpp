#include <vk/client.hpp>
#include <thread>
#include <mutex>

std::mutex mutex;
size_t thread_index = 0;

void get_info(const Vk::Client::json & account);
auto print_audios(const Vk::Client::json & audios, size_t threads_count, bool is_debug, size_t current_index) -> void;

int main(int argc, char * argv[])
{
    std::string token;
    std::cout << "Для получения токена пройдите по ссылке:" << std::endl;
    std::cout << "https://oauth.vk.com/authorize?client_id=5719956&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=audio&response_type=token&v=5.60" << std::endl;
    std::cout << "Токен: ";
    std::cin >> token;
	
	if (!std::cin)
        std::cerr << "Неверный ввод" << std::endl;
	
    Vk::Client vk_cl({{"token", token}});

    if (vk_cl.check_connection())
        std::cout << "Подключено." << std::endl;

    get_info(vk_cl.get_profile_info());

    size_t n;
    std::cout << "Введите количество одновременно запускаемых потоков: " << std::endl;
    std::cin >> n;
	
	if (!std::cin)
        std::cerr << "Неверный ввод" << std::endl;

    if (n < 1 || n > std::thread::hardware_concurrency())
	    std::cout << "Неверное число потоков" << std::endl;
    else
    {
        std::vector<std::thread> threads;
        auto audios = vk_cl.get_audios();

        for (auto i = 0; i < n; i++)
            threads.push_back(std::thread(print_audios, audios, n, argc == 2 && strcmp(argv[1], "-v") == 0, i));

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
            std::cout << "sex: " << ((int) jsn_sex.begin().value() == 1 ? "female" : "male") << std::endl;

        Vk::Client::json jsn_hometown = account["home_town"];
        if (!jsn_hometown.is_null())
            std::cout << "home town: " << jsn_hometown.begin().value() << std::endl;
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << std::endl;
    }
}

auto print_audios(const Vk::Client::json & audios, size_t threads_count, bool is_debug, size_t current_index) -> void
{
    for (auto i = current_index; i < audios.size(); i += threads_count)
    {
        while (thread_index != current_index)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        std::lock_guard<std::mutex> lock(mutex);

        if (is_debug)
        {
            std::time_t timer = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << "Thread " << current_index + 1 << std::endl << "Start time: " << ctime(&timer);
        }

        std::cout << i + 1 << ". ";
        auto value = audios.at(i);

        Vk::Client::json jsn_artist = value["artist"];
        if (!jsn_artist.is_null())
            std::cout << "Artist: " << jsn_artist.begin().value() << std::endl;

        Vk::Client::json jsn_title = value["title"];
        if (!jsn_title.is_null())
            std::cout << "Title: " << jsn_title.begin().value() << std::endl;

        Vk::Client::json jsn_duration = value["duration"];
        if (!jsn_duration.is_null())
            std::cout << "Duration: " << jsn_duration.begin().value() << std::endl;

        if (thread_index < threads_count - 1)
            thread_index++;
        else
            thread_index = 0;

        if (is_debug)
        {
            std::time_t timer = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << "End time: " << ctime(&timer) << std::endl;
        }
    }
}
