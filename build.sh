#!/bin/bash
IntDir=./int
OutputFile="Main.out"
CommonCompilerFlags="-std=c++17 -Wall -Wextra"
DebugCompilerFlags="-O0 -g"
ReleaseCompilerFlags="-O2"

mkdir -p ./build
mkdir -p ./int

CompilerMode="debug"
CompilerFlags="$CommonCompilerFlags $DebugCompilerFlags"

for var in $@;
do
    if [ "$var" = "release" ]; then
        CompilerMode="release"
        CompilerFlags="$CommonCompilerFlags $ReleaseCompilerFlags"
    fi
done

mkdir -p ./build/$CompilerMode
OutputDir=./build/$CompilerMode

# One line compilation. Does not create object files int folder will be empty.
g++ $CompilerFlags src/*.cpp -o $OutputDir/$OutputFile
echo "compilation returned with code $?"

# multi line compilation that creates object files.
# for File in src/*.c;
# do 
#     # shell parameter expansion
#     FilePathTrimmed=${File/"src/"/""}
#     FileExtSubbed=${FilePathTrimmed/".c"/".o"}
#     echo gcc -c $File -o $IntDir/$FileExtSubbed
# done

# gcc src/*.o -o $OutputDir/$OutputFile
