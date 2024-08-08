@echo off
goto:$Main

:$Main
setlocal EnableExtensions
  if not exist "%~dp0bin" (
    mkdir "%~dp0bin"
  )

  REM Add the following to get more verbose output
  REM -v

  REM -fno-integrated-cc1

  call zig cc ^
    -o "%~dp0bin\dwm-win32.exe" ^
      -target x86_64-windows-msvc ^
      -std=c99 ^
      -pedantic ^
      -Wall ^
      -Wno-error ^
      -O2 ^
      -s ^
      -Wno-deprecated-declarations ^
      -Wno-empty-body ^
      -Wno-int-to-pointer-cast ^
      -Wno-strict-prototypes ^
      -Wno-misleading-indentation ^
      -Wno-pointer-to-int-cast ^
      -Wno-language-extension-token ^
      -Wno-incompatible-pointer-types ^
      -Wno-format ^
      -DPROJECT_NAME="dwm-win32" ^
      -DPROJECT_VER="0.1.2" ^
      -DPROJECT_VER_MAJOR=0 ^
      -DPROJECT_VER_MINOR=1 ^
      -DPROJECT_VER_PATCH=2 ^
      -DNDEBUG ^
    -Isrc ^
      src/dwm-win32.c ^
      src/bstack.c ^
      src/win32_utf8.c ^
      src/fibonacci.c ^
      src/gaplessgrid.c ^
      src/grid.c ^
      src/mods/client.c src/mods/display.c src/mods/dwm.c src/mods/eventemitter.c src/mods/hotkey.c ^
    -Iextern/luabitop ^
      extern/luabitop/bit.c ^
    -Iextern/lua/src ^
      extern/lua/src/lapi.c extern/lua/src/lauxlib.c extern/lua/src/lbaselib.c ^
      extern/lua/src/lcode.c extern/lua/src/ldblib.c extern/lua/src/ldebug.c ^
      extern/lua/src/ldo.c extern/lua/src/ldump.c extern/lua/src/lfunc.c ^
      extern/lua/src/lgc.c extern/lua/src/linit.c extern/lua/src/liolib.c ^
      extern/lua/src/llex.c extern/lua/src/lmathlib.c extern/lua/src/lmem.c ^
      extern/lua/src/loadlib.c extern/lua/src/lobject.c extern/lua/src/lopcodes.c ^
      extern/lua/src/loslib.c extern/lua/src/lparser.c extern/lua/src/lstate.c ^
      extern/lua/src/lstring.c extern/lua/src/lstrlib.c extern/lua/src/ltable.c ^
      extern/lua/src/ltablib.c extern/lua/src/ltm.c extern/lua/src/lundump.c ^
      extern/lua/src/lvm.c extern/lua/src/lzio.c ^
    -Iextern/lua-compat-5.3 ^
      extern/lua-compat-5.3/compat-5.3.c

  echo Build complete: %ERRORLEVEL%
endlocal & exit /b %ERRORLEVEL%
