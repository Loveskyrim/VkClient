#include <curl/curl.h>
#include <iostream>
#include <vk/client.hpp>
#include <vk/json.hpp>

namespace Vk
{
    using json = nlohmann::json;

    auto Client::check_connection() -> bool
    {
        CURL * curl = curl_easy_init();

        if (curl)
        {
            std::string fields = "access_token=" + settings_["token"] + "&v=5.59";
            std::string buffer = "";

            curl_easy_setopt(curl, CURLOPT_URL, "https://api.vk.com/method/account.getProfileInfo?");
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

            if (curl_easy_perform(curl) == CURLE_OK)
            {
                json jsn_obj = json::parse(buffer);
                json jsn_response = jsn_obj["response"];

                if (!jsn_response.is_null())
                {
                    curl_easy_cleanup(curl);
                    return true;
                }
            }
        }

        curl_easy_cleanup(curl);
        return false;
    }

    auto Client::write_callback(char *data, size_t size, size_t nmemb, std::string &buff) -> size_t
    {
        int result = 0;

        if (buff.c_str())
        {
            buff.append(data, size * nmemb);
            result = size * nmemb;
        }

        return result;
    }
}
