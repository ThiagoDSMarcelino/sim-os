#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

inline void ltrim(std::string &s);

inline void rtrim(std::string &s);

inline void trim(std::string &s);

std::vector<std::string> split(const std::string &str, const std::string &delimiter);

bool try_parse_int(const std::string &str, int &value);

bool is_color(const std::string &str);

#endif // UTILS_H
