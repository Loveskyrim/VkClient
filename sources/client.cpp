#include <curl/curl.h>
#include <iostream>
#include <vk/client.hpp>

namespace Vk
{
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
		try
		{
                    json jsn_obj = json::parse(buffer);
                    json jsn_response = jsn_obj["response"];

                    if (!jsn_response.is_null())
                    {
                        curl_easy_cleanup(curl);
                        return true;
                    }
		}
		catch (const std::exception & error)
		{
		    std::cerr << error.what() << std::endl;
		} 
            }
        }

        curl_easy_cleanup(curl);
        return false;
    }

    auto Client::get_profile_info() -> json
    {
	CURL * curl = curl_easy_init();

        if (curl)
        {
	    std::string fields = "access_token=" + settings_["token"] + "&v=5.60";
            std::string buffer = "";
	    curl_easy_setopt(curl, CURLOPT_URL, "https://api.vk.com/method/account.getProfileInfo?");
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

            if (curl_easy_perform(curl) == CURLE_OK)
            {
		try
		{
		    json jsn_obj = json::parse(buffer);
		    json jsn_response = jsn_obj["response"];

		    if (!jsn_response.is_null())
		    {
		        curl_easy_cleanup(curl);
		        return jsn_response;
		    }
		}
		catch (const std::exception & error)
		{
		    std::cerr << error.what() << std::endl;
		} 
            }			
	}

	curl_easy_cleanup(curl);
        return nullptr;	
    }

auto Client::get_audios() -> json
	{
        CURL * curl = curl_easy_init();

        if (curl)
        {
            std::string fields = "access_token=" + settings_["token"] + "&v=5.60";
            std::string buffer = "";
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.vk.com/method/audio.get?");
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fields.length());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

            if (curl_easy_perform(curl) == CURLE_OK)
            {
                try
                {
                    json jsn_obj = json::parse(buffer);
                    json jsn_response = jsn_obj["response"];

                    if (!jsn_response.is_null())
                    {
                        curl_easy_cleanup(curl);
                        return jsn_response["items"];
                    }
                }
                catch (const std::exception & error)
                {
                    std::cerr << error.what() << std::endl;
                }
            }
        }

        curl_easy_cleanup(curl);
        return nullptr;
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
