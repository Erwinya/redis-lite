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

if /I "%~1"=="test" (
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude tests\store_test.cpp src\store.cpp -o build\store_test.exe
  if errorlevel 1 exit /b 1
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude tests\protocol_test.cpp src\store.cpp src\protocol.cpp -o build\protocol_test.exe
  if errorlevel 1 exit /b 1
  build\store_test.exe
  if errorlevel 1 exit /b 1
  build\protocol_test.exe
  if errorlevel 1 exit /b 1
  echo Tests passed.
  goto :eof
)

echo Run:  build\redis-lite.exe --port 6379
echo Test: build.bat test
endlocal
