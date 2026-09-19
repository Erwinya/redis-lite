@echo off
setlocal
where g++ >nul 2>&1
if errorlevel 1 (
  echo g++ not found. Install LLVM-MinGW or MSYS2, then re-run build.bat
  exit /b 1
)
if not exist build mkdir build
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src\main.cpp src\store.cpp src\protocol.cpp -o build\redis-lite.exe -lws2_32
if errorlevel 1 exit /b 1
echo Built build\redis-lite.exe
echo Run: build\redis-lite.exe --port 6379
endlocal
