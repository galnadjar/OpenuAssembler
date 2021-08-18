cmake_minimum_required(VERSION 3.19)
project(MAMAN14 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS -m32)

add_executable(MAMAN14 main.c errorsHeader.h fileHandling.c fileHandling.h symbolTable.c symbolTable.h spaceParse.c spaceParse.h lineParsing.c lineParsing.h entryExternDataStructures.h entryExternDataStructures.c instructionFuncs.c instructionFuncs.h codeImg.c codeImg.h dataImg.c dataImg.h directiveFuncs.c directiveFuncs.h handleNumbers.c handleNumbers.h labelHandling.c labelHandling.h labelAsArg.c labelAsArg.h printFile.c printFile.h)