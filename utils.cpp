#include "utils.h"

#include <algorithm>
#include <regex>

inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

std::vector<std::string> split(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos)
    {
        token = str.substr(start, end - start);
        trim(token);

        if (token.length() != 0)
        {
            tokens.push_back(token);
        }

        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    token = str.substr(start, str.length() - start);
    trim(token);

    if (token.length() != 0)
    {
        tokens.push_back(token);
    }

    return tokens;
}

bool try_parse_int(const std::string &str, int &value)
{
    try
    {
        value = std::stoi(str);
        return true;
    }
    catch (const std::invalid_argument &e)
    {
        return false;
    }
    catch (const std::out_of_range &e)
    {
        return false;
    }
}

bool is_color(const std::string &str)
{
    return std::regex_match(str, std::regex("^#\\w{6}$"));
}
