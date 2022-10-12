cmake_minimum_required(VERSION 3.20)
project(mimedemo)

file(GLOB_RECURSE SRC "src/*.cpp")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

find_package(Qt5 COMPONENTS Gui Widgets)

add_executable(${PROJECT_NAME}
    ${SRC}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Gui
    Qt5::Widgets
)

set(CMAKE_INSTALL_PREFIX /usr)
install(TARGETS mimedemo RUNTIME)
install(FILES misc/mimedemo.desktop DESTINATION share/applications)
install(FILES misc/mimedemo.xml DESTINATION share/mime/packages)