# redis-lite

Educational in-memory key-value server in **C++17**.

Supports a small text command set inspired by Redis (`PING`, `SET`, `GET`, `DEL`, `EXISTS`, `QUIT`).

## Status

In-memory store (`SET` / `GET` / `DEL` / `EXISTS` + TTL) and text command dispatch (`handle_line`) are in place. TCP server loop and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "store.hpp"
#include "protocol.hpp"
#include <chrono>

redislite::Store store;
store.set("foo", "bar", std::chrono::seconds{30});
auto reply = redislite::handle_line(store, "GET foo");
store.exists("foo");
store.del("foo");
```

## Requirements

- C++17 compiler

## License

MIT
