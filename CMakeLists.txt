cmake_minimum_required(VERSION 3.0.0)
project(cnsoftbei)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(ASIO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(./cpp cppfiles)
aux_source_directory(./head headfiles)

# add_executable(cnsoftbei main.cpp cpp/Part.cpp head/Part.h head/Lineitem.h cpp/dataReader.cpp head/dataReader.h cpp/Utils.cpp head/Utils.h cpp/ZipPart.cpp head/ZipPart.h cpp/ZipOrder.cpp head/ZipOrder.h cpp/PartData.cpp head/PartData.h cpp/BrandData.cpp head/BrandData.h cpp/TotalData.cpp head/TotalData.h cpp/Calculator.cpp head/Calculator.h cpp/server.cpp head/server.h cpp/client.cpp head/client.h head/message.h)
add_executable(cnsoftbei main.cpp ${cppfiles} ${headfiles})

target_compile_options(cnsoftbei PUBLIC -DASIO_STANDALONE)
target_include_directories(cnsoftbei PUBLIC ${ASIO_INCLUDE})

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    target_link_libraries(cnsoftbei PUBLIC -lwsock32 -lws2_32)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")


find_package(Threads REQUIRED)
target_link_libraries(cnsoftbei PUBLIC Threads::Threads)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
