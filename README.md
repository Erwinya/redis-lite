# redis-lite

Educational in-memory key-value server in **C++17**.

Supports a small text command set inspired by Redis:

- `PING`
- `SET key value [EX seconds]`
- `GET key`
- `DEL key`
- `EXISTS key`
- `QUIT`

Single-threaded accept loop. Useful for learning sockets, TTLs, and protocol design — not a Redis replacement.

## Build

```bash
make
```

Windows:

```bat
build.bat
```

## Run

```bash
./redis-lite --port 6379
```

Example session (netcat / `Test-NetConnection` client / another TCP tool):

```text
PING
SET foo bar EX 30
GET foo
EXISTS foo
DEL foo
QUIT
```

## License

MIT
