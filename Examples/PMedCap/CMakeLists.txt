cmake_minimum_required(VERSION 4.0)
project(my-project LANGUAGES CXX VERSION 0.0.1)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
Include(FetchContent)
# add all executables
add_executable(mainPMedCap src/mainPMedCap.cpp )    # main
add_executable(Example.Test tests/Example.Test.cpp )
# begin dependencies from cxxdeps.txt
# cxxdeps dependency OptFrame
FetchContent_Declare(OptFrame GIT_REPOSITORY https://github.com/optframe/optframe.git GIT_TAG master)
FetchContent_MakeAvailable(OptFrame)
target_link_libraries(mainPMedCap PRIVATE OptFrameHeaders)
target_link_libraries(Example.Test PRIVATE OptFrameHeaders)
# cxxdeps dependency Catch2
FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.5.1)
FetchContent_MakeAvailable(Catch2)
target_link_libraries(Example.Test PRIVATE Catch2::Catch2WithMain)
# finally, add all sources
set(SOURCES
	src/ProblemInstance.cpp
	src/InitialSolutionRandomGreedy.cpp
	src/InitialSolutionRandom.cpp
	src/InitialSolutionGreedy.cpp
	src/Evaluator.cpp
	src/NSSeqSWAP.cpp
	src/cidade.cpp
)
target_sources(mainPMedCap PRIVATE ${SOURCES})
target_sources(Example.Test PRIVATE ${SOURCES})