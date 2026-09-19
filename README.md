# redis-lite

Educational in-memory key-value server in **C++17**.

Supports a small text command set inspired by Redis (`PING`, `SET`, `GET`, `DEL`, `EXISTS`, `QUIT`).

## Status

Store, command dispatch, TCP server, and build scripts are in place.

## Build

```bash
make
./redis-lite --port 6379
```

Windows (MinGW / LLVM):

```bat
build.bat
build\redis-lite.exe --port 6379
```

## Try it

In another terminal (PowerShell):

```powershell
$c = New-Object System.Net.Sockets.TcpClient("127.0.0.1", 6379)
$s = $c.GetStream()
$w = New-Object System.IO.StreamWriter($s); $w.AutoFlush = $true
$r = New-Object System.IO.StreamReader($s)
$w.WriteLine("PING")
$r.ReadLine()
$w.WriteLine("SET foo bar")
$r.ReadLine()
$w.WriteLine("GET foo")
$r.ReadLine(); $r.ReadLine()
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
