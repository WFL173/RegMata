@echo off

SET OutputFile=Main.exe
SET IntDir=int\
SET CommonCompilerFlags=/utf-8 /std:c++17 /FC /W4 /nologo /Fo%IntDir% /Fd%IntDir%
SET DebugCompilerFlags=/Od /Zi /MTd
SET ReleaseCompilerFlags=/O2 /Zo /MT /WX
REM /ENTRY:<function name> /OPT:REF /INCREMENTAL:NO /SUBSYSTEM:<CONSOLE | WINDOWS> /STACK:0x100000,0x100000
SET CommonLinkerFlags=/INCREMENTAL:NO /OPT:REF /SUBSYSTEM:CONSOLE 

IF NOT EXIST build mkdir build
IF NOT EXIST int mkdir int

IF "%1"=="release" (
    SET CompilerMode=release
    SET CompilerFlags=%CommonCompilerFlags% %ReleaseCompilerFlags%
) ELSE (
    SET CompilerMode=debug
    SET CompilerFlags=%CommonCompilerFlags% %DebugCompilerFlags%
)

IF NOT EXIST build\%CompilerMode% mkdir build\%CompilerMode%
SET OutputDir=build\%CompilerMode%

del *.db > NUL 2> NUL

cl %CompilerFlags% src\*.cpp /link %CommonLinkerFlags% /OUT:%OutputDir%\%OutputFile%
echo compilation returned with code %ERRORLEVEL%
