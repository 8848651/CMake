# Repository Overview

## 1. Project purpose (from code)
This repository is a small C++14 playground that combines:

- a custom header-only "stl" module (`stl_core` INTERFACE target), and
- a minimal Muduo-inspired event-driven TCP server implementation (`muduo/version_5`, target `v5`).

The executable `main` simply calls `test()` (defined in `muduo/version_5/include/version.h`), which constructs a `tcpserver`, initializes the event loop, and prints incoming client data.

## 2. Build layout

- **Top-level CMake**: creates object library `common_main` from `src/main.cpp`, adds subdirectories `stl` and `muduo/version_5`, then links executable `main` against static library `v5`.
- **stl/CMakeLists.txt**: defines `stl_core` as INTERFACE and exports `stl/include`.
- **muduo/version_5/CMakeLists.txt**: globs `src/*.cpp` into static library `v5`, exports `include/` as PUBLIC.

## 3. Code organization

### 3.1 Entry point
- `src/main.cpp` includes several custom headers and calls `test()`.

### 3.2 Server stack (`muduo/version_5`)
- `tcpserver`: owns base event loop and acceptor; installs connection callback.
- `acceptor`: accepts inbound TCP connections and forwards accepted fd.
- `eventloop`: wraps epoll polling + task wakeup via `eventfd`.
- `poller`: thin epoll wrapper; stores channels and waits for readable events.
- `channel`: binds fd + callbacks and can register itself to loop.
- `tcpthread`: thread wrapper around an `eventloop` (present but not heavily used from `test()`).

### 3.3 Custom headers (`stl/include`)
Contains custom containers/meta utilities (e.g., tuple/function/bind/list/vector/string) that appear educational/experimental. Current executable path mostly uses `meta/*` includes in `main.cpp` but does not deeply exercise them.

## 4. Runtime flow (as currently wired)

1. `main()` calls `test()`.
2. `test()` constructs `tcpserver` with a message callback.
3. `tcpserver` initializes loop + acceptor and enters blocking `baseloop_->loop()`.
4. On new connection, server creates `channel`, sets read callback, and schedules `channel->update()` via pending task queue.
5. `poller::wait()` returns active channels; read callback reads from socket and prints message.

## 5. Notable technical observations

- `test()` is defined in a header (`version.h`) as a non-`inline` function. This is fine with current single translation-unit inclusion pattern, but could cause ODR multiple-definition issues if included in multiple `.cpp` files.
- CMake currently uses `file(GLOB ...)` for sources in `muduo/version_5`, convenient for experiments but less explicit for production maintenance.
- Existing `build/` cache references a different source path (`/workspaces/...`), so a fresh build directory (e.g., `build_local/`) is safer in this environment.

## 6. Quick commands

```bash
cmake -S . -B build_local
cmake --build build_local -j4
./build_local/main
```

(Last command starts server loop and blocks waiting for network events.)
