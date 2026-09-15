#include "store.hpp"

namespace redislite {

void Store::set(std::string key, std::string value) {
    entries_[std::move(key)] = std::move(value);
}

std::optional<std::string> Store::get(const std::string &key) const {
    auto it = entries_.find(key);
    if (it == entries_.end()) return std::nullopt;
    return it->second;
}

}  // namespace redislite
