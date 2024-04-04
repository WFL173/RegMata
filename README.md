# RegMata
RegMata is a header library that uses graph and automata theory to automate the process of compiling state machines, written in C++. This project has been inspired by a CACM paper about a regular expression search algorithm written in 1968 by Ken Thompson and a post about [regular expression matching](https://swtch.com/~rsc/regexp/regexp1.html) by Russ Cox. 

To add this library to your C++ project, just copy all the .hpp files in the src folder into your project and include the Automata.hpp file.

**Warning: this is a project made for my personal use. Use  it at your own risk.**

## Building
This library is platform agnostic, therefore you can use this library on any OS. To include this library it is sufficient to just copy all the .hpp files and then to include Automata.hpp. However if you want to run the tests this project uses you can use the build scripts provided to build. 

### Windows
Windows does not come pre-installed with a default compiler. Therefore, to compile for Windows, build tools for visual studio will be necessary. These tools can be downloaded from [here](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) under tools for visual studio > build tools for visual studio. After downloading the tools open the x64 native tools command prompt or if you already have visual studio installed you can open a command prompt and call vcvarsall.bat located in your visual studio files to setup the build environment in the command prompt. In the command prompt you just configured, change your current working directory to the RegMata folder and run the commands below and the project will be built and run.

    build.bat
    build\debug\Main.exe
    
If you want a release version the project run the commands below to build and run.

    build.bat release
    build\release\Main.exe

You could also open the project folder in visual studio code using the configured command prompt. After doing this you will be able to build with your visual studio code build shortcut.

### LINUX
The linux build scripts require the g++ compiler to build. If you don't have g++ you can install it using the command below.

    sudo apt install g++

With the compiler installed change your current workind directory to the project folder and run the command below.

    ./build.sh
    ./build/debug/Main.out
    
If you want a release version the project run the commands below to build and run.

    ./build.sh release
    ./build/debug/Main.out

You could also open the project folder in visual studio code using the configured command prompt. After doing this you will be able to build with your visual studio code build shortcut. 