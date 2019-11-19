# CMakeList.txt: CMake-Projekt für "backend". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

project(backend VERSION 1.0.1 DESCRIPTION "backend description")

add_executable (backend src/backend.cpp "src/backend.hpp" src/main.cpp src/sender.cpp src/receiver.cpp)
#add_library(backend SHARED src/backend.cpp "src/backend.hpp" src/main.cpp src/sender.cpp src/receiver.cpp)
set_target_properties(backend PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(backend PROPERTIES PUBLIC_HEADER include/backend_pub.h)

target_link_libraries ( backend LINK_PRIVATE ${PLATFORM_LIBS} 
                        /project/libs/qpid-proton/build/c/libqpid-proton-core.so 
                        /project/libs/qpid-proton/build/c/libqpid-proton-proactor.so 
                        /project/libs/qpid-proton/build/c/libqpid-proton.so
                        /project/libs/qpid-proton/build/cpp/libqpid-proton-cpp.so
                        /usr/lib/x86_64-linux-gnu/libsqlite3.so.0 
                        ${CMAKE_DL_LIBS} 
                        ${CONNECT_CONFIG_LIBS} )

target_include_directories(backend PRIVATE include)
target_include_directories(backend PRIVATE src)
target_include_directories(backend PRIVATE ../libs/qpid-proton/build/cpp)
target_include_directories(backend PRIVATE ../libs/qpid-proton/cpp/include)
target_include_directories(backend PRIVATE ../libs/qpid-proton/build/c/include)


#target_include_directories(backend PRIVATE ../libs/qpid-proton/cpp/include/proton/codec)
#target_include_directories(backend PRIVATE ../libs/qpid-proton/cpp/include/proton)

# copy the template db
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../dev/database.db
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# TODO: add testcases for "make test"
