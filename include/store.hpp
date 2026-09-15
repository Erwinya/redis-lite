#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace redislite {

/// In-memory key/value store (TTL and command protocol land later).
class Store {
public:
    void set(std::string key, std::string value);
    std::optional<std::string> get(const std::string &key) const;
    std::size_t size() const { return entries_.size(); }

private:
    std::unordered_map<std::string, std::string> entries_;
};

}  // namespace redislite
