@echo off

SET OutputFile=Main.exe
SET IntDir=..\\..\\int\\
SET CommonCompilerFlags=/utf-8 /std:c++17 /W4 /nologo /Fo%IntDir% /Fd%IntDir%
SET DebugCompilerFlags=/Od /Zi /MTd
SET ReleaseCompilerFlags=/O2 /Zo /MT /WX
REM /ENTRY:<function name> /OPT:REF /INCREMENTAL:NO /SUBSYSTEM:<CONSOLE | WINDOWS> /STACK:0x100000,0x100000
SET CommonLinkerFlags=/INCREMENTAL:NO /OPT:REF /SUBSYSTEM:CONSOLE 
SET ResourceFlags=

IF NOT EXIST build mkdir build
IF NOT EXIST int mkdir int
pushd build

IF "%1"=="release" (
    SET CompilerMode=release
    SET CompilerFlags=%CommonCompilerFlags% %ReleaseCompilerFlags%
) ELSE (
    SET CompilerMode=debug
    SET CompilerFlags=%CommonCompilerFlags% %DebugCompilerFlags%
)

IF NOT EXIST %CompilerMode% mkdir %CompilerMode%
pushd %CompilerMode%

del *.db > NUL 2> NUL

@echo on

cl %CompilerFlags% ..\\..\\src\\*.cpp /link %CommonLinkerFlags% /OUT:%OutputFile%
SET LastError=%ERRORLEVEL%

@echo off

popd
popd