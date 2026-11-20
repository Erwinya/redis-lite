#include "protocol.hpp"

#include <iostream>
#include <string>

namespace {

int fails = 0;

void expect(bool cond, const std::string &msg) {
    if (!cond) {
        std::cerr << "FAIL: " << msg << '\n';
        ++fails;
    }
}

}  // namespace

int main() {
    redislite::Store store;
    expect(redislite::handle_line(store, "PING") == "+PONG\n", "ping");
    expect(redislite::handle_line(store, "SET foo bar") == "+OK\n", "set");
    expect(redislite::handle_line(store, "GET foo") == "$3\nbar\n", "get");
    expect(redislite::handle_line(store, "EXISTS foo") == ":1\n", "exists");
    expect(redislite::handle_line(store, "EXPIRE foo 30") == ":1\n", "expire");
    auto ttl = redislite::handle_line(store, "TTL foo");
    expect(ttl.rfind(':', 0) == 0, "ttl reply");
    expect(redislite::handle_line(store, "PERSIST foo") == ":1\n", "persist");
    expect(redislite::handle_line(store, "TTL foo") == ":-1\n", "ttl after persist");
    expect(redislite::handle_line(store, "SET bad x EX 0") == "-ERR invalid expire time in SET\n",
           "reject zero EX");
    expect(redislite::handle_line(store, "SET bad x EX 12x") == "-ERR invalid expire time in SET\n",
           "reject bad EX");
    expect(redislite::handle_line(store, "EXPIRE missing 5") == ":0\n", "expire missing");
    expect(redislite::handle_line(store, "DEL foo") == ":1\n", "del");

    if (fails) {
        std::cerr << fails << " assertion(s) failed\n";
        return 1;
    }
    std::cout << "protocol_test: ok\n";
    return 0;
}
