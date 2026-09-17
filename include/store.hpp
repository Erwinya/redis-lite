#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

namespace redislite {

class Store {
public:
    void set(std::string key, std::string value,
             std::optional<std::chrono::seconds> ttl = std::nullopt);
    std::optional<std::string> get(const std::string &key);
    std::size_t size() const { return entries_.size(); }

private:
    struct Entry {
        std::string value;
        std::optional<std::chrono::steady_clock::time_point> expires;
    };

    void purge_if_expired(const std::string &key);
    std::unordered_map<std::string, Entry> entries_;
};

}  // namespace redislite
