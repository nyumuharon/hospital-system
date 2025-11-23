#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace Utils {
    inline std::string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << ltm->tm_mday << "/"
           << std::setfill('0') << std::setw(2) << (1 + ltm->tm_mon) << "/"
           << (1900 + ltm->tm_year);
        return ss.str();
    }

    inline std::string toLowerCase(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c){ return tolower(c); });
        return s;
    }
}

#endif
