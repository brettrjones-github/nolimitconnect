@ECHO OFF

rem Build libCEC for Windows

SETLOCAL

SET MYDIR=%~dp0
SET BUILDTYPE=Release
SET VSVERSION=12
SET INSTALLPATH=%MYDIR%..\build

IF NOT EXIST "%MYDIR%..\support\windows\cmake\build.cmd" (
  ECHO "support git submodule was not checked out"
  GOTO exit
)

IF NOT EXIST "%MYDIR%..\src\platform\windows\build.cmd" (
  ECHO "platform git submodule was not checked out"
  GOTO exit
)

rmdir /s /q %MYDIR%..\build

FOR %%T IN (amd64 x86) DO (
  CALL %MYDIR%build-lib.cmd %%T %BUILDTYPE% %VSVERSION% %INSTALLPATH% nmake
  IF NOT ERRORLEVEL 0 (
    GOTO builderror
  )
)

rmdir /s /q %MYDIR%..\build\cmake
exit /b 0

:builderror
echo "Failed to build"

:exit
exit /b 1