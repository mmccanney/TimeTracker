cmake_minimum_required(VERSION 3.24)
project(TimeTracker)

set(CMAKE_CXX_STANDARD 20)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
find_package(SQLite3)

include(${wxWidgets_USE_FILE})
include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(TimeTracker main.cpp wxApp.cpp jobs.cpp jobevents.cpp tcontroller.cpp wxViews.cpp)

#target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} stdc++fs)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(${PROJECT_NAME} stdc++fs)
target_link_libraries (${PROJECT_NAME} ${SQLite3_LIBRARIES})

# you can put them all in a one-liner
#target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${SQLite3_LIBRARIES} stdc++fs)
