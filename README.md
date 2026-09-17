# redis-lite

Educational in-memory key-value server in **C++17**.

Supports a small text command set inspired by Redis (`PING`, `SET`, `GET`, `DEL`, `EXISTS`, `QUIT`).

## Status

In-memory `SET` / `GET` with optional TTL are in place. TCP loop, remaining commands, and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "store.hpp"
#include <chrono>

redislite::Store store;
store.set("foo", "bar", std::chrono::seconds{30});  // expires in 30s
auto v = store.get("foo");
```

## Requirements

- C++17 compiler

## License

MIT
