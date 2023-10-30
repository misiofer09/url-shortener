# url shortener
Simple url shortening backend, written in C++17 using: [Crow](https://github.com/CrowCpp/Crow) framework, [sqlite_orm](https://github.com/fnc12/sqlite_orm) and [toml++](https://github.com/marzer/tomlplusplus) for config file parsing.

## Building
Build with CMake

## Config
Stores port on which the app is running as well as path to sqlite database file in `config.toml`. It's created with default values if it does not exist. 

## API
```POST: /api/create?long_url=<url>```

Creates a short url from specified long_url. Should return JSON response with short id of created url.

```POST: /api/stats?short_url=<url>```

Should return JSON response with number of times specified url was visited, if it exists.

```GET: /<short_url_id>```

Redirects to long url if it exists in database.