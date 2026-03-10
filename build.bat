@echo off
set "CPP_FILES="
for /R src %%f in (*.cpp) do call set "CPP_FILES=%%CPP_FILES%% "%%f""

set PATH=C:\msys64\mingw64\bin;%PATH%
echo Compiling files: %CPP_FILES%
C:\msys64\mingw64\bin\g++.exe -std=c++17 %CPP_FILES% -I include -I raylib_mingw/raylib-5.0_win64_mingw-w64/include -L raylib_mingw/raylib-5.0_win64_mingw-w64/lib -lraylib -lopengl32 -lgdi32 -lwinmm -o SwarmDroneSim.exe > build.log 2>&1
if %ERRORLEVEL% equ 0 (
    echo Build successful: SwarmDroneSim.exe
) else (
    echo Build failed. See build.log
)
