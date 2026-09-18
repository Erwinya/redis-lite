#include "store.hpp"

namespace redislite {

void Store::purge_if_expired(const std::string &key) {
    auto it = entries_.find(key);
    if (it == entries_.end()) return;
    if (it->second.expires && std::chrono::steady_clock::now() >= *it->second.expires) {
        entries_.erase(it);
    }
}

void Store::set(std::string key, std::string value,
                std::optional<std::chrono::seconds> ttl) {
    Entry e;
    e.value = std::move(value);
    if (ttl) {
        e.expires = std::chrono::steady_clock::now() + *ttl;
    }
    entries_[std::move(key)] = std::move(e);
}

std::optional<std::string> Store::get(const std::string &key) {
    purge_if_expired(key);
    auto it = entries_.find(key);
    if (it == entries_.end()) return std::nullopt;
    return it->second.value;
}

bool Store::del(const std::string &key) {
    purge_if_expired(key);
    return entries_.erase(key) > 0;
}

}  // namespace redislite
