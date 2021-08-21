cmake_minimum_required(VERSION 3.19)
project(MAMAN14 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS -m32)

add_executable(MAMAN14 main.c errorsHeader.h fileHandling.c fileHandling.h symbolTable.c symbolTable.h spaceParse.c spaceParse.h lineParsing.c lineParsing.h handleEntExt.h handleEntExt.c instructionFuncs.c instructionFuncs.h codeImg.c codeImg.h dataImg.c dataImg.h directiveFuncs.c directiveFuncs.h handleNumbers.c handleNumbers.h labelAnalyze.h labelAsArg.c labelAsArg.h printFile.c printFile.h labelAnalyze.c)