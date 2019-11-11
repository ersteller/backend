# CMakeList.txt: CMake-Projekt für "backend". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
# add_executable (backend "backend.cpp" "backend.h")
add_library(backend SHARED backend.cpp "backend.h")

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
# add_test(NAME loadlib COMMAND "python ../tests/loadlib.py")
