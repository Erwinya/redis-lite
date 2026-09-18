#include "protocol.hpp"

#include <cctype>
#include <sstream>
#include <vector>

namespace redislite {
namespace {

std::string upper(std::string s) {
    for (char &c : s) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return s;
}

std::vector<std::string> split_ws(const std::string &line) {
    std::istringstream in(line);
    std::vector<std::string> parts;
    std::string tok;
    while (in >> tok) {
        parts.push_back(tok);
    }
    return parts;
}

}  // namespace

std::string handle_line(Store &store, const std::string &line) {
    auto parts = split_ws(line);
    if (parts.empty()) return "";

    const std::string cmd = upper(parts[0]);
    if (cmd == "PING") return "+PONG\n";
    if (cmd == "QUIT") return "+OK\n";

    if (cmd == "SET") {
        if (parts.size() == 3) {
            store.set(parts[1], parts[2]);
            return "+OK\n";
        }
        if (parts.size() == 5 && upper(parts[3]) == "EX") {
            try {
                auto ttl = std::chrono::seconds{std::stoll(parts[4])};
                store.set(parts[1], parts[2], ttl);
                return "+OK\n";
            } catch (...) {
                return "-ERR invalid expire time in SET\n";
            }
        }
        return "-ERR wrong number of arguments for SET\n";
    }

    if (cmd == "GET") {
        if (parts.size() != 2) return "-ERR wrong number of arguments for GET\n";
        auto v = store.get(parts[1]);
        if (!v) return "$-1\n";
        return "$" + std::to_string(v->size()) + "\n" + *v + "\n";
    }

    if (cmd == "DEL") {
        if (parts.size() != 2) return "-ERR wrong number of arguments for DEL\n";
        return ":" + std::to_string(store.del(parts[1]) ? 1 : 0) + "\n";
    }

    if (cmd == "EXISTS") {
        if (parts.size() != 2) return "-ERR wrong number of arguments for EXISTS\n";
        return ":" + std::to_string(store.exists(parts[1]) ? 1 : 0) + "\n";
    }

    return "-ERR unknown command\n";
}

}  // namespace redislite
