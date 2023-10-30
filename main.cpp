#include "main.h"

auto load_config(std::string_view path) -> std::tuple<int, std::string> {
    using namespace std::string_literals;
    
    auto result = std::tuple<int, std::string>{8080, "storage.db"};
    try
    {
        auto table = toml::parse_file(path);
        auto port = table["config"]["port"].value<int>();
        auto file = table["config"]["dbfile"].value<std::string>();
        if(port.has_value()) result = {port.value(), std::get<1>(result)};
        if(file.has_value()) result = {std::get<0>(result), file.value()};
    }
    catch (const toml::parse_error& err)
    {
        if(!std::filesystem::exists(path)) {
            auto ofstream = std::ofstream(path.data());
            ofstream << default_config;
        }
    }
    return result;
}

auto main() -> int {
    auto [port, db] = load_config("config.toml");

    auto storage = sqlite_orm::make_storage(db,
        sqlite_orm::make_table("urls",
            sqlite_orm::make_column("id", &shortener::Url::id, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("long_url", &shortener::Url::long_url),
            sqlite_orm::make_column("datetime_added", &shortener::Url::datetime_added),
            sqlite_orm::make_column("visits", &shortener::Url::visits)
        )
    );
    storage.sync_schema();

    auto app = crow::SimpleApp();
    CROW_ROUTE(app, "/api/create").methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req){
        auto long_url = std::string(req.url_params.get("long_url"));
        
        auto url = shortener::Url{
            0, 
            long_url, 
            static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()),
            0
        };
        auto id = storage.insert(url);
        url.id = id;

        auto short_url = shortener::idToShortUrl(id);
        storage.update(url);
        
        return crow::json::wvalue({"url", short_url});
    });

    CROW_ROUTE(app, "/api/stats").methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req){
        auto short_url = std::string(req.url_params.get("short_url"));
        auto id = shortener::shortUrlToId(short_url);
        if(auto url = storage.get_pointer<shortener::Url>(sqlite_orm::where(sqlite_orm::c(&shortener::Url::id) == id))) {       
            return crow::json::wvalue({"visits", std::to_string(url->visits)});
        }
        return crow::json::wvalue({"error", std::string("URL NOT FOUND!")});
    });

    CROW_ROUTE(app, "/<string>")
    ([&](std::string_view short_url){
        auto response = crow::response(404);
        auto id = shortener::shortUrlToId(short_url);
        if(auto url = storage.get_pointer<shortener::Url>(sqlite_orm::where(sqlite_orm::c(&shortener::Url::id) == id))) {
            url->visits++;
            storage.update(*url);
            response.redirect(url->long_url);
        }
        return response;
    });

    app.port(port).multithreaded().run();
}
