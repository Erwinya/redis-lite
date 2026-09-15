# redis-lite

Educational in-memory key-value server in **C++17**.

Supports a small text command set inspired by Redis (`PING`, `SET`, `GET`, `DEL`, `EXISTS`, `QUIT`).

## Status

In-memory `SET` / `GET` store is in place. TTL, TCP loop, remaining commands, and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "store.hpp"

redislite::Store store;
store.set("foo", "bar");
auto v = store.get("foo");  // "bar"
```

## Requirements

- C++17 compiler

## License

MIT
