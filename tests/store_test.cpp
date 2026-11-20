#include "store.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

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
    store.set("a", "1");
    expect(store.exists("a"), "exists after set");
    expect(store.ttl("a") == -1, "no expire yet");
    expect(store.expire("a", std::chrono::seconds{2}), "expire ok");
    expect(store.ttl("a") >= 1, "ttl positive");
    expect(store.persist("a"), "persist ok");
    expect(store.ttl("a") == -1, "persist clears expire");
    expect(store.ttl("missing") == -2, "missing ttl");
    expect(!store.expire("missing", std::chrono::seconds{1}), "expire missing");

    store.set("tmp", "x", std::chrono::seconds{1});
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    expect(!store.exists("tmp"), "lazy expire");
    expect(store.ttl("tmp") == -2, "expired ttl");

    if (fails) {
        std::cerr << fails << " assertion(s) failed\n";
        return 1;
    }
    std::cout << "store_test: ok\n";
    return 0;
}
