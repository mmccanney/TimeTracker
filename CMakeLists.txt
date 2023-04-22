cmake_minimum_required(VERSION 3.24)
project(TimeTracker)

set(CMAKE_CXX_STANDARD 20)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

add_executable(TimeTracker main.cpp wxApp.cpp wxMainFrame.cpp)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} stdc++fs)
