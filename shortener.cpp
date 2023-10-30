#include "main.h"

auto shortener::idToShortUrl(const uint64_t& input) -> std::string {
    const auto n = 34;
    auto tmp = input + 1000000000;
    auto chars = std::string{"abcdefghijkmnopqrstuvwxyz234567890"};
    auto output = std::string{""};

    while(tmp > 0) {
        output += chars.at(tmp%n);
        tmp /= n;
    }

    return output;
}

auto shortener::shortUrlToId(std::string_view input) -> uint64_t {
    const auto n = 34;
    auto chars = std::string{"abcdefghijkmnopqrstuvwxyz234567890"};
    auto output = 0u;

    for(auto i = 0; i < input.length(); i++) {
        const auto c = input.at(i);
        
        auto find = chars.find(c);
        if(find == std::string::npos)
            return 0;
        
        output += find * std::pow(34, i);
    }
    return output - 1000000000;
}
