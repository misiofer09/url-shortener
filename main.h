#include "lib/crow_all.h"
#include "lib/sqlite_orm.h"
#include "lib/toml.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <cmath>

using namespace std::string_literals;
const static auto default_config = 
R"([config]
port = 8080
dbfile = "storage.db"
)"s;

namespace shortener {
    struct Url {
        int id;
        std::string long_url;
        uint64_t datetime_added;
        int visits;
    };

    auto idToShortUrl(const uint64_t& input) -> std::string;
    auto shortUrlToId(std::string_view input) -> uint64_t;

}