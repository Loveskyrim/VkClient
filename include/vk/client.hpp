#ifndef VKCLIENT_CLIENT_HPP
#define VKCLIENT_CLIENT_HPP

#include <string>
#include <map>
#include "json.hpp"

namespace Vk
{
    class Client
    {
    public:
        using dict_t = std::map<std::string, std::string>;
	using json = nlohmann::json;
        Client(dict_t settings) : settings_(settings) {}
        auto check_connection() -> bool;
	auto get_profile_info() -> json;

    private:
        dict_t settings_;
        static auto write_callback(char * data, size_t size, size_t nmemb, std::string & buff) -> size_t;
    };
}

#endif
