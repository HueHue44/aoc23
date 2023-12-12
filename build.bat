@echo off
setlocal

set out=out
set arg=%*
set peak="../peak"

IF not exist %out% (mkdir %out%)

call :build main
exit /b

:build
set opt=/std:c++20 /EHsc /W4 %arg%
set inc=/I%VULKAN_SDK%/include /I%peak%/src
set ext=user32.lib gdi32.lib shell32.lib ws2_32.lib ole32.lib %VULKAN_SDK%/lib/vulkan-1.lib %peak%/src/lib.cpp
where /q cl && (
    cl %opt% %inc% %~1.cpp %ext% /link /out:%out%/%~1.exe
    del %~1.obj
    del lib.obj
    exit /b
)
set opt=-static -static-libgcc -static-libstdc++ -Wall -Wextra -std=c++20 -msse4.1 %arg%
set inc=-I%VULKAN_SDK%/include -I%peak%/src
set ext=-L%VULKAN_SDK%/lib -lgdi32 -lws2_32 -lole32 -lvulkan-1 %peak%/src/lib.cpp
where /q g++ && (
    g++ %~1.cpp -o %out%/%~1 %opt% %inc% %ext%
    exit /b
)
echo Error: found no suitable compiler in PATH. Checked for cl and g++.
exit /b 1
