# redis-lite

Educational in-memory key-value server in **C++17**.

Supports a small text command set inspired by Redis:

`PING`, `SET` [`EX` seconds], `GET`, `DEL`, `EXISTS`, `EXPIRE`, `TTL`, `PERSIST`, `QUIT`.

## Status

Store, TTL commands, command dispatch, TCP server, unit tests, and CI.

## Build

```bash
make
make test
./redis-lite --port 6379
```

Windows (MinGW / LLVM):

```bat
build.bat
build.bat test
build\redis-lite.exe --port 6379
```

## TTL semantics

| Command | Result |
|---------|--------|
| `EXPIRE key seconds` | `1` if key exists and seconds `> 0`, else `0` |
| `TTL key` | `-2` missing, `-1` no expire, else remaining seconds |
| `PERSIST key` | `1` if expiration removed, else `0` |

`SET key value EX seconds` rejects non-positive or non-integer seconds.

## Try it

In another terminal (PowerShell):

```powershell
$c = New-Object System.Net.Sockets.TcpClient("127.0.0.1", 6379)
$s = $c.GetStream()
$w = New-Object System.IO.StreamWriter($s); $w.AutoFlush = $true
$r = New-Object System.IO.StreamReader($s)
$w.WriteLine("PING")
$r.ReadLine()
$w.WriteLine("SET foo bar EX 30")
$r.ReadLine()
$w.WriteLine("TTL foo")
$r.ReadLine()
$w.WriteLine("QUIT")
$c.Close()
```

## Library

```cpp
#include "store.hpp"
#include "protocol.hpp"

redislite::Store store;
auto reply = redislite::handle_line(store, "SET foo bar EX 30");
```

## Requirements

- C++17 compiler
- On Windows, link with `ws2_32` (handled by `build.bat` / Makefile)

## License

MIT
