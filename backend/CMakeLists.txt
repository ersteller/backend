# CMakeList.txt: CMake-Projekt für "backend". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

project(backend VERSION 1.0.1 DESCRIPTION "backend description")

# add_executable (backend "backend.cpp" "backend.h")
add_library(backend SHARED src/backend.cpp "src/backend.h")
set_target_properties(backend PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(backend PROPERTIES PUBLIC_HEADER include/backend_pub.h)

target_include_directories(backend PRIVATE include)
target_include_directories(backend PRIVATE src)
target_include_directories(backend PRIVATE ../libs/qpid-proton/build/cpp)
target_include_directories(backend PRIVATE ../libs/qpid-proton/cpp/include)

target_include_directories(backend PRIVATE ../libs/qpid-proton/build/c/include)



# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
# add_test(NAME loadlib COMMAND "python ../tests/loadlib.py")
